#include "duality/SceneController3D.h"

#include "src/duality/SceneController3DImpl.h"

SceneController3D::SceneController3D(std::unique_ptr<SceneController3DImpl> impl)
    : m_impl(std::move(impl)) {}

SceneController3D::~SceneController3D() = default;

void SceneController3D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_impl->updateScreenInfo(screenInfo);
}

void SceneController3D::initializeDatasets() {
    m_impl->initializeDatasets();
}

void SceneController3D::setNodeUpdateEnabled(const std::string &name, bool enabled) {
    m_impl->setNodeUpdateEnabled(name, enabled);
}

void SceneController3D::updateDatasets() {
    m_impl->updateDatasets();
}

void SceneController3D::setRedrawRequired() {
    m_impl->setRedrawRequired();
}

void SceneController3D::render() {
    m_impl->render();
}

void SceneController3D::addTranslation(const IVDA::Vec2f& translation) {
    m_impl->addTranslation(translation);
}

void SceneController3D::addRotation(const IVDA::Mat4f& rotation) {
    m_impl->addRotation(rotation);
}

void SceneController3D::addZoom(const float zoom) {
    m_impl->setZoom(zoom);
}

VariableMap SceneController3D::variableMap() const {
    return m_impl->variableMap();
}

void SceneController3D::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_impl->setVariable(objectName, variableName, value);
}

void SceneController3D::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_impl->setVariable(objectName, variableName, value);
}
