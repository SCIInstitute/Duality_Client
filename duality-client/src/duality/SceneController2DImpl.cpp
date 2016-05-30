#include "src/duality/SceneController2DImpl.h"

#include "src/duality/RenderDispatcher2D.h"
#include "src/duality/Scene.h"

SceneController2DImpl::SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters,
                                             std::unique_ptr<RenderDispatcher2D> renderDispatcher, std::unique_ptr<MVP2D> mvp)
    : m_scene(scene)
    , m_parameters(initialParameters)
    , m_renderDispatcher(std::move(renderDispatcher))
    , m_mvp(std::move(mvp)) {}

void SceneController2DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_mvp->updateScreenInfo(screenInfo);
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

VariableInfoMap SceneController2DImpl::variableInfoMap() const {
    return m_scene.variableInfoMap();
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController2DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boudningBox = duality::calculateSceneBoundingBox(m_scene);
    m_mvp->updateBoundingBox(boudningBox);
}

void SceneController2DImpl::render() {
    m_renderDispatcher->setMVP(m_mvp->calculate(m_parameters));
    m_renderDispatcher->setAxis(m_parameters.axis());
    m_renderDispatcher->setSlice(m_parameters.slice());
    m_scene.dispatch(*m_renderDispatcher);
}
