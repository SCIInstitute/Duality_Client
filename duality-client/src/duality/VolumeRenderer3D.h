#pragma once

#include "IVDA/Vectors.h"
#include "src/duality/MVP3D.h"
#include "src/duality/VolumeDataset.h"

#include <memory>

class GLShader;

class VolumeRenderer3D {
public:
    VolumeRenderer3D();
    ~VolumeRenderer3D();

    void render(const VolumeDataset& dataset, const MVP3D& mvp, const TransferFunction& tf);

private:
    GLShader& determineActiveShader() const;
    void updateStackDirection(const IVDA::Mat4f& mv);

private:
    std::unique_ptr<GLShader> m_shaderL;
    std::unique_ptr<GLShader> m_shaderNL;
    std::unique_ptr<GLShader> m_shaderLTI;
    std::unique_ptr<GLShader> m_shaderNLTI;
    size_t m_stackDirection;
    bool m_reverseStackOrdering;
};