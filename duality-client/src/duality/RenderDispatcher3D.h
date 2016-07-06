#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/BoundingBox.h"
#include "src/duality/MVP3D.h"
#include "src/duality/RenderableConcept.h"

class GLFrameBufferObject;
class GeometryRenderer3D;
class VolumeRenderer3D;
class GeometryNode;
class VolumeNode;
class SceneNode;

class RenderDispatcher3D {
public:
    RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters3D& initialParameters);
    ~RenderDispatcher3D();

    std::vector<Renderable> createRenderables(const std::vector<std::unique_ptr<SceneNode>>& nodes);

    void dispatch(GeometryNode& node);
    void dispatch(VolumeNode& node);
    bool startDraw();
    void finishDraw();

    void updateScreenInfo(const ScreenInfo& screenInfo);
    void updateBoundingBox(const BoundingBox& boundingBox);
    void setRedrawRequired();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);
    void addZoom(const float zoom);

private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer3D> m_volumeRenderer;
    RenderParameters3D m_parameters;
    ScreenInfo m_screenInfo;
    BoundingBox m_boundingBox;
    MVP3D m_mvp;
    bool m_redraw;
};