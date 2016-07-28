#pragma once

#include "duality/Settings.h"
#include "src/IVDA/GLFrameBufferObject.h"
#include "src/duality/MVP3D.h"
#include "src/duality/RenderParameters3D.h"

#include "duality/InputVariable.h"

#include <memory>

class Scene;
class RenderDispatcher3D;

class SceneController3DImpl {
public:
    SceneController3DImpl(Scene& scene, const RenderParameters3D& initialParameters,
                          std::function<void(int, int, const std::string&)> updateDatasetCallback, std::shared_ptr<GLFrameBufferObject> fbo,
                          std::shared_ptr<Settings> settings);
    ~SceneController3DImpl();

    void updateScreenInfo(const ScreenInfo& screenInfo);
    
    void setNodeUpdateEnabled(const std::string& name, bool enabled);
    void updateDatasets();
    void initializeDatasets();

    void setRedrawRequired();
    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    void setZoom(const float zoom);

    VariableMap variableMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    Scene& m_scene;
    RenderParameters3D m_parameters;
    std::shared_ptr<Settings> m_settings;
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<RenderDispatcher3D> m_renderDispatcher;
    ScreenInfo m_screenInfo;
    BoundingBox m_boundingBox;
    MVP3D m_mvp;
};