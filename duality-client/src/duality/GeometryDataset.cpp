#include "src/duality/GeometryDataset.h"

#include "src/duality/AbstractIO.h"
#include "src/duality/DatasetDispatcher.h"

#include "IVDA/Vectors.h"
#include "duality/Error.h"

using namespace IVDA;

GeometryDataset::GeometryDataset(std::unique_ptr<DataProvider> provider, std::vector<IVDA::Mat4f> transforms)
    : Dataset(std::move(provider))
    , m_transforms(transforms)
    , m_geometry(nullptr) {}

void GeometryDataset::accept(DatasetDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void GeometryDataset::readData(const std::vector<uint8_t>& data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data.data()), data.size());
    m_geometry = std::make_unique<G3D::GeometrySoA>();
    G3D::readSoA(reader, *m_geometry);
    for (const auto& transform : m_transforms) {
        G3D::applyTransform(*m_geometry, transform);
    }
}

const G3D::GeometrySoA& GeometryDataset::geometry() const {
    return *m_geometry;
}