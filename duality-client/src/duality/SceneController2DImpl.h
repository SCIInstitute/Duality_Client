#pragma once

#include "duality/InputVariable.h"
#include "duality/SceneController2D.h"
#include "duality/Settings.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/duality/MVP2D.h"
#include "src/duality/SliderParameterCalculator.h"

#include <memory>

class Scene;
class RenderDispatcher2D;
class RenderParameters2D;

class SceneController2DImpl {
public:
    SceneController2DImpl(Scene& scene, const RenderParameters2D& initialParameters, std::shared_ptr<GLFrameBufferObject> fbo,
                          std::shared_ptr<Settings> settings);
    ~SceneController2DImpl();

    void updateScreenInfo(const ScreenInfo& screenInfo);

    void updateDatasets();
    void initializeDatasets();
    void setUpdateDatasetCallback(std::function<void(int,int,const std::string&)> callback);
    
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
    std::string labelForCurrentAxis(SceneController2D::AxisLabelMode mode) const;

    VariableMap variableMap() const;
    void setVariable(const std::string& objectName, const std::string& variableName, float value);
    void setVariable(const std::string& objectName, const std::string& variableName, const std::string& value);

private:
    void updateBoundingBox();
    
private:
    Scene& m_scene;
    RenderParameters2D m_parameters;
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::shared_ptr<Settings> m_settings;
    std::unique_ptr<RenderDispatcher2D> m_renderDispatcher;
    std::unique_ptr<SliderParameterCalculator> m_sliderCalculator;
    ScreenInfo m_screenInfo;
    BoundingBox m_boundingBox;
    MVP2D m_mvp;
};