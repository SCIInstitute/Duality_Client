#include "src/duality/SceneController3DImpl.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/Scene.h"

SceneController3DImpl::SceneController3DImpl(Scene& scene, const RenderParameters3D& initialParameters,
                                             std::shared_ptr<GLFrameBufferObject> fbo)
    : m_scene(scene)
    , m_parameters(initialParameters)
    , m_fbo(fbo) {
    m_scene.updateDatasets();
}

SceneController3DImpl::~SceneController3DImpl() = default;

void SceneController3DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    if (m_mvp == nullptr) {
        BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View3D);
        m_mvp = std::make_unique<MVP3D>(screenInfo, boundingBox);
    } else {
        m_mvp->updateScreenInfo(screenInfo);
    }
    if (m_renderDispatcher == nullptr) {
        m_renderDispatcher = std::make_unique<RenderDispatcher3D>(m_fbo);
    } else {
        // m_renderDispatcher->updateScreenInfo(); // FIXME
    }
}

void SceneController3DImpl::addTranslation(const IVDA::Vec2f& translation) {
    m_parameters.addTranslation(translation);
}

void SceneController3DImpl::addRotation(const IVDA::Mat4f& rotation) {
    m_parameters.addRotation(rotation);
}

void SceneController3DImpl::setZoom(const float zoom) {
    m_parameters.addZoom(zoom);
}

VariableInfoMap SceneController3DImpl::variableInfoMap() const {
    return m_scene.variableInfoMap(View::View3D);
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene, View::View3D);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene, View::View3D);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController3DImpl::render() {
    m_renderDispatcher->setMVP(m_mvp->calculate(m_parameters));
    m_renderDispatcher->startDraw();
    m_scene.dispatch(*m_renderDispatcher, View::View3D);
    m_renderDispatcher->finishDraw();
}
