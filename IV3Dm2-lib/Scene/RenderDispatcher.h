//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/DatasetDispatcher.h"
#include "Scene/GeometryRenderer.h"

class RenderDispatcher : public DatasetDispatcher {
public:
    RenderDispatcher(const ScreenInfo& screenInfo);
    void setModelView(const GLMatrix* modelView);
    void dispatch(GeometryDataset& node) override;
    void startDraw();
    void finishDraw();
private:
    std::unique_ptr<GeometryRenderer> m_geoRenderer;
    const GLMatrix* m_modelView;
};