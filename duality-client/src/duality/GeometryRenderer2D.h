#pragma once

#include "IVDA/GLMatrix.h"
#include "duality/CoordinateSystem.h"
#include "src/duality/GeometryDataset.h"

#include <memory>

class GLShader;

class GeometryRenderer2D {
public:
    GeometryRenderer2D();
    ~GeometryRenderer2D();

    void render(const GeometryDataset& dataset, const GLMatrix& mvp, CoordinateAxis axis, float slice);

private:
    std::unique_ptr<GLShader> m_shader;
};