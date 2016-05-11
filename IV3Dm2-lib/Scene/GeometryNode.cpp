#include "GeometryNode.h"

#include "AbstractDispatcher.h"
#include "AbstractIO.h"
#include "Common/Error.h"

GeometryNode::GeometryNode(std::unique_ptr<DataProvider> provider, SceneNode::MatrixTriple transforms)
    : SceneNode(std::move(provider), std::move(transforms))
    , m_geometry(nullptr)
    , m_positions(nullptr)
    , m_normals(nullptr)
    , m_tangents(nullptr)
    , m_colors(nullptr)
    , m_texcoords(nullptr)
    , m_alphas(nullptr) {}

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