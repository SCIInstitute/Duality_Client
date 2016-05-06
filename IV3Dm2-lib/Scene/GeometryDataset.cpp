#include "GeometryDataset.h"

#include "Common/Error.h"

GeometryDataset::GeometryDataset(AbstractReader& reader)
    : m_geometry(std::make_unique<G3D::GeometrySoA>()) {
    G3D::read(reader, m_geometry.get());

    if (m_geometry->info.numberVertices > 0) {
        if (m_geometry->info.indexSize == sizeof(uint32_t)) {
            // TODO: this is a really bad hack since it simply casts 32bit
            //       indices down to 16 bit instead of fixing the problems
            //       i.e., breaking the mesh up into multiple meshes
            //       this needs to be fixed

            if (m_geometry->info.numberIndices > std::numeric_limits<uint16_t>::max()) {
                THROW_ERROR("Ignoring " << m_geometry->info.numberIndices - std::numeric_limits<uint16_t>::max() << " indices above 2^16");
            }

            m_geometry->info.indexSize = sizeof(uint16_t);
            uint16_t* indices = (uint16_t*)new char[m_geometry->info.numberIndices * m_geometry->info.indexSize];
            for (size_t i = 0; i < m_geometry->info.numberIndices; ++i) {
                if (m_geometry->indices[i] > std::numeric_limits<uint16_t>::max()) {
                    indices[i] = 0;
                } else {
                    indices[i] = (uint16_t)m_geometry->indices[i];
                }
            }
            delete[] m_geometry->indices;
            m_geometry->indices = (uint32_t*)indices;
        }

        assignShortcutPointers();
    } else {
        G3D::clean(m_geometry.get());
        m_geometry = nullptr;
    }
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