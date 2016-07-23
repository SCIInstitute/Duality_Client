#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "duality/InputVariable.h"
#include "duality/SliderParameter.h"

#include <memory>

class SceneController2DImpl;

class SceneController2D {
public:
    SceneController2D(std::unique_ptr<SceneController2DImpl> impl);
    ~SceneController2D();

    void updateScreenInfo(const ScreenInfo& screenInfo);
    void updateDatasets();
    void setRedrawRequired();
    void render();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const float rotationAngle);
    void addZoom(const float zoom);
    
    bool supportsSlices() const;
    int numSlicesForCurrentAxis() const;
    void setSlice(int slice);
    std::pair<float, float> boundsForCurrentAxis() const;
    void setDepth(float depth);
    SliderParameter sliderParameter() const;

    void toggleAxis();
    enum class AxisLabelMode { Mathematical, Anatomical };
    std::string labelForCurrentAxis(AxisLabelMode mode) const;
    
    VariableMap variableMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    std::unique_ptr<SceneController2DImpl> m_impl;
};