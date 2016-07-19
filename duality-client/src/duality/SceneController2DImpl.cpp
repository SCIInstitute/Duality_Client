#include "src/duality/SceneController2DImpl.h"

#include "duality/CoordinateSystem.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/Scene.h"

SceneController2DImpl::SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters,
                                             std::shared_ptr<GLFrameBufferObject> fbo, std::shared_ptr<Settings> settings)
    : m_scene(scene)
    , m_renderDispatcher(std::make_unique<RenderDispatcher2D>(fbo, initialParameters, settings))
    , m_settings(settings) {
    m_scene.updateDatasets();
    m_sliderCalculator = std::make_unique<SliderParameterCalculator>(m_scene);
}

SceneController2DImpl::~SceneController2DImpl() = default;

void SceneController2DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_renderDispatcher->updateScreenInfo(screenInfo);
    BoundingBox boundingBox = m_scene.boundingBox(View::View2D);
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

bool SceneController2DImpl::supportsSlices() const {
    return m_sliderCalculator->supportsSlice();
}

int SceneController2DImpl::numSlicesForCurrentAxis() const {
    return m_sliderCalculator->numSlicesForAxis(m_renderDispatcher->currentAxis());
}

void SceneController2DImpl::setSlice(int slice) {
    assert(m_sliderCalculator->supportsSlice());
    auto sliderParameter = m_sliderCalculator->parameterForSlice(slice, m_renderDispatcher->currentAxis());
    m_renderDispatcher->setSliderParameter(sliderParameter);
}

std::pair<float, float> SceneController2DImpl::boundsForCurrentAxis() const {
    return m_sliderCalculator->boundsForAxis(m_renderDispatcher->currentAxis());
}

void SceneController2DImpl::setDepth(float depth) {
    auto sliderParameter = m_sliderCalculator->parameterForDepth(depth, m_renderDispatcher->currentAxis());
    m_renderDispatcher->setSliderParameter(sliderParameter);
}

void SceneController2DImpl::toggleAxis() {
    m_renderDispatcher->toggleAxis();
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

VariableMap SceneController2DImpl::variableMap() const {
    return m_scene.variableMap(View::View2D);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boundingBox = m_scene.boundingBox(View::View2D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boundingBox = m_scene.boundingBox(View::View2D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController2DImpl::render() {
    m_scene.render(*m_renderDispatcher);
}
