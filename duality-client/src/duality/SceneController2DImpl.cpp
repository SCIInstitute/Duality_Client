#include "src/duality/SceneController2DImpl.h"

#include "duality/CoordinateSystem.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/Scene.h"

SceneController2DImpl::SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters,
                                             std::shared_ptr<GLFrameBufferObject> fbo)
    : m_scene(scene)
    , m_parameters(initialParameters)
    , m_fbo(fbo) {
    m_scene.updateDatasets();
    auto minMax = minMaxForCurrentAxis();
    m_parameters.setSlice((minMax.first + minMax.second) / 2);
}

SceneController2DImpl::~SceneController2DImpl() = default;

void SceneController2DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    if (m_mvp == nullptr) {
        BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
        m_mvp = std::make_unique<MVP2D>(screenInfo, boundingBox);
    } else {
        m_mvp->updateScreenInfo(screenInfo);
    }
    if (m_renderDispatcher == nullptr) {
        m_renderDispatcher = std::make_unique<RenderDispatcher2D>(m_fbo);
    } else {
        // m_renderDispatcher->updateScreenInfo(); // FIXME
    }
}

void SceneController2DImpl::addTranslation(const IVDA::Vec2f& translation) {
    m_parameters.addTranslation(translation);
}

void SceneController2DImpl::addRotation(const float rotationAngle) {
    m_parameters.addRotation(rotationAngle);
}

void SceneController2DImpl::addZoom(const float zoom) {
    m_parameters.addZoom(zoom);
}

std::pair<float, float> SceneController2DImpl::minMaxForCurrentAxis() const {
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    return std::make_pair(boundingBox.min[m_parameters.axis()], boundingBox.max[m_parameters.axis()]);
}

void SceneController2DImpl::setSlice(float slice) {
    m_parameters.setSlice(slice);
}

float SceneController2DImpl::slice() const {
    return m_parameters.slice();
}

void SceneController2DImpl::toggleAxis() {
    m_parameters.toggleAxis();
}

std::string SceneController2DImpl::labelForCurrentAxis() const {
    std::map<CoordinateAxis, std::string> mapper;
    mapper[CoordinateAxis::X_Axis] = "yz-plane";
    mapper[CoordinateAxis::Y_Axis] = "xz-plane";
    mapper[CoordinateAxis::Z_Axis] = "xy-plane";
    return mapper[m_parameters.axis()];
}

VariableInfoMap SceneController2DImpl::variableInfoMap() const {
    return m_scene.variableInfoMap(View::View2D);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController2DImpl::render() {
    m_renderDispatcher->setMVP(m_mvp->calculate(m_parameters));
    m_renderDispatcher->setAxis(m_parameters.axis());
    m_renderDispatcher->setSlice(m_parameters.slice());
    m_renderDispatcher->startDraw();
    m_scene.dispatch(*m_renderDispatcher, View::View2D);
    m_renderDispatcher->finishDraw();
}
