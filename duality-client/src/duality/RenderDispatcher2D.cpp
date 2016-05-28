//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "src/duality/RenderDispatcher2D.h"

#include "src/IVDA/GLFrameBufferObject.h"
#include "src/IVDA/GLInclude.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer2D.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher2D::RenderDispatcher2D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox)
    : m_fbo(std::make_unique<GLFrameBufferObject>(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                                                  static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor), true))
    , m_geoRenderer(std::make_unique<GeometryRenderer2D>())
    , m_mvp(screenInfo, boundingBox) {}

RenderDispatcher2D::~RenderDispatcher2D() = default;

void RenderDispatcher2D::dispatch(GeometryDataset& node) {
    m_geoRenderer->render(node, m_mvp.calculate());
}

// void RenderDispatcher2D::addTranslation(const IVDA::Vec2f& translation) {
//    m_mvp.addTranslation(translation);
//}

// void RenderDispatcher2D::addRotation(const IVDA::Mat4f& rotation) {
//    m_mvp.addRotation(rotation);
//}

void RenderDispatcher2D::setSlice(float slice) {
    m_geoRenderer->setSlice(slice);
}

void RenderDispatcher2D::startDraw() {
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher2D::finishDraw() {
    m_fbo->Read(0);
}