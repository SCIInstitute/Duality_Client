#pragma once

#include "IVDA/GLMatrix.h"
#include "src/duality/VolumeDataset.h"

#include <memory>

class GLShader;

class VolumeRenderer3D {
public:
    VolumeRenderer3D();
    ~VolumeRenderer3D();

    void render(const VolumeDataset& dataset, const GLMatrix& mvp);

private:
    std::unique_ptr<GLShader> m_shaderL;
    std::unique_ptr<GLShader> m_shaderNL;
    std::unique_ptr<GLShader> m_shaderLTI;
    std::unique_ptr<GLShader> m_shaderNLTI;
};