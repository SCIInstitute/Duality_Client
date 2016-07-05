#include "src/duality/RenderDispatcher3D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/GeometryRenderer3D.h"
#include "src/duality/VolumeNode.h"
#include "src/duality/VolumeRenderer3D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher3D::RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters3D& initialParameters)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer3D>())
    , m_volumeRenderer(std::make_unique<VolumeRenderer3D>())
    , m_parameters(initialParameters)
    , m_screenInfo()
    , m_boundingBox()
    , m_mvp()
    , m_redraw(true) {}

RenderDispatcher3D::~RenderDispatcher3D() = default;

void RenderDispatcher3D::nextPass() {
    m_pass = static_cast<RenderPass>(m_pass + 1);
}

RenderDispatcher3D::RenderPass RenderDispatcher3D::renderPass() const {
    return m_pass;
}

std::vector<SceneNode*> RenderDispatcher3D::sortNodes(const std::vector<std::unique_ptr<SceneNode>>& nodes) {
    std::vector<SceneNode*> sortedNodes;
    for (const auto& node : nodes) {
        sortedNodes.push_back(node.get());
    }
    
    auto sorter = [&](const SceneNode* lhs, const SceneNode* rhs) {
        auto lhsBB = lhs->boundingBox();
        auto lhsCenterEye = (((lhsBB.max - lhsBB.min) / 2) - m_mvp.eyePos());
        auto rhsBB = rhs->boundingBox();
        auto rhsCenterEye = (((rhsBB.max - rhsBB.min) / 2) - m_mvp.eyePos());
        return lhsCenterEye.sqLength() > rhsCenterEye.sqLength();
    };
    std::sort(begin(sortedNodes), end(sortedNodes), sorter);
    return sortedNodes;
}

void RenderDispatcher3D::dispatch(GeometryNode& node) {
    if (m_pass == RenderPass::OpaqueGeometryPass && m_redraw) {
        m_geoRenderer->renderOpaque(node.dataset(), m_mvp);
    }
    else if (m_pass == RenderPass::TransparentGeometryPass && m_redraw) {
        m_geoRenderer->renderTransparent(node.dataset(), m_mvp);
    }
}

void RenderDispatcher3D::dispatch(VolumeNode& node) {
    if (m_pass == RenderPass::VolumePass && m_redraw) {
       m_volumeRenderer->render(node.dataset(), m_mvp, node.transferFunction());
    }
}

void RenderDispatcher3D::startDraw() {
    if (m_redraw) {
        m_pass = RenderPass::FirstPass;
        GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
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