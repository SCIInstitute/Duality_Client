#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "duality/VariableInfo.h"

#include <memory>

class SceneController2DImpl;

class SceneController2D {
public:
    SceneController2D(std::unique_ptr<SceneController2DImpl> impl);
    ~SceneController2D();

    void updateScreenInfo(const ScreenInfo& screenInfo);
    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const float rotationAngle);
    void addZoom(const float zoom);
    
    std::pair<float, float> minMaxForCurrentAxis() const;
    void setSlice(float slice);
    float slice() const;

    void toggleAxis();
    std::string labelForCurrentAxis() const;
    
    VariableInfoMap variableInfoMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    std::unique_ptr<SceneController2DImpl> m_impl;
};