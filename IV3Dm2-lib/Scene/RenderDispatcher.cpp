//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "Scene/RenderDispatcher.h"

#include "Scene/GeometryDataset.h"

RenderDispatcher::RenderDispatcher(const ScreenInfo& screenInfo)
    : m_geoRenderer(std::make_unique<GeometryRenderer>(screenInfo))
    , m_modelView(nullptr) {}

void RenderDispatcher::setModelView(const GLMatrix* modelView) {
    m_modelView = modelView;
}

void RenderDispatcher::dispatch(GeometryDataset& node) {
    m_geoRenderer->render(node, *m_modelView);
}

void RenderDispatcher::startDraw() {
    m_geoRenderer->startDraw();
}

void RenderDispatcher::finishDraw() {
    m_geoRenderer->finishDraw();
}