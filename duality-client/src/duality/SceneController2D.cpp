#include "duality/SceneController2D.h"

#include "src/duality/SceneController2DImpl.h"
#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/BoundingBox.h"

using namespace IVDA;

SceneController2D::SceneController2D(std::unique_ptr<SceneController2DImpl> impl)
: m_impl(std::move(impl)) {}

SceneController2D::~SceneController2D() = default;

void SceneController2D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_impl->updateScreenInfo(screenInfo);
}

void SceneController2D::updateDatasets() {
    m_impl->updateDatasets();
}

void SceneController2D::initializeDatasets() {
    m_impl->initializeDatasets();
}

void SceneController2D::setUpdateDatasetCallback(std::function<void(int,int,const std::string&)> callback) {
    m_impl->setUpdateDatasetCallback(callback);
}

void SceneController2D::setRedrawRequired() {
    m_impl->setRedrawRequired();
}

void SceneController2D::render() {
    m_impl->render();
}

void SceneController2D::addTranslation(const IVDA::Vec2f& translation) {
    m_impl->addTranslation(translation);
}

void SceneController2D::addRotation(const float rotationAngle) {
    m_impl->addRotation(rotationAngle);
}

void SceneController2D::addZoom(const float zoom) {
    m_impl->addZoom(zoom);
}

bool SceneController2D::supportsSlices() const {
    return m_impl->supportsSlices();
}

int SceneController2D::numSlicesForCurrentAxis() const {
    return m_impl->numSlicesForCurrentAxis();
}

void SceneController2D::setSlice(int slice) {
    m_impl->setSlice(slice);
}

std::pair<float, float> SceneController2D::boundsForCurrentAxis() const {
    return m_impl->boundsForCurrentAxis();
}

void SceneController2D::setDepth(float depth) {
    m_impl->setDepth(depth);
}

SliderParameter SceneController2D::sliderParameter() const {
    return m_impl->sliderParameter();
}

void SceneController2D::toggleAxis() {
    m_impl->toggleAxis();
}

std::string SceneController2D::labelForCurrentAxis(AxisLabelMode mode) const {
    return m_impl->labelForCurrentAxis(mode);
}

VariableMap SceneController2D::variableMap() const {
    return m_impl->variableMap();
}

void SceneController2D::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_impl->setVariable(objectName, variableName, value);
}

void SceneController2D::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_impl->setVariable(objectName, variableName, value);
}
