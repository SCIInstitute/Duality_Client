#pragma once

#include "Dataset.h"
#include "G3D.h"

class AbstractRenderer;

class GeometryDataset : public Dataset {
public:
    GeometryDataset(AbstractReader& reader);

    void render(AbstractRenderer& renderer) const override;

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