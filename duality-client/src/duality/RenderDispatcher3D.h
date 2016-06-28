#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/BoundingBox.h"
#include "src/duality/NodeDispatcher.h"
#include "src/duality/MVP3D.h"

class GLFrameBufferObject;
class GeometryRenderer3D;
class VolumeRenderer3D;

class RenderDispatcher3D : public NodeDispatcher {
public:
    RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters3D& initialParameters);
    ~RenderDispatcher3D();

    enum RenderPass { FirstPass = 0, OpaqueGeometryPass = 0, TransparentGeometryPass = 1, VolumePass = 2, LastPass = 3 };
    void nextPass();
    RenderPass renderPass() const;
    
    void dispatch(GeometryNode& node) override;
    void dispatch(VolumeNode& node) override;
    void startDraw();
    void finishDraw();

    void updateScreenInfo(const ScreenInfo& screenInfo);
    void updateBoundingBox(const BoundingBox& boundingBox);
    void setRedrawRequired();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    void addZoom(const float zoom);
    
private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    RenderPass m_pass;
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer3D> m_volumeRenderer;
    RenderParameters3D m_parameters;
    ScreenInfo m_screenInfo;
    BoundingBox m_boundingBox;
    MVP3D m_mvp;
    bool m_redraw;
};