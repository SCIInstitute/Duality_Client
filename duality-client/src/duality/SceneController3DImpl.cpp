#include "src/duality/SceneController3DImpl.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/Scene.h"

SceneController3DImpl::SceneController3DImpl(Scene& scene, const RenderParameters3D& initialParameters,
                                             std::unique_ptr<RenderDispatcher3D> renderDispatcher, std::unique_ptr<MVP3D> mvp)
    : m_scene(scene)
    , m_parameters(initialParameters)
    , m_renderDispatcher(std::move(renderDispatcher))
    , m_mvp(std::move(mvp)) {}

void SceneController3DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_mvp->updateScreenInfo(screenInfo);
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
    return m_scene.variableInfoMap();
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController3DImpl::render() {
    m_renderDispatcher->setMVP(m_mvp->calculate(m_parameters));
    m_scene.dispatch(*m_renderDispatcher);
}
