#pragma once

#include "duality/VariableInfo.h"

#include "src/duality/MVP2D.h"
#include "src/duality/RenderParameters2D.h"

#include <memory>

class Scene;
class RenderDispatcher2D;

class SceneController2DImpl {
public:
    SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters, std::unique_ptr<RenderDispatcher2D> renderDispatcher,
        std::unique_ptr<MVP2D> mvp);

    void updateScreenInfo(const ScreenInfo& screenInfo);

    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const float rotationAngle);

    VariableInfoMap variableInfoMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    Scene& m_scene;
    RenderParameters2D m_parameters;
    std::unique_ptr<MVP2D> m_mvp;
    std::unique_ptr<RenderDispatcher2D> m_renderDispatcher;
};