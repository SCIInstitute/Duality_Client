#pragma once

#include "src/duality/Dataset.h"
#include "src/duality/I3M.h"
#include "src/duality/GLTexture2D.h"
#include "src/duality/TransferFunction.h"
#include "src/duality/BoundingBox.h"

#include <array>

class VolumeDataset : public Dataset {
public:
    VolumeDataset(Dataset::Visibility visibility);

    virtual void accept(DatasetDispatcher& dispatcher) override;

    struct SliceInfo {
        float depth;
        size_t textureIndex1;
        size_t textureIndex2;
        float interpolationParam;
    };
    const std::array<std::vector<SliceInfo>, 3>& sliceInfos() const;
    BoundingBox boundingBox() const;

    void bindTextures(size_t dir, size_t slice) const;

private:
    virtual void read(std::shared_ptr<std::vector<uint8_t>> data) override;
    virtual void applyTransform(const IVDA::Mat4f& matrix) override;

    void initSliceInfos(const I3M::VolumeInfo& volumeInfo);
    void initTextures();
    size_t texelIndexInVolume(size_t x, size_t y, size_t z);
    void initTransferFunction(const TransferFunction& tf);

private:
    std::unique_ptr<I3M::Volume> m_volume;
    std::unique_ptr<GLTexture2D> m_tf;
    std::array<std::vector<SliceInfo>, 3> m_sliceInfos;
    std::array<std::vector<GLTexture2D>, 3> m_textures;
};