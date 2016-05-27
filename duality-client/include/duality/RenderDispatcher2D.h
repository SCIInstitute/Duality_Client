//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "duality/DatasetDispatcher.h"
#include "duality/ScreenInfo.h"
#include "duality/MVP2D.h"
#include "IVDA/Vectors.h"

class GLFrameBufferObject;
class GeometryRenderer2D;

class RenderDispatcher2D : public DatasetDispatcher {
public:
    RenderDispatcher2D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox);
    ~RenderDispatcher2D();
    
    //void addTranslation(const IVDA::Vec2f& translation);
    //void addRotation(const IVDA::Mat4f& rotation);
    void setSlice(float slice);
    
    void dispatch(GeometryDataset& node) override;
    void startDraw();
    void finishDraw();
    
private:
    std::unique_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer2D> m_geoRenderer;
    MVP2D m_mvp;
};