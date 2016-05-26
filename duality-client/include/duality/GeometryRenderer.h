#pragma once

#include "duality/GLMatrix.h"
#include "duality/GeometryDataset.h"
#include "duality/ScreenInfo.h"

#include <memory>

class GeometryRendererImpl;

class GeometryRenderer {
public:
    GeometryRenderer(const ScreenInfo& screenInfo);
    ~GeometryRenderer();

    void render(const GeometryDataset& dataset, const GLMatrix& modelView);

private:
    static GLMatrix createProjectionMatrix(const ScreenInfo& screenInfo);
    
private:
    GLMatrix m_projection;
    std::unique_ptr<GeometryRendererImpl> m_impl;
};