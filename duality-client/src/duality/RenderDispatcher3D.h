//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/BoundingBox.h"
#include "src/duality/DatasetDispatcher.h"
#include "src/duality/MVP3D.h"

class GLFrameBufferObject;
class GeometryRenderer3D;

class RenderDispatcher3D : public DatasetDispatcher {
public:
    RenderDispatcher3D(std::shared_ptr<GLFrameBufferObject> fbo);
    ~RenderDispatcher3D();

    void dispatch(GeometryDataset& node) override;
    void startDraw();
    void finishDraw();

    void setMVP(const GLMatrix& mvp);

private:
    std::shared_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    GLMatrix m_mvp;
};