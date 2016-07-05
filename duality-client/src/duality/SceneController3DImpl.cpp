#include "src/duality/SceneController3DImpl.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/Scene.h"

SceneController3DImpl::SceneController3DImpl(Scene& scene, const RenderParameters3D& initialParameters,
                                             std::shared_ptr<GLFrameBufferObject> fbo)
    : m_scene(scene)
    , m_renderDispatcher(std::make_unique<RenderDispatcher3D>(fbo, initialParameters)) {
    m_scene.updateDatasets();
}

SceneController3DImpl::~SceneController3DImpl() = default;

void SceneController3DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_renderDispatcher->updateScreenInfo(screenInfo);
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View3D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController3DImpl::setRedrawRequired() {
    m_renderDispatcher->setRedrawRequired();
}

void SceneController3DImpl::addTranslation(const IVDA::Vec2f& translation) {
    m_renderDispatcher->addTranslation(translation);
}

void SceneController3DImpl::addRotation(const IVDA::Mat4f& rotation) {
    m_renderDispatcher->addRotation(rotation);
}

void SceneController3DImpl::setZoom(const float zoom) {
    m_renderDispatcher->addZoom(zoom);
}

VariableMap SceneController3DImpl::variableMap() const {
    return m_scene.variableMap(View::View3D);
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View3D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    BoundingBox boundingBox = duality::calculateSceneBoundingBox(m_scene, View::View3D);
    m_renderDispatcher->updateBoundingBox(boundingBox);
}

void SceneController3DImpl::render() {
    m_scene.render(*m_renderDispatcher);
}
