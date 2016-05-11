#include "GeometryNode.h"

#include "AbstractDispatcher.h"
#include "AbstractIO.h"
#include "Common/Error.h"
#include "IVDA/Vectors.h"

using namespace IVDA;

GeometryNode::GeometryNode(std::unique_ptr<DataProvider> provider, SceneNode::MatrixTriple transforms)
    : SceneNode(std::move(provider), std::move(transforms))
    , m_geometry(nullptr)
    , m_positions(nullptr)
    , m_normals(nullptr)
    , m_tangents(nullptr)
    , m_colors(nullptr)
    , m_texcoords(nullptr)
    , m_alphas(nullptr) {}

void GeometryNode::applyTransform(const Mat4f& matrix) {
    uint32_t numVertices = m_geometry->info.numberVertices;
    {
        float* positions = const_cast<float*>(this->getPositions());
        if (positions != nullptr) {
            for (uint32_t i = 0; i < numVertices; ++i, positions += 3) {
                IVDA::Vec4f position(IVDA::Vec3f(m_positions), 1);
                position = matrix * position;
                positions[0] = position.x;
                positions[1] = position.y;
                positions[2] = position.z;
            }
        }
    }
    {
        float* normals = const_cast<float*>(this->getNormals());
        if (normals != nullptr) {
            Mat4f normalMatrix = matrix.inverse().Transpose();
            for (uint32_t i = 0; i < numVertices; ++i, normals += 3) {
                Vec4f normal(Vec3f(normals), 0);
                normal = normalMatrix * normal;
                normals[0] = normal.x;
                normals[1] = normal.y;
                normals[2] = normal.z;
            }
        }
    }
    {
        float* tangents = const_cast<float*>(this->getTangents());
        if (tangents != nullptr) {
            for (uint32_t i = 0; i < numVertices; ++i, tangents += 3) {
                Vec4f tangent(Vec3f(tangents), 0);
                tangent = matrix * tangent;
                tangents[0] = tangent.x;
                tangents[1] = tangent.y;
                tangents[2] = tangent.z;
            }
        }
    }
}

void GeometryNode::accept(AbstractDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryNode::readDataset(std::shared_ptr<std::vector<uint8_t>> data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data->data()), data->size());
    m_geometry = std::make_unique<G3D::GeometrySoA>();
    G3D::read(reader, m_geometry.get());
    assignShortcutPointers();
}

const G3D::GeometryInfo& GeometryNode::geometryInfo() const noexcept {
    return m_geometry->info;
}

const uint32_t* GeometryNode::getIndices() const noexcept {
    return m_geometry->indices;
}

const float* GeometryNode::getPositions() const noexcept {
    return m_positions;
}

const float* GeometryNode::getNormals() const noexcept {
    return m_normals;
}

const float* GeometryNode::getTangents() const noexcept {
    return m_tangents;
}

const float* GeometryNode::getColors() const noexcept {
    return m_colors;
}

const float* GeometryNode::getTexCoords() const noexcept {
    return m_texcoords;
}

const float* GeometryNode::getAlphas() const noexcept {
    return m_alphas;
}

void GeometryNode::assignShortcutPointers() {
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