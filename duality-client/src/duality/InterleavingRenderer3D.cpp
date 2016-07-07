#include "src/duality/InterleavingRenderer3D.h"

#include "duality/CoordinateSystem.h"
#include "src/duality/GeometryDataset.h"
#include "src/duality/GeometryRenderer3D.h"
#include "src/duality/VolumeDataset.h"
#include "src/duality/VolumeRenderer3D.h"

#include <unordered_map>

InterleavingRenderer3D::InterleavingRenderer3D()
    : m_geoRenderer(std::make_unique<GeometryRenderer3D>())
    , m_volRenderer(std::make_unique<VolumeRenderer3D>()) {}

InterleavingRenderer3D::~InterleavingRenderer3D() = default;

void InterleavingRenderer3D::render(const VolumeDataset& volumeDataset, const std::vector<const GeometryDataset*>& geometryDatasets,
                                    const MVP3D& mvp, const TransferFunction& tf) {
    CoordinateAxis stackDirection = CoordinateAxis::X_Axis; // FIXME!!!
    const auto& sliceInfos = volumeDataset.sliceInfos()[stackDirection];
    float minDepth = sliceInfos[0].depth;
    float maxDepth = sliceInfos[sliceInfos.size() - 1].depth;
    float stackDepth = maxDepth - minDepth;
    size_t numSlices = sliceInfos.size();

    using SlapAssignment = std::map<size_t, std::vector<uint32_t>>;
    std::vector<SlapAssignment> slapAssignments(geometryDatasets.size());

    for (size_t geoIndex = 0; geoIndex < geometryDatasets.size(); ++geoIndex) {
        const auto& ds = *geometryDatasets[geoIndex];
        auto sortedIndices = ds.indicesTransparentSorted(mvp.eyePos());
        const auto& centroids = ds.centroids();
        for (size_t centroidIndex = 0; centroidIndex < centroids.size(); ++centroidIndex) {
            const auto& centroid = centroids[centroidIndex]; // FIXME: use sorted centroids
            float centroidDepth = centroid[stackDirection];

            // FIXME: hard coded 3
            size_t slapIndex;
            if (centroidDepth <= minDepth) {
                slapIndex = 0;
            } else if (centroidDepth >= maxDepth) {
                slapIndex = numSlices;
            } else {
                slapIndex = 1 + size_t(std::min<int>((int)numSlices - 1,
                                                     std::max<int>(0, int((numSlices - 1) * (centroidDepth - minDepth) / stackDepth))));
            }

            auto indexStart = begin(sortedIndices) + 3 * centroidIndex;
            auto indexEnd = indexStart + 3;
            std::copy(indexStart, indexEnd, std::back_inserter(slapAssignments[geoIndex][slapIndex]));
        }
    }
}