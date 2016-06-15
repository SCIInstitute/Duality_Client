#pragma once

#include "IVDA/GLMatrix.h"
#include "IVDA/Vectors.h"
#include "duality/CoordinateSystem.h"
#include "src/duality/VolumeDataset.h"
#include "src/duality/TransferFunction.h"

#include <memory>
#include <array>

class GLShader;

class VolumeRenderer2D {
public:
    VolumeRenderer2D();
    ~VolumeRenderer2D();
    
    void render(const VolumeDataset& dataset, const GLMatrix& mvp, const TransferFunction& tf, CoordinateAxis axis, float slice);
    
private:
    std::unique_ptr<GLShader> m_shader;
    std::array<std::array<IVDA::Vec3f, 4>, 3> m_vertices;
    std::array<IVDA::Vec2f, 4> m_texCoords;
};