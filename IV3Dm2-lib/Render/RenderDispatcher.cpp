//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "RenderDispatcher.h"

#include "Scene/GeometryNode.h"

RenderDispatcher::RenderDispatcher(const ScreenInfo& screenInfo)
: m_geoRenderer(std::make_unique<GeometryRenderer>(screenInfo)) {}

void RenderDispatcher::dispatch(GeometryNode& node) {
    m_geoRenderer->render(node);
}
