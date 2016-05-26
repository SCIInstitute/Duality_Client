//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "duality/DatasetDispatcher.h"
#include "duality/GeometryRenderer.h"

class GLFrameBufferObject;

class RenderDispatcher : public DatasetDispatcher {
public:
    RenderDispatcher(const ScreenInfo& screenInfo);
    ~RenderDispatcher();
    
    void setModelView(const GLMatrix& modelView);
    void dispatch(GeometryDataset& node) override;
    void startDraw();
    void finishDraw();
private:
    std::unique_ptr<GeometryRenderer> m_geoRenderer;
    std::unique_ptr<GLFrameBufferObject> m_fbo;
    GLMatrix m_modelView;
};