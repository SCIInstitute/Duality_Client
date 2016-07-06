#include "src/duality/RenderDispatcher2D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/VolumeNode.h"
#include "src/duality/GeometryRenderer2D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher2D::RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters2D& initialParameters)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer2D>())
    , m_volRenderer(std::make_unique<VolumeRenderer2D>())
    , m_parameters(initialParameters)
    , m_screenInfo()
    , m_boundingBox()
    , m_mvp()
    , m_redraw(true) {}

RenderDispatcher2D::~RenderDispatcher2D() = default;

void RenderDispatcher2D::render(const std::vector<std::unique_ptr<SceneNode>>& nodes) {
    if (!m_redraw) {
        return;
    }
    
    startDraw();
    for (const auto& node : nodes) {
        if (node->isVisibleInView(View::View2D)) {
            node->render(*this);
        }
    }
    finishDraw();
}

void RenderDispatcher2D::dispatch(GeometryNode& node) {
    if (m_redraw) {
        m_geoRenderer->render(node.dataset(), m_mvp.mvp(), m_parameters.axis(), m_parameters.slice());
    }
}

void RenderDispatcher2D::dispatch(VolumeNode& node) {
    if (m_redraw) {
        m_volRenderer->render(node.dataset(), m_mvp.mvp(), node.transferFunction(), m_parameters.axis(), m_parameters.slice());
    }
}

bool RenderDispatcher2D::startDraw() {
    if (m_redraw) {
        GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
    return m_redraw;
}

void RenderDispatcher2D::finishDraw() {
    if (m_redraw) {
        m_fbo->Read(0);
        m_redraw = false;
    }
}
void RenderDispatcher2D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    m_screenInfo = screenInfo;
    m_mvp = MVP2D(m_screenInfo, m_boundingBox, m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::updateBoundingBox(const BoundingBox& boundingBox) {
    m_boundingBox = boundingBox;
    if (m_parameters == RenderParameters2D()) {
         m_parameters.setSlice((boundingBox.min[m_parameters.axis()] + boundingBox.max[m_parameters.axis()]) / 2);
    }
    m_mvp = MVP2D(m_screenInfo, m_boundingBox, m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::setRedrawRequired() {
    m_redraw = true;
}

void RenderDispatcher2D::addTranslation(const IVDA::Vec2f& translation) {
    m_parameters.addTranslation(translation);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::addRotation(const float rotationAngle) {
    m_parameters.addRotation(rotationAngle);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::addZoom(const float zoom) {
    m_parameters.addZoom(zoom);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::setSlice(float slice) {
    m_parameters.setSlice(slice);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::toggleAxis() {
    m_parameters.toggleAxis();
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

float RenderDispatcher2D::slice() const {
    return m_parameters.slice();
}

CoordinateAxis RenderDispatcher2D::currentAxis() const {
    return m_parameters.axis();
}
