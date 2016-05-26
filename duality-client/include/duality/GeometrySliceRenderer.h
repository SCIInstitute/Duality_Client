#pragma once

#include "duality/GLMatrix.h"
#include "duality/GeometryDataset.h"
#include "duality/ScreenInfo.h"

#include <memory>

class GLShader;

class GeometrySliceRenderer {
public:
    GeometrySliceRenderer(const ScreenInfo& screenInfo);
    ~GeometrySliceRenderer();
    
    void render(const GeometryDataset& dataset, const GLMatrix& modelView);
    
private:
    static GLMatrix createProjectionMatrix(const ScreenInfo& screenInfo);
    
private:
    GLMatrix m_projection;
    std::unique_ptr<GLShader> m_shader;
};