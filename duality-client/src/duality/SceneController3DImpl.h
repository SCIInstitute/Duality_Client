#pragma once

#include "duality/VariableInfo.h"

#include "src/duality/MVP3D.h"
#include "src/duality/RenderParameters3D.h"
#include "src/IVDA/GLFrameBufferObject.h"

#include <memory>

class Scene;
class RenderDispatcher3D;

class SceneController3DImpl {
public:
    SceneController3DImpl(Scene& scene, const RenderParameters3D& initialParameters, std::shared_ptr<GLFrameBufferObject> fbo);
    ~SceneController3DImpl();

    void updateScreenInfo(const ScreenInfo& screenInfo);

    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    void setZoom(const float zoom);

    VariableInfoMap variableInfoMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    Scene& m_scene;
    RenderParameters3D m_parameters;
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<RenderDispatcher3D> m_renderDispatcher;
};