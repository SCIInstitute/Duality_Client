#include "src/duality/SceneController3DImpl.h"

#include "src/duality/RenderDispatcher3D.h"
#include "src/duality/Scene.h"

SceneController3DImpl::SceneController3DImpl(Scene& scene, const RenderParameters3D& initialParameters,
                                             std::shared_ptr<GLFrameBufferObject> fbo, std::shared_ptr<Settings> settings)
    : m_scene(scene)
    , m_parameters(initialParameters)
    , m_fbo(fbo)
    , m_settings(settings)
    , m_renderDispatcher(std::make_unique<RenderDispatcher3D>(fbo, settings)) {
    m_scene.updateDatasets();
}

SceneController3DImpl::~SceneController3DImpl() = default;

void SceneController3DImpl::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    m_screenInfo = screenInfo;
    m_boundingBox = m_scene.boundingBox(View::View3D);
    m_mvp = MVP3D(m_screenInfo, m_boundingBox, m_parameters);
}

void SceneController3DImpl::setRedrawRequired() {
    m_renderDispatcher->setRedrawRequired();
}

void SceneController3DImpl::addTranslation(const IVDA::Vec2f& translation) {
    m_parameters.addTranslation(translation);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController3DImpl::addRotation(const IVDA::Mat4f& rotation) {
    m_parameters.addRotation(rotation);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController3DImpl::setZoom(const float zoom) {
    m_parameters.addZoom(zoom);
    m_mvp.updateParameters(m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

VariableMap SceneController3DImpl::variableMap() const {
    return m_scene.variableMap(View::View3D);
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, float value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    m_boundingBox = m_scene.boundingBox(View::View3D);
    m_mvp = MVP3D(m_screenInfo, m_boundingBox, m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController3DImpl::setVariable(const std::string& objectName, const std::string& variableName, const std::string& value) {
    m_scene.setVariable(objectName, variableName, value);
    m_scene.updateDatasets();
    m_boundingBox = m_scene.boundingBox(View::View3D);
    m_mvp = MVP3D(m_screenInfo, m_boundingBox, m_parameters);
    m_renderDispatcher->setRedrawRequired();
}

void SceneController3DImpl::render() {
    m_renderDispatcher->render(m_scene.nodes(), m_mvp);
}
