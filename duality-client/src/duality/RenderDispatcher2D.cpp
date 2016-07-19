#include "src/duality/RenderDispatcher2D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/GeometryRenderer2D.h"
#include "src/duality/VolumeNode.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher2D::RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo, std::shared_ptr<Settings> settings)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer2D>())
    , m_volRenderer(std::make_unique<VolumeRenderer2D>())
    , m_settings(settings)
    , m_sliderParameter(SliderParameter(0, 0.0f))
    , m_redraw(true) {}

RenderDispatcher2D::~RenderDispatcher2D() = default;

void RenderDispatcher2D::render(const std::vector<std::unique_ptr<SceneNode>>& nodes, const MVP2D& mvp, CoordinateAxis axis,
                                const SliderParameter& sliderParameter) {
    if (!m_redraw) {
        return;
    }

    m_mvp = &mvp;
    m_axis = axis;
    m_sliderParameter = sliderParameter;

    startDraw();
    for (const auto& node : nodes) {
        if (node->isVisibleInView(View::View2D)) {
            node->render(*this);
        }
    }
    finishDraw();
}

void RenderDispatcher2D::dispatch(GeometryNode& node) {
    float depth = m_sliderParameter.depth();
    m_geoRenderer->render(node.dataset(), m_mvp->mvp(), m_axis, depth);
}

void RenderDispatcher2D::dispatch(VolumeNode& node) {
    int slice;
    if (m_sliderParameter.hasSlice()) {
        slice = m_sliderParameter.slice();
    } else {
        BoundingBox bb = node.boundingBox();
        int numSlices = static_cast<int>(node.dataset().sliceInfos()[m_axis].size());
        float range = std::abs(bb.max[m_axis] - bb.min[m_axis]);
        slice = std::min<int>((m_sliderParameter.depth() - bb.min[m_axis]) / range * numSlices, numSlices - 1);
        if (slice < 0 || slice >= numSlices) {
            return;
        }
    }
    m_volRenderer->render(node.dataset(), m_mvp->mvp(), node.transferFunction(), m_axis, slice);
}

void RenderDispatcher2D::startDraw() {
    std::array<float, 3> backgroundColor = m_settings->backgroundColor();
    GL(glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher2D::finishDraw() {
    m_fbo->Read(0);
    m_redraw = false;
}

void RenderDispatcher2D::setRedrawRequired() {
    m_redraw = true;
}