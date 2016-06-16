#include "src/duality/SceneController2DImpl.h"

#include "duality/CoordinateSystem.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/Scene.h"

SceneController2DImpl::SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters,
                                             std::shared_ptr<GLFrameBufferObject> fbo)
    : m_scene(scene) {
    m_scene.updateDatasets();
    m_renderDispatcher = std::make_unique<RenderDispatcher2D>(fbo, initialParameters);
}

SceneController2DImpl::~SceneController2DImpl() = default;

void SceneController2DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_renderDispatcher->updateScreenInfo(screenInfo);
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController2DImpl::setRedrawRequired() {
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::addTranslation(const IVDA::Vec2f& translation) {
    m_renderDispatcher->addTranslation(translation);
}

void SceneController2DImpl::addRotation(const float rotationAngle) {
    m_renderDispatcher->addRotation(rotationAngle);
}

void SceneController2DImpl::addZoom(const float zoom) {
    m_renderDispatcher->addZoom(zoom);
}

void SceneController2DImpl::setSlice(float slice) {
    m_renderDispatcher->setSlice(slice);
}

float SceneController2DImpl::slice() const {
    return m_renderDispatcher->slice();
}

void SceneController2DImpl::toggleAxis() {
    m_renderDispatcher->toggleAxis();
}

std::pair<float, float> SceneController2DImpl::minMaxForCurrentAxis() const {
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    auto axis = m_renderDispatcher->currentAxis();
    return std::make_pair(boundingBox.min[axis], boundingBox.max[axis]);
}

std::string SceneController2DImpl::labelForCurrentAxis(SceneController2D::AxisLabelMode mode) const {
    std::map<CoordinateAxis, std::string> mapper;
    if (mode == SceneController2D::AxisLabelMode::Mathematical) {
        mapper[CoordinateAxis::X_Axis] = "yz-plane";
        mapper[CoordinateAxis::Y_Axis] = "xz-plane";
        mapper[CoordinateAxis::Z_Axis] = "xy-plane";
    } else {
        mapper[CoordinateAxis::X_Axis] = "sagittal";
        mapper[CoordinateAxis::Y_Axis] = "axial";
        mapper[CoordinateAxis::Z_Axis] = "coronal";
    }
    return mapper[m_renderDispatcher->currentAxis()];
}

VariableInfoMap SceneController2DImpl::variableInfoMap() const {
    return m_scene.variableInfoMap(View::View2D);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View2D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController2DImpl::render() {
    m_renderDispatcher->startDraw();
    m_scene.dispatch(*m_renderDispatcher, View::View2D);
    m_renderDispatcher->finishDraw();
}
