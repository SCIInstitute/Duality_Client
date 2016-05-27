#pragma once

#include "duality/GLMatrix.h"
#include "duality/GeometryDataset.h"
#include "duality/ScreenInfo.h"

#include <memory>

class GLShader;

class GeometryRenderer2D {
public:
    GeometryRenderer2D();
    ~GeometryRenderer2D();

    void render(const GeometryDataset& dataset, const GLMatrix& mvp);
    
    void setSlice(float slice);
    
private:
    std::unique_ptr<GLShader> m_shader;
    float m_slice;
};