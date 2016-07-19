#include "src/duality/RenderDispatcher2D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryNode.h"
#include "src/duality/GeometryRenderer2D.h"
#include "src/duality/VolumeNode.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher2D::RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo, const RenderParameters2D& initialParameters,
                                       std::shared_ptr<Settings> settings)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer2D>())
    , m_volRenderer(std::make_unique<VolumeRenderer2D>())
    , m_parameters(initialParameters)
    , m_settings(settings)
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
    float depth = m_parameters.sliderParameter().depth();
    m_geoRenderer->render(node.dataset(), m_mvp.mvp(), m_parameters.axis(), depth);
}

void RenderDispatcher2D::dispatch(VolumeNode& node) {
    auto sliderParameter = m_parameters.sliderParameter();
    CoordinateAxis axis = m_parameters.axis();
    int slice;
    if (sliderParameter.hasSlice()) {
        slice = sliderParameter.slice();
    } else {
        BoundingBox bb = node.boundingBox();
        int numSlices = static_cast<int>(node.dataset().sliceInfos()[axis].size());
        float range = std::abs(bb.max[axis] - bb.min[axis]);
        slice = std::min<int>((sliderParameter.depth() - bb.min[axis]) / range * numSlices, numSlices - 1);
        if (slice < 0 || slice >= numSlices) {
            return;
        }
    }
    m_volRenderer->render(node.dataset(), m_mvp.mvp(), node.transferFunction(), axis, slice);
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
void RenderDispatcher2D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_fbo->Resize(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor), true);
    m_screenInfo = screenInfo;
    m_mvp = MVP2D(m_screenInfo, m_boundingBox, m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::updateBoundingBox(const BoundingBox& boundingBox) {
    m_boundingBox = boundingBox;
    /*if (m_parameters == RenderParameters2D()) {
        m_parameters.setSlice((boundingBox.min[m_parameters.axis()] + boundingBox.max[m_parameters.axis()]) / 2);
    }*/ // FIXME
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

void RenderDispatcher2D::setSliderParameter(const SliderParameter& sliderParameter) {
    m_parameters.setSliderParameter(sliderParameter);
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

void RenderDispatcher2D::toggleAxis() {
    m_parameters.toggleAxis();
    m_mvp.updateParameters(m_parameters);
    m_redraw = true;
}

CoordinateAxis RenderDispatcher2D::currentAxis() const {
    return m_parameters.axis();
}
