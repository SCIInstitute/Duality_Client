#pragma once

#include "duality/VariableInfo.h"
#include "duality/SceneController2D.h"

#include "src/duality/MVP2D.h"
#include "src/duality/RenderParameters2D.h"
#include "src/IVDA/GLFrameBufferObject.h"

#include <memory>

class Scene;
class RenderDispatcher2D;

class SceneController2DImpl {
public:
    SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters, std::shared_ptr<GLFrameBufferObject> fbo);
    ~SceneController2DImpl();

    void updateScreenInfo(const ScreenInfo& screenInfo);

    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const float rotationAngle);
    void addZoom(const float zoom);

    std::pair<float, float> minMaxForCurrentAxis() const;
    void setSlice(float slice);
    float slice() const;
    
    void toggleAxis();
    std::string labelForCurrentAxis(SceneController2D::AxisLabelMode mode) const;
    
    VariableInfoMap variableInfoMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    Scene& m_scene;
    RenderParameters2D m_parameters;
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<MVP2D> m_mvp;
    std::unique_ptr<RenderDispatcher2D> m_renderDispatcher;
};