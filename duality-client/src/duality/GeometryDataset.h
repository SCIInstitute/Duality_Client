#pragma once

#include "src/duality/Dataset.h"
#include "src/duality/G3D.h"

class DataProvider;
class DatasetDispatcher;

class GeometryDataset : public Dataset {
public:
    GeometryDataset(std::vector<IVDA::Mat4f> transforms = {});
    GeometryDataset(std::unique_ptr<G3D::GeometrySoA> geometry, std::vector<IVDA::Mat4f> transforms = {});

    // SceneNode interface
    void applyTransform(const IVDA::Mat4f& matrix) override;
    void accept(DatasetDispatcher& renderer) override;
    void read(std::shared_ptr<std::vector<uint8_t>> data) override;

    const G3D::GeometryInfo* geometryInfo() const noexcept;
    const std::vector<uint32_t>& getIndices() const noexcept;
    const float* getPositions() const noexcept;
    const float* getNormals() const noexcept;
    const float* getTangents() const noexcept;
    const float* getColors() const noexcept;
    const float* getTexCoords() const noexcept;
    const float* getAlphas() const noexcept;

private:
    void assignShortcutPointers();

private:
    std::unique_ptr<G3D::GeometrySoA> m_geometry;

    // vertex attributes
    float* m_positions;
    float* m_normals;
    float* m_tangents;
    float* m_colors;
    float* m_texcoords;
    float* m_alphas;
};