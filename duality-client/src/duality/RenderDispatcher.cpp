//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/RenderDispatcher.h"

#include "IVDA/GLInclude.h"
#include "duality/GeometryDataset.h"
#include "duality/private/GLFrameBufferObject.h"

#include <OpenGLES/ES3/gl.h>

RenderDispatcher::RenderDispatcher(const ScreenInfo& screenInfo)
    : m_geoRenderer(std::make_unique<GeometryRenderer>(screenInfo))
    , m_fbo(std::make_unique<GLFrameBufferObject>(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                                                  static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor), true))
    , m_modelView() {}

RenderDispatcher::~RenderDispatcher() = default;

void RenderDispatcher::setModelView(const GLMatrix& modelView) {
    m_modelView = modelView;
}

void RenderDispatcher::dispatch(GeometryDataset& node) {
    m_geoRenderer->render(node, m_modelView);
}

void RenderDispatcher::startDraw() {
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderDispatcher::finishDraw() {
    m_fbo->Read(0);
}