#include "src/duality/GeometryDataset.h"

#include "src/duality/AbstractIO.h"
#include "src/duality/NodeDispatcher.h"

#include "IVDA/Vectors.h"
#include "duality/Error.h"

using namespace IVDA;

GeometryDataset::GeometryDataset(std::unique_ptr<DataProvider> provider, std::vector<IVDA::Mat4f> transforms)
    : Dataset(std::move(provider))
    , m_transforms(transforms)
    , m_geometry(nullptr) {}

const std::vector<uint32_t>& GeometryDataset::indicesOpaque() const {
    return m_indicesOpaque;
}

const std::vector<uint32_t>& GeometryDataset::indicesTransparent() const {
    return m_indicesTransparent;
}

void GeometryDataset::readData(const std::vector<uint8_t>& data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data.data()), data.size());
    m_geometry = std::make_unique<G3D::GeometrySoA>();
    G3D::readSoA(reader, *m_geometry);
    for (const auto& transform : m_transforms) {
        G3D::applyTransform(*m_geometry, transform);
    }
    presortIndices();
    computeCentroids();
}

void GeometryDataset::presortIndices() {
    m_indicesOpaque.clear();
    m_indicesTransparent.clear();

    if (m_geometry->info.primitiveType == G3D::Point) {
        presortIndices<1>();
    } else if (m_geometry->info.primitiveType == G3D::Line) {
        presortIndices<2>();
    } else if (m_geometry->info.primitiveType == G3D::Triangle) {
        presortIndices<3>();
    }
}

void GeometryDataset::computeCentroids() {
    m_centroids.clear();
    if (m_geometry->info.primitiveType == G3D::Point) {
        computeCentroids<1>();
    } else if (m_geometry->info.primitiveType == G3D::Line) {
        computeCentroids<2>();
    } else if (m_geometry->info.primitiveType == G3D::Triangle) {
        computeCentroids<3>();
    }
}

const G3D::GeometrySoA& GeometryDataset::geometry() const {
    return *m_geometry;
}