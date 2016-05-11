//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/AbstractDispatcher.h"
#include "Scene/GeometryRenderer.h"

class RenderDispatcher : public AbstractDispatcher {
public:
    RenderDispatcher(const ScreenInfo& screenInfo);
    
    void dispatch(GeometryNode& node) override;
    
private:
    std::unique_ptr<GeometryRenderer> m_geoRenderer;
};