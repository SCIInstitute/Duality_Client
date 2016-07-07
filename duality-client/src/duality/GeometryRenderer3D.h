#pragma once

#include "IVDA/Vectors.h"
#include "duality/ScreenInfo.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/MVP3D.h"

class GLShader;

class GeometryRenderer3D {
public:
    GeometryRenderer3D();
    ~GeometryRenderer3D();

    void renderOpaque(const GeometryDataset& dataset, const MVP3D& mvp);
    void renderTransparent(const GeometryDataset& dataset, const MVP3D& mvp);

private:
    static int primitiveTypeGL(const GeometryDataset& dataset);
    static int enableAttributeArrays(const GeometryDataset& dataset);
    GLShader& determineActiveShader(const GeometryDataset& dataset) const;

private:
    std::unique_ptr<GLShader> m_normShader;
    std::unique_ptr<GLShader> m_normAlphaShader;
    std::unique_ptr<GLShader> m_normTexAlphaShader;
    std::unique_ptr<GLShader> m_normColShader;
    std::unique_ptr<GLShader> m_colShader;
    std::unique_ptr<GLShader> m_normTexShader;
    std::unique_ptr<GLShader> m_texShader;
};

namespace duality {
std::vector<int32_t> backToFrontPermutation(const std::vector<IVDA::Vec3f>& points, const IVDA::Vec3f& refPoint);

template <int stride, typename T>
void applyPermutation(const std::vector<int32_t> permutation, const std::vector<T>& source, std::vector<T>& target) {
    target.resize(source.size());
    for (size_t i = 0; i < permutation.size(); ++i) {
        for (size_t j = 0; j < stride; ++j) {
            target[i * stride + j] = source[permutation[i] * stride + j];
        }
    }
}
}