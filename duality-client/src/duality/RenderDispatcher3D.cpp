#include "src/duality/RenderDispatcher3D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer3D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher3D::RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer3D>()) {}  

RenderDispatcher3D::~RenderDispatcher3D() = default;

void RenderDispatcher3D::dispatch(GeometryDataset& node) {
    m_geoRenderer->render(node, m_mvp);
}

void RenderDispatcher3D::startDraw() {
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher3D::finishDraw() {
    m_fbo->Read(0);
}

void RenderDispatcher3D::setMVP(const GLMatrix& mvp) {
    m_mvp = mvp;
}
