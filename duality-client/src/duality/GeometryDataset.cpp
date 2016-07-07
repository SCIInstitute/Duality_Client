#include "src/duality/GeometryDataset.h"

#include "src/duality/AbstractIO.h"

#include "IVDA/Vectors.h"
#include "duality/Error.h"

using namespace IVDA;

GeometryDataset::GeometryDataset(std::unique_ptr<DataProvider> provider, std::vector<Mat4f> transforms)
    : Dataset(std::move(provider))
    , m_transforms(transforms)
    , m_geometry(nullptr) {}

bool GeometryDataset::isTransparent() const {
    return !m_indicesTransparent.empty();
}

const std::vector<IVDA::Vec3f>& GeometryDataset::centroids() const {
    return m_centroids;
}

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

BoundingBox GeometryDataset::boundingBox() const {
    BoundingBox boundingBox;
    for (size_t i = 0; i < m_geometry->info.numberIndices; ++i) {
        auto offset = 3 * m_geometry->indices[i];
        const auto positions = m_geometry->positions;
        IVDA::Vec3f pos(positions[offset + 0], positions[offset + 1], positions[offset + 2]);
        boundingBox.min.StoreMin(pos);
        boundingBox.max.StoreMax(pos);
    }
    return boundingBox;
}

bool GeometryDataset::intersects(const BoundingBox& box) const {
    for (const auto& centroid : m_centroids) {
        if (centroid.x >= box.min.x && centroid.y >= box.min.y && centroid.z >= box.min.z &&
            centroid.x <= box.max.x && centroid.y <= box.max.y && centroid.z <= box.max.z) {
            return true;
        }
    }
    return false;
}

const G3D::GeometrySoA& GeometryDataset::geometry() const {
    return *m_geometry;
}