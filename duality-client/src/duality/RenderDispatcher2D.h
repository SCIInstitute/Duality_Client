#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/NodeDispatcher.h"
#include "src/duality/MVP2D.h"
#include "src/duality/GeometryRenderer2D.h"
#include "src/duality/VolumeRenderer2D.h"

class GLFrameBufferObject;
class GeometryRenderer2D;

class RenderDispatcher2D : public NodeDispatcher {
public:
    RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters2D& initialParameters);
    ~RenderDispatcher2D();

    void dispatch(GeometryNode& node) override;
    void dispatch(VolumeNode& node) override;
    void startDraw();
    void finishDraw();

    void updateScreenInfo(const ScreenInfo& screenInfo);
    void updateBoundingBox(const BoundingBox& boundingBox);
    void setRedrawRequired();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const float rotationAngle);
    void addZoom(const float zoom);
    void setSlice(float slice);
    void toggleAxis();
    
    float slice() const;
    CoordinateAxis currentAxis() const;
    
private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer2D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer2D> m_volRenderer;
    RenderParameters2D m_parameters;
    ScreenInfo m_screenInfo;
    BoundingBox m_boundingBox;
    MVP2D m_mvp;
    bool m_redraw;
};