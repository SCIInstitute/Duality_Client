#include "src/duality/RenderDispatcher2D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer2D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher2D::RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer2D>())
    , m_volRenderer(std::make_unique<VolumeRenderer2D>())
    , m_redraw(true) {}

RenderDispatcher2D::~RenderDispatcher2D() = default;

void RenderDispatcher2D::dispatch(GeometryDataset& dataset) {
    if (m_redraw) {
        m_geoRenderer->render(dataset, m_mvp.mvp(), m_currentParams.axis(), m_currentParams.slice());
    }
}

void RenderDispatcher2D::dispatch(VolumeDataset& dataset) {
    if (m_redraw) {
        m_volRenderer->render(dataset, m_mvp.mvp(), m_currentParams.axis(), m_currentParams.slice());
    }
}

void RenderDispatcher2D::startDraw() {
    if (m_redraw) {
        GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}

void RenderDispatcher2D::finishDraw() {
    if (m_redraw) {
        m_fbo->Read(0);
        m_redraw = false;
    }
}

void RenderDispatcher2D::updateParameters(const RenderParameters2D& parameters) {
    if (m_currentParams != parameters) {
        m_mvp.updateParameters(parameters);
        m_redraw = true;
        m_currentParams = parameters;
    }
}

void RenderDispatcher2D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_mvp.updateScreenInfo(screenInfo);
    m_redraw = true;
}

void RenderDispatcher2D::updateBoundingBox(const BoundingBox& boundingBox) {
    m_mvp.updateBoundingBox(boundingBox);
    m_redraw = true;
}
