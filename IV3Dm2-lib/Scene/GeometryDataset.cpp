#include "GeometryDataset.h"

#include "Common/Error.h"
#include "Render/AbstractRenderer.h"

GeometryDataset::GeometryDataset(AbstractReader& reader)
    : m_geometry(std::make_unique<G3D::GeometrySoA>()) {
    G3D::read(reader, m_geometry.get());

    if (m_geometry->info.numberVertices > 0) {
        assignShortcutPointers();
    } else {
        G3D::clean(m_geometry.get());
    }
}

void GeometryDataset::render(AbstractRenderer& renderer) const
{

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