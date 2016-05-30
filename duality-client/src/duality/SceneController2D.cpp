#include "duality/SceneController2D.h"

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
    m_impl->setZoom(zoom);
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
