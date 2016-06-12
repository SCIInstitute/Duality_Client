#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/DatasetDispatcher.h"
#include "src/duality/MVP2D.h"
#include "src/duality/GeometryRenderer2D.h"
#include "src/duality/VolumeRenderer2D.h"

class GLFrameBufferObject;
class GeometryRenderer2D;

class RenderDispatcher2D : public DatasetDispatcher {
public:
    RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo);
    ~RenderDispatcher2D();

    void dispatch(GeometryDataset& dataset) override;
    void dispatch(VolumeDataset& dataset) override;
    void startDraw();
    void finishDraw();

    void setMVP(const GLMatrix& mvp);
    void setAxis(CoordinateAxis axis);
    void setSlice(const float slice);

private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer2D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer2D> m_volRenderer;
    GLMatrix m_mvp;
    CoordinateAxis m_axis;
    float m_slice;
};