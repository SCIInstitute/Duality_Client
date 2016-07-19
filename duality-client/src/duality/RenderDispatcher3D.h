#pragma once

#include "IVDA/Vectors.h"
#include "duality/Settings.h"
#include "src/duality/RenderableConcept.h"

class GLFrameBufferObject;
class GeometryRenderer3D;
class VolumeRenderer3D;
class InterleavingRenderer3D;
class GeometryNode;
class VolumeNode;
class SceneNode;
class MVP3D;

class RenderDispatcher3D {
public:
    RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo, std::shared_ptr<Settings> settings);
    ~RenderDispatcher3D();

    void render(const std::vector<std::unique_ptr<SceneNode>>& nodes, const MVP3D& mvp);
    void setRedrawRequired();

    void dispatch(GeometryNode& node);
    void dispatch(VolumeNode& node);

private:
    struct IntersectingNode {
        VolumeNode* volumeNode;
        std::vector<GeometryNode*> geometryNodes;

        void render(RenderDispatcher3D& dispatcher);
        BoundingBox boundingBox() const;
    };

private:
    std::vector<Renderable> sortRenderables(const std::vector<Renderable>& renderables);
    std::vector<Renderable> calculateRenderables(const std::vector<std::unique_ptr<SceneNode>>& nodes);
    void startDraw();
    void finishDraw();
    void dispatch(IntersectingNode& node);

private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer3D> m_volumeRenderer;
    std::unique_ptr<InterleavingRenderer3D> m_interleavingRenderer;
    std::shared_ptr<Settings> m_settings;
    const MVP3D* m_mvp;
    bool m_redraw;
};