#include "src/duality/SceneController2DImpl.h"

#include "duality/CoordinateSystem.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/Scene.h"

SceneController2DImpl::SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters,
                                             std::function<void(int, int, const std::string&)> updateDatasetCallback,
                                             std::shared_ptr<GLFrameBufferObject> fbo, std::shared_ptr<Settings> settings)
    : m_scene(scene)
    , m_parameters(initialParameters)
    , m_fbo(fbo)
    , m_settings(settings)
    , m_renderDispatcher(std::make_unique<RenderDispatcher2D>(fbo, settings)) {
    m_scene.setUpdateDatasetCallback(updateDatasetCallback);
}

SceneController2DImpl::~SceneController2DImpl() = default;

void SceneController2DImpl::initializeSliderCalculator() {
    m_scene.updateDatasets();
    m_sliderCalculator = std::make_unique<SliderParameterCalculator>(m_scene);
    if (m_parameters == RenderParameters2D()) {
        auto middle = (m_boundingBox.min[m_parameters.axis()] + m_boundingBox.max[m_parameters.axis()]) / 2;
        m_parameters.setSliderParameter(m_sliderCalculator->parameterForDepth(middle, m_parameters.axis()));
    }
}

std::shared_ptr<Settings> SceneController2DImpl::settings() const {
    return m_settings;
}

void SceneController2DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    m_screenInfo = screenInfo;
    m_boundingBox = m_scene.boundingBox(View::View2D);
    m_mvp = MVP2D(m_screenInfo, m_boundingBox, m_parameters);
}

void SceneController2DImpl::setNodeUpdateEnabled(const std::string& name, bool enabled) {
    m_scene.setNodeUpdateEnabled(name, enabled);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::updateDatasets() {
    m_scene.updateDatasets();
}

void SceneController2DImpl::initializeDatasets() {
    m_scene.initializeDatasets();
}

void SceneController2DImpl::setRedrawRequired() {
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::addTranslation(const IVDA::Vec2f& translation) {
    m_parameters.addTranslation(translation);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::addRotation(const float rotationAngle) {
    m_parameters.addRotation(rotationAngle);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::addZoom(const float zoom) {
    m_parameters.addZoom(zoom);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

bool SceneController2DImpl::supportsSlices() const {
    return m_settings->useSliceIndices() && m_sliderCalculator->supportsSlice();
}

int SceneController2DImpl::numSlicesForCurrentAxis() const {
    return m_sliderCalculator->numSlicesForAxis(m_parameters.axis());
}

void SceneController2DImpl::setSlice(int slice) {
    assert(m_sliderCalculator->supportsSlice());
    auto sliderParameter = m_sliderCalculator->parameterForSlice(slice, m_parameters.axis());
    m_parameters.setSliderParameter(sliderParameter);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

std::pair<float, float> SceneController2DImpl::boundsForCurrentAxis() const {
    return m_sliderCalculator->boundsForAxis(m_parameters.axis());
}

void SceneController2DImpl::setDepth(float depth) {
    auto sliderParameter = m_sliderCalculator->parameterForDepth(depth, m_parameters.axis());
    m_parameters.setSliderParameter(sliderParameter);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

SliderParameter SceneController2DImpl::sliderParameter() const {
    return m_parameters.sliderParameter();
}

void SceneController2DImpl::toggleAxis() {
    m_parameters.toggleAxis();
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
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
    return mapper[m_parameters.axis()];
}

VariableMap SceneController2DImpl::variableMap() const {
    return m_scene.variableMap(View::View2D);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    m_scene.initializeDatasets();
    updateBoundingBox();
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    m_scene.initializeDatasets();
    updateBoundingBox();
    m_renderDispatcher->setRedrawRequired();
}

void SceneController2DImpl::render() {
    m_renderDispatcher->render(m_scene.nodes(), m_mvp, m_parameters.axis(), m_parameters.sliderParameter());
}

void SceneController2DImpl::updateBoundingBox() {
    m_boundingBox = m_scene.boundingBox(View::View2D);
    m_mvp = MVP2D(m_screenInfo, m_boundingBox, m_parameters);
}