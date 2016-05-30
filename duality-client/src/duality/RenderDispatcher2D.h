//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/DatasetDispatcher.h"
#include "src/duality/MVP2D.h"

class GLFrameBufferObject;
class GeometryRenderer2D;

class RenderDispatcher2D : public DatasetDispatcher {
public:
    RenderDispatcher2D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox);
    ~RenderDispatcher2D();

    void dispatch(GeometryDataset& node) override;
    void startDraw();
    void finishDraw();

    void setMVP(const GLMatrix& mvp);
    void setAxis(CoordinateAxis axis);
    void setSlice(const float slice);

private:
    std::unique_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer2D> m_geoRenderer;
    GLMatrix m_mvp;
    CoordinateAxis m_axis;
    float m_slice;
};