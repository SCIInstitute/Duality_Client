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

std::pair<float, float> SceneController2D::minMaxForCurrentAxis() const {
    return m_impl->minMaxForCurrentAxis();
}

void SceneController2D::setSlice(float slice) {
    m_impl->setSlice(slice);
}

float SceneController2D::slice() const {
    return m_impl->slice();
}

void SceneController2D::toggleAxis() {
    m_impl->toggleAxis();
}

std::string SceneController2D::labelForCurrentAxis(AxisLabelMode mode) const {
    return m_impl->labelForCurrentAxis(mode);
}

VariableInfoMap SceneController2D::variableInfoMap() const {
    return m_impl->variableInfoMap();
}

void SceneController2D::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_impl->setVariable(objectName, variableName, value);
}

void SceneController2D::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_impl->setVariable(objectName, variableName, value);
}
