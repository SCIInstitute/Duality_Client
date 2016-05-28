//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "src/duality/RenderDispatcher3D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer3D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher3D::RenderDispatcher3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox)
    : m_fbo(std::make_unique<GLFrameBufferObject>(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                                                  static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor), true))
    , m_geoRenderer(std::make_unique<GeometryRenderer3D>())
    , m_mvp(screenInfo, boundingBox) {}

RenderDispatcher3D::~RenderDispatcher3D() = default;

void RenderDispatcher3D::dispatch(GeometryDataset& node) {
    m_geoRenderer->render(node, m_mvp.calculate());
}

void RenderDispatcher3D::addTranslation(const IVDA::Vec2f& translation) {
    m_mvp.addTranslation(translation);
}

void RenderDispatcher3D::addRotation(const IVDA::Mat4f& rotation) {
    m_mvp.addRotation(rotation);
}

void RenderDispatcher3D::startDraw() {
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher3D::finishDraw() {
    m_fbo->Read(0);
}