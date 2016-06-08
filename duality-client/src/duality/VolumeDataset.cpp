#include "src/duality/AbstractIO.h"
#include "src/duality/VolumeDataset.h"

VolumeDataset::VolumeDataset(Dataset::Visibility visibility)
    : Dataset({}, visibility) {}

void VolumeDataset::accept(DatasetDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

void VolumeDataset::read(std::shared_ptr<std::vector<uint8_t>> data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data->data()), data->size());
    m_volume = std::make_unique<I3M::Volume>();
    I3M::read(reader, *m_volume);
    initSliceInfos(m_volume->info);
}

void VolumeDataset::applyTransform(const IVDA::Mat4f& matrix) {
    // FIXME: how to apply transforms to volumetric dataset?
}

void VolumeDataset::initSliceInfos(const I3M::VolumeInfo& volumeInfo) {
    IVDA::Vec3f vMin = volumeInfo.scale * -0.5f;
    IVDA::Vec3f vMax = volumeInfo.scale * 0.5f;
    for (size_t dir = 0; dir < 3; ++dir) {
        for (size_t i = 0; i < volumeInfo.size[dir]; ++i) {
            float normalizedPosInStack = static_cast<float>(i) / static_cast<float>(volumeInfo.size[dir] - 1);
            float depth = vMin[dir] * (1.0f - normalizedPosInStack) + vMax[dir] * normalizedPosInStack;
            float sliceIndex = normalizedPosInStack * (volumeInfo.size[dir] - 1);
            size_t sliceIndex1 = std::min<size_t>(static_cast<size_t>(sliceIndex), volumeInfo.size[dir] - 1);
            size_t sliceIndex2 = std::min<size_t>(sliceIndex1 + 1, volumeInfo.size[dir] - 1);
            float interpolationParam = sliceIndex - sliceIndex1;
            m_sliceInfos[dir] = SliceInfo{depth, sliceIndex1, sliceIndex2, interpolationParam};
        }
    }
}