//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "IVDA/Vectors.h"
#include "duality/BoundingBox.h"
#include "duality/DatasetDispatcher.h"
#include "duality/MVP3D.h"
#include "duality/ScreenInfo.h"

class GLFrameBufferObject;
class GeometryRenderer3D;

class RenderDispatcher3D : public DatasetDispatcher {
public:
    RenderDispatcher3D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox);
    ~RenderDispatcher3D();

    void addTranslation(const IVDA::Vec2f& translation);
    void addRotation(const IVDA::Mat4f& rotation);

    void dispatch(GeometryDataset& node) override;
    void startDraw();
    void finishDraw();

private:
    std::unique_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    MVP3D m_mvp;
};