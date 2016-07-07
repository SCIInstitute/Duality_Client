#pragma once

#include "IVDA/Vectors.h"
#include "duality/CoordinateSystem.h"
#include "src/duality/MVP3D.h"
#include "src/duality/VolumeDataset.h"

#include <memory>

class GLShader;

struct StackDirection {
    CoordinateAxis direction;
    bool reverse;
};

namespace duality {
StackDirection determineStackDirection(const IVDA::Mat4f& mv);
}

class VolumeRenderer3D {
public:
    VolumeRenderer3D();
    ~VolumeRenderer3D();

    void render(const VolumeDataset& dataset, const MVP3D& mvp, const TransferFunction& tf);
    void renderPartial(const VolumeDataset& dataset, const MVP3D& mvp, const TransferFunction& tf, const StackDirection& stackDir, size_t slice);

private:
    GLShader& determineActiveShader() const;

private:
    std::unique_ptr<GLShader> m_shaderL;
    std::unique_ptr<GLShader> m_shaderNL;
    std::unique_ptr<GLShader> m_shaderLTI;
    std::unique_ptr<GLShader> m_shaderNLTI;
};