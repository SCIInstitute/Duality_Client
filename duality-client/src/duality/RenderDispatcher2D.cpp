//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "src/duality/RenderDispatcher2D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer2D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher2D::RenderDispatcher2D(std::shared_ptr<GLFrameBufferObject> fbo)
    : m_fbo(fbo)
    , m_geoRenderer(std::make_unique<GeometryRenderer2D>()) {}

RenderDispatcher2D::~RenderDispatcher2D() = default;

void RenderDispatcher2D::dispatch(GeometryDataset& node) {
    m_geoRenderer->render(node, m_mvp, m_axis, m_slice);
}

void RenderDispatcher2D::startDraw() {
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher2D::finishDraw() {
    m_fbo->Read(0);
}

void RenderDispatcher2D::setMVP(const GLMatrix& mvp) {
    m_mvp = mvp;
}

void RenderDispatcher2D::setSlice(float slice) {
    m_slice = slice;
}

void RenderDispatcher2D::setAxis(CoordinateAxis axis) {
    m_axis = axis;
}
