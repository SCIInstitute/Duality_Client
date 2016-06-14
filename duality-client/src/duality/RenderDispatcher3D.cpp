#include "src/duality/RenderDispatcher3D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer3D.h"
#include "src/duality/VolumeDataset.h"
#include "src/duality/VolumeRenderer3D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher3D::RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer3D>())
    , m_volumeRenderer(std::make_unique<VolumeRenderer3D>())
    , m_redraw(true) {}

RenderDispatcher3D::~RenderDispatcher3D() = default;

void RenderDispatcher3D::dispatch(GeometryDataset& dataset) {
    if (m_redraw) {
        m_geoRenderer->render(dataset, m_mvp);
    }
}

void RenderDispatcher3D::dispatch(VolumeDataset& dataset) {
    if (m_redraw) {
        m_volumeRenderer->render(dataset, m_mvp);
    }
}

void RenderDispatcher3D::startDraw() {
    if (m_redraw) {
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

void RenderDispatcher3D::updateParameters(const RenderParameters3D& parameters) {
    if (m_currentParams != parameters) {
        m_mvp.updateParameters(parameters);
        m_redraw = true;
        m_currentParams = parameters;
    }
}

void RenderDispatcher3D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_mvp.updateScreenInfo(screenInfo);
    m_redraw = true;
}

void RenderDispatcher3D::updateBoundingBox(const BoundingBox& boundingBox) {
    m_mvp.updateBoundingBox(boundingBox);
    m_redraw = true;
}