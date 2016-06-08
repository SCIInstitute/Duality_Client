#pragma once

#include "src/duality/Dataset.h"
#include "src/duality/I3M.h"

#include <array>

class VolumeDataset : public Dataset {
public:
    VolumeDataset(Dataset::Visibility visibility);

    virtual void accept(DatasetDispatcher& dispatcher) override;

private:
    virtual void read(std::shared_ptr<std::vector<uint8_t>> data) override;
    virtual void applyTransform(const IVDA::Mat4f& matrix) override;

    void initSliceInfos(const I3M::VolumeInfo& volumeInfo);
    struct SliceInfo {
        float depth;
        size_t textureIndex1;
        size_t textureIndex2;
        float interpolationParam;
    };

private:
    std::unique_ptr<I3M::Volume> m_volume;
    std::array<std::vector<SliceInfo>, 3> m_sliceInfos;
};