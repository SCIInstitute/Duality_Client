#pragma once

#include "src/duality/BoundingBox.h"
#include "src/duality/DataProvider.h"
#include "src/duality/GLTexture2D.h"
#include "src/duality/I3M.h"
#include "src/duality/TransferFunction.h"

#include <array>

class VolumeDataset {
public:
    VolumeDataset(std::unique_ptr<DataProvider> provider);

    void updateDataset();
    void initializeDataset();

    struct SliceInfo {
        float depth;
        size_t textureIndex1;
        size_t textureIndex2;
        float interpolationParam;
    };
    const std::array<std::vector<SliceInfo>, 3>& sliceInfos() const;
    BoundingBox boundingBox() const;

    void bindTextures(size_t dir, size_t texIndex1, size_t texIndex2) const;

private:
    void initSliceInfos();
    void initTextures();
    size_t texelIndexInVolume(size_t x, size_t y, size_t z);

private:
    std::unique_ptr<DataProvider> m_provider;
    std::unique_ptr<I3M::Volume> m_volume;
    std::array<std::vector<SliceInfo>, 3> m_sliceInfos;
    std::array<std::vector<std::unique_ptr<GLTexture2D>>, 3> m_textures;
};