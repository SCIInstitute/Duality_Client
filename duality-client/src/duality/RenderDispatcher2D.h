#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "duality/Settings.h"
#include "src/duality/GeometryRenderer2D.h"
#include "src/duality/MVP2D.h"
#include "src/duality/SliderParameter.h"
#include "src/duality/VolumeRenderer2D.h"

class GLFrameBufferObject;
class GeometryRenderer2D;
class GeometryNode;
class VolumeNode;
class SceneNode;

class RenderDispatcher2D {
public:
    RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo, std::shared_ptr<Settings> settings);
    ~RenderDispatcher2D();

    void render(const std::vector<std::unique_ptr<SceneNode>>& nodes, const MVP2D& mvp, CoordinateAxis axis,
                const SliderParameter& sliderParameter);
    void setRedrawRequired();

    void dispatch(GeometryNode& node);
    void dispatch(VolumeNode& node);

private:
    void startDraw();
    void finishDraw();

private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer2D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer2D> m_volRenderer;
    std::shared_ptr<Settings> m_settings;
    const MVP2D* m_mvp;
    CoordinateAxis m_axis;
    SliderParameter m_sliderParameter;
    bool m_redraw;
};