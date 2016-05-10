#pragma once

#include "Dataset.h"
#include "G3D.h"

class AbstractRenderer;

class GeometryDataset : public Dataset {
public:
    GeometryDataset(AbstractReader& reader);

    void render(AbstractRenderer& renderer) const override;

    const G3D::GeometryInfo& geometryInfo() const;
    
    const uint32_t* getIndices() const;
    const float* getPositions() const;
    const float* getNormals() const;
    const float* getTangents() const;
    const float* getColors() const;
    const float* getTexCoords() const;
    const float* getAlphas() const;

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