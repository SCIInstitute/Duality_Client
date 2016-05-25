#pragma once

#include "duality/AbstractRenderer.h"
#include "duality/G3D.h"
#include "duality/GeometryDataset.h"
#include "duality/ScreenInfo.h"

#include <memory>

class GeometryRendererImpl;

class GeometryRenderer : public AbsstractRenderer {
public:
    GeometryRenderer(const ScreenInfo& screenInfo);
    ~GeometryRenderer();

    void render(const GeometryDataset& dataset, const GLMatrix& modelView);

private:
    std::unique_ptr<GeometryRendererImpl> m_impl;
};