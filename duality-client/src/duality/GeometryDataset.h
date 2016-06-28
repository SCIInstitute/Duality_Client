#pragma once

#include "src/duality/G3D.h"

#include "src/duality/Dataset.h"

#include "IVDA/GLMatrix.h"
#include "IVDA/Vectors.h"

#include <array>
#include <numeric>

class DataProvider;
class NodeDispatcher;

class GeometryDataset : public Dataset {
public:
    GeometryDataset(std::unique_ptr<DataProvider> provider, std::vector<IVDA::Mat4f> transforms = {});

    const std::vector<uint32_t>& indicesOpaque() const;
    const std::vector<uint32_t>& indicesTransparent() const;

    const G3D::GeometrySoA& geometry() const;

private:
    void readData(const std::vector<uint8_t>& data) override;

    void presortIndices();
    template <int32_t size> void presortIndices() {
        for (int32_t i = 0; i < m_geometry->indices.size(); i += size) {

            bool isTransparent = false;
            for (int32_t j = 0; j < size; ++j) {
                if (m_geometry->colors[(i + j) * 4 + 3] <= 0.95f) {
                    isTransparent = true;
                    break;
                }
            }

            if (isTransparent) {
                for (int32_t k = 0; k < size; ++k) {
                    m_indicesTransparent.push_back(i + k);
                }
            } else {
                for (int32_t k = 0; k < size; ++k) {
                    m_indicesOpaque.push_back(i + k);
                }
            }
        }
    }

    void computeCentroids();
    template <int32_t size> void computeCentroids() {
        for (int32_t i = 0; i < m_indicesTransparent.size(); i += size) {
            std::array<IVDA::Vec3f, size> pos;
            for (int32_t j = 0; j < size; ++j) {
                pos[j] = IVDA::Vec3f(m_geometry->positions + (m_indicesTransparent[i + j] * size));
            }
            IVDA::Vec3f centroid = std::accumulate(begin(pos), end(pos), IVDA::Vec3f()) / static_cast<float>(size);
            m_centroids.push_back(centroid);
        }
    }

private:
    std::unique_ptr<G3D::GeometrySoA> m_geometry;
    std::vector<IVDA::Mat4f> m_transforms;
    std::vector<uint32_t> m_indicesOpaque;
    std::vector<uint32_t> m_indicesTransparent;
    std::vector<IVDA::Vec3f> m_centroids;
};