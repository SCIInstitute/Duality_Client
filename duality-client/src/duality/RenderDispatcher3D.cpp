#include "src/duality/RenderDispatcher3D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/GeometryRenderer3D.h"
#include "src/duality/InterleavingRenderer3D.h"
#include "src/duality/SceneNode.h"
#include "src/duality/View.h"
#include "src/duality/VolumeNode.h"
#include "src/duality/VolumeRenderer3D.h"

#include <OpenGLES/ES3/gl.h>

#include <algorithm>

RenderDispatcher3D::RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters3D& initialParameters,
                                       std::shared_ptr<Settings> settings)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer3D>())
    , m_volumeRenderer(std::make_unique<VolumeRenderer3D>())
    , m_interleavingRenderer(std::make_unique<InterleavingRenderer3D>())
    , m_parameters(initialParameters)
    , m_settings(settings)
    , m_screenInfo()
    , m_boundingBox()
    , m_mvp()
    , m_redraw(true) {}

RenderDispatcher3D::~RenderDispatcher3D() = default;

std::vector<Renderable> RenderDispatcher3D::sortRenderables(const std::vector<Renderable>& renderables) {
    std::vector<Renderable> result = renderables;
    auto sorter = [&](const Renderable& lhs, const Renderable& rhs) {
        IVDA::Vec3f eyePos = m_mvp.eyePos();
        auto lhsBB = lhs.boundingBox();
        auto lhsCenterEye = ((lhsBB.min + (lhsBB.max - lhsBB.min) / 2) - eyePos);
        auto rhsBB = rhs.boundingBox();
        auto rhsCenterEye = ((rhsBB.min + (rhsBB.max - rhsBB.min) / 2) - eyePos);
        return lhsCenterEye.sqLength() > rhsCenterEye.sqLength();
    };
    std::sort(begin(result), end(result), sorter);
    return result;
}

std::vector<Renderable> RenderDispatcher3D::calculateRenderables(const std::vector<std::unique_ptr<SceneNode>>& nodes) {
    // separate volume nodes and geometry nodes
    std::vector<VolumeNode*> volumeNodes;
    std::vector<GeometryNode*> geometryNodes;
    for (const auto& node : nodes) {
        VolumeNode* volumeNode = dynamic_cast<VolumeNode*>(node.get());
        if (volumeNode != nullptr && volumeNode->isVisibleInView(View::View3D)) {
            volumeNodes.push_back(volumeNode);
        }
        GeometryNode* geometryNode = dynamic_cast<GeometryNode*>(node.get());
        if (geometryNode != nullptr && geometryNode->isVisibleInView(View::View3D)) {
            geometryNodes.push_back(geometryNode);
        }
    }

    // group intersecting volume nodes and geometry nodes into IntersectingNodes
    std::vector<Renderable> result;
    auto volIt = begin(volumeNodes);
    while (volIt != end(volumeNodes)) {
        IntersectingNode node;
        VolumeNode* volumeNode = *volIt;
        node.volumeNode = volumeNode;
        auto it = std::partition(begin(geometryNodes), end(geometryNodes),
                                 [&](const GeometryNode* n) { return !n->intersects(volumeNode->boundingBox()); });
        if (it != end(geometryNodes)) {
            std::copy(it, end(geometryNodes), std::back_inserter(node.geometryNodes));
            geometryNodes.erase(it, end(geometryNodes));
            volIt = volumeNodes.erase(volIt);
            result.push_back(node);
        } else {
            ++volIt;
        }
    }

    // insert remaining geometry and volume nodes
    std::copy(begin(geometryNodes), end(geometryNodes), std::back_inserter(result));
    std::copy(begin(volumeNodes), end(volumeNodes), std::back_inserter(result));

    return result;
}

void RenderDispatcher3D::render(const std::vector<std::unique_ptr<SceneNode>>& nodes) {
    if (!m_redraw) {
        return;
    }
    auto renderables = calculateRenderables(nodes); // FIXME: cache renderables
    auto sortedRenderables = sortRenderables(renderables);
    startDraw();
    for (auto& r : sortedRenderables) {
        r.render(*this);
    }
    finishDraw();
}

void RenderDispatcher3D::dispatch(GeometryNode& node) {
    m_geoRenderer->renderOpaque(node.dataset(), m_mvp);
    if (node.isTransparent()) {
        m_geoRenderer->renderTransparent(node.dataset(), m_mvp);
    }
}

void RenderDispatcher3D::dispatch(VolumeNode& node) {
    m_volumeRenderer->render(node.dataset(), m_mvp, node.transferFunction());
}

void RenderDispatcher3D::dispatch(IntersectingNode& node) {
    std::vector<const GeometryDataset*> geoDatasets;
    for (auto geoNode : node.geometryNodes) {
        geoDatasets.push_back(&geoNode->dataset());
    }
    m_interleavingRenderer->render(node.volumeNode->dataset(), geoDatasets, m_mvp, node.volumeNode->transferFunction());
}

void RenderDispatcher3D::startDraw() {
    std::array<float, 3> backgroundColor = m_settings->backgroundColor();
    GL(glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher3D::finishDraw() {
    if (m_redraw) {
        m_fbo->Read(0);
        m_redraw = false;
    }
}

void RenderDispatcher3D::addTranslation(const IVDA::Vec2f& translation) {
    m_parameters.addTranslation(translation);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher3D::addRotation(const IVDA::Mat4f& rotation) {
    m_parameters.addRotation(rotation);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher3D::addZoom(const float zoom) {
    m_parameters.addZoom(zoom);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher3D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    m_screenInfo = screenInfo;
    m_mvp = MVP3D(m_screenInfo, m_boundingBox, m_parameters);
    m_redraw = true;
}

void RenderDispatcher3D::updateBoundingBox(const BoundingBox& boundingBox) {
    m_boundingBox = boundingBox;
    m_mvp = MVP3D(m_screenInfo, m_boundingBox, m_parameters);
    m_redraw = true;
}

void RenderDispatcher3D::setRedrawRequired() {
    m_redraw = true;
}

void RenderDispatcher3D::IntersectingNode::render(RenderDispatcher3D& dispatcher) {
    dispatcher.dispatch(*this);
}

BoundingBox RenderDispatcher3D::IntersectingNode::boundingBox() const {
    BoundingBox bb = duality::boundingBox(geometryNodes);
    bb.min.StoreMin(volumeNode->boundingBox().min);
    bb.max.StoreMax(volumeNode->boundingBox().max);
    return bb;
}