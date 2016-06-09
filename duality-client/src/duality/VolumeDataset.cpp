#include "src/duality/AbstractIO.h"
#include "src/duality/VolumeDataset.h"

#include <algorithm>

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
            m_sliceInfos[dir].push_back(SliceInfo{depth, sliceIndex1, sliceIndex2, interpolationParam});
        }
    }

    for (size_t dir = 0; dir < 3; ++dir) {
        std::sort(begin(m_sliceInfos[dir]), end(m_sliceInfos[dir]),
                  [](const SliceInfo& s1, const SliceInfo& s2) { return s1.depth < s2.depth; });
    }
}

void VolumeDataset::initTextures() {
    for (size_t dir = 0; dir < 3; ++dir) {
        int sizeU, sizeV, sizeW;
        switch (dir) {
        case 0:
            sizeU = m_volume->info.size.y;
            sizeV = m_volume->info.size.z;
            sizeW = m_volume->info.size.x;
            break;
        case 1:
            sizeU = m_volume->info.size.x;
            sizeV = m_volume->info.size.z;
            sizeW = m_volume->info.size.y;
            break;
        case 2:
            sizeU = m_volume->info.size.x;
            sizeV = m_volume->info.size.y;
            sizeW = m_volume->info.size.z;
            break;
        }

        std::vector<std::array<uint8_t, 4>> pixels(sizeU * sizeV);
        for (size_t slice = 0; slice < sizeW; ++slice) {
            for (size_t v = 0; v < sizeV; ++v) {
                for (size_t u = 0; u < sizeU; ++u) {
                    size_t index = 0;
                    switch (dir) {
                    case 0:
                        index = texelIndexInVolume(slice, u, v);
                        break;
                    case 1:
                        index = texelIndexInVolume(u, slice, v);
                        break;
                    case 2:
                        index = texelIndexInVolume(u, v, slice);
                        break;
                    }
                    pixels[v * sizeU + u] = {m_volume->voxels[index], m_volume->voxels[index + 1], m_volume->voxels[index + 2],
                                             m_volume->voxels[index + 3]};
                }
            }
        }
        m_textures[dir].emplace_back(pixels.data(), GLTexture2D::TextureData::Color, sizeU, sizeV);
    }
}

size_t VolumeDataset::texelIndexInVolume(size_t x, size_t y, size_t z) {
    return x + y * m_volume->info.size.x + z * m_volume->info.size.x * m_volume->info.size.y;
}