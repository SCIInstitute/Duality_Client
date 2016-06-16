#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "duality/VariableInfo.h"

#include <memory>

class SceneController3DImpl;

class SceneController3D {
public:
    SceneController3D(std::unique_ptr<SceneController3DImpl> impl);
    ~SceneController3D();
    
    void updateScreenInfo(const ScreenInfo& screenInfo);
    void setRedrawRequired();
    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    void addZoom(const float zoom);

    VariableInfoMap variableInfoMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    std::unique_ptr<SceneController3DImpl> m_impl;
};