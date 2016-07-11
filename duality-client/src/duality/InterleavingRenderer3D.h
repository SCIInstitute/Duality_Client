#pragma once

#include "src/duality/VolumeDataset.h"
#include "src/duality/VolumeRenderer3D.h"

#include <memory>
#include <unordered_map>
#include <vector>

class GeometryDataset;
class GeometryRenderer3D;
class MVP3D;
class TransferFunction;

class InterleavingRenderer3D {
public:
    InterleavingRenderer3D();
    ~InterleavingRenderer3D();

    void render(const VolumeDataset& volumeDataset, const std::vector<const GeometryDataset*>& geometryDatasets, const MVP3D& mvp,
                const TransferFunction& tf);

private:
    using SlapAssignment = std::unordered_map<size_t, std::vector<uint32_t>>;

    std::vector<SlapAssignment> calculateSlapAssignments(const VolumeDataset& volumeDataset,
                                                         const std::vector<const GeometryDataset*>& geometryDatasets,
                                                         const StackDirection& stackDir, const MVP3D& mvp);
    void renderGeometries(const std::vector<const GeometryDataset*>& geometryDatasets, const MVP3D& mvp, const size_t sliceIndex,
                          const std::vector<SlapAssignment>& slapAssignments);

private:
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer3D> m_volRenderer;
};