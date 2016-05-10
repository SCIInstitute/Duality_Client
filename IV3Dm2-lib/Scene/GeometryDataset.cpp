#include "GeometryDataset.h"

#include "Common/Error.h"
#include "Render/AbstractRenderer.h"

GeometryDataset::GeometryDataset(AbstractReader& reader)
    : m_geometry(std::make_unique<G3D::GeometrySoA>())
    , m_positions(nullptr)
    , m_normals(nullptr)
    , m_tangents(nullptr)
    , m_colors(nullptr)
    , m_texcoords(nullptr)
    , m_alphas(nullptr) {
    G3D::read(reader, m_geometry.get());

    if (m_geometry->info.numberVertices > 0) {
        assignShortcutPointers();
    } else {
        G3D::clean(m_geometry.get());
    }
}

void GeometryDataset::render(AbstractRenderer& renderer) const {
    renderer.render(*this);
}

const G3D::GeometryInfo& GeometryDataset::geometryInfo() const {
    return m_geometry->info;
}

const uint32_t* GeometryDataset::getIndices() const {
    return m_geometry->indices;
}

const float* GeometryDataset::getPositions() const {
    return m_positions;
}

const float* GeometryDataset::getNormals() const {
    return m_normals;
}

const float* GeometryDataset::getTangents() const {
    return m_tangents;
}

const float* GeometryDataset::getColors() const {
    return m_colors;
}

const float* GeometryDataset::getTexCoords() const {
    return m_texcoords;
}

const float* GeometryDataset::getAlphas() const {
    return m_alphas;
}

void GeometryDataset::assignShortcutPointers() {
    for (size_t i = 0; i < m_geometry->info.attributeSemantics.size(); ++i) {
        switch (m_geometry->info.attributeSemantics[i]) {
        case G3D::Position: {
            m_positions = m_geometry->vertexAttributes.at(i);
            break;
        }
        case G3D::Normal: {
            m_normals = m_geometry->vertexAttributes.at(i);
            break;
        }
        case G3D::Tangent: {
            m_tangents = m_geometry->vertexAttributes.at(i);
            break;
        }
        case G3D::Color: {
            m_colors = m_geometry->vertexAttributes.at(i);
            break;
        }
        case G3D::Tex: {
            m_texcoords = m_geometry->vertexAttributes.at(i);
            break;
        }
        case G3D::Float: {
            m_alphas = m_geometry->vertexAttributes.at(i);
            break;
        }
        }
    }
}