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
    StackDirection stackDir = duality::determineStackDirection(static_cast<IVDA::Mat4f>(mvp.mv()));
    const auto& sliceInfos = volumeDataset.sliceInfos()[stackDir.direction];
    float minDepth = sliceInfos[0].depth;
    float maxDepth = sliceInfos[sliceInfos.size() - 1].depth;
    float stackDepth = maxDepth - minDepth;
    size_t numSlices = sliceInfos.size();

    using SlapAssignment = std::map<size_t, std::vector<uint32_t>>;
    std::vector<SlapAssignment> slapAssignments(geometryDatasets.size());

    for (size_t geoIndex = 0; geoIndex < geometryDatasets.size(); ++geoIndex) {
        const auto& ds = *geometryDatasets[geoIndex];
        const auto& centroids = ds.centroids();
        const auto& indices = ds.indicesTransparent();

        auto permutation = duality::backToFrontPermutation(centroids, mvp.eyePos());

        std::vector<IVDA::Vec3f> sortedCentroids;
        duality::applyPermutation<1>(permutation, centroids, sortedCentroids);

        std::vector<uint32_t> indicesSorted;
        if (ds.geometry().info.primitiveType == G3D::Point) {
            duality::applyPermutation<1>(permutation, indices, indicesSorted);
        } else if (ds.geometry().info.primitiveType == G3D::Line) {
            duality::applyPermutation<2>(permutation, indices, indicesSorted);
        } else if (ds.geometry().info.primitiveType == G3D::Triangle) {
            duality::applyPermutation<3>(permutation, indices, indicesSorted);
        }

        for (size_t centroidIndex = 0; centroidIndex < sortedCentroids.size(); ++centroidIndex) {
            const auto& centroid = sortedCentroids[centroidIndex];
            float centroidDepth = centroid[stackDir.direction];

            size_t slapIndex;
            if (centroidDepth <= minDepth) {
                slapIndex = 0;
            } else if (centroidDepth >= maxDepth) {
                slapIndex = numSlices;
            } else {
                slapIndex = 1 + size_t(std::min<int>((int)numSlices - 1,
                                                     std::max<int>(0, int((numSlices - 1) * (centroidDepth - minDepth) / stackDepth))));
            }

            // FIXME: hard coded 3
            auto indexStart = begin(indicesSorted) + 3 * centroidIndex;
            auto indexEnd = indexStart + 3;
            std::copy(indexStart, indexEnd, std::back_inserter(slapAssignments[geoIndex][slapIndex]));
        }
    }

    for (size_t i = 0; i <= numSlices; ++i) {
        const size_t renderIndex = stackDir.reverse ? numSlices - i : i;
        for (size_t geoIndex = 0; geoIndex < geometryDatasets.size(); ++geoIndex) {
            if (slapAssignments[geoIndex].count(renderIndex)) {
                const auto& ds = *geometryDatasets[geoIndex];
                m_geoRenderer->renderTransparentPartial(ds, mvp, slapAssignments[geoIndex][renderIndex]);
            }
        }
        
        if (i < numSlices ) {
            m_volRenderer->renderPartial(volumeDataset, mvp, tf, stackDir, i);
        }
    }
}