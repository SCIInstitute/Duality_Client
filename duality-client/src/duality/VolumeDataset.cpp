#include "src/duality/AbstractIO.h"
#include "src/duality/VolumeDataset.h"

#include <algorithm>
#include <cmath>

VolumeDataset::VolumeDataset(std::unique_ptr<DataProvider> provider)
    : Dataset(std::move(provider)) {}

void VolumeDataset::accept(NodeDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

const std::array<std::vector<VolumeDataset::SliceInfo>, 3>& VolumeDataset::sliceInfos() const {
    return m_sliceInfos;
}

BoundingBox VolumeDataset::boundingBox() const {
    return BoundingBox{-0.5f * m_volume->info.scale, 0.5f * m_volume->info.scale};
}

void VolumeDataset::bindTextures(size_t dir, size_t texIndex1, size_t texIndex2) const {
    m_textures[dir][texIndex1]->bindWithUnit(1);
    m_textures[dir][texIndex2]->bindWithUnit(2);
}

void VolumeDataset::readData(const std::vector<uint8_t>& data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data.data()), data.size());
    m_volume = std::make_unique<I3M::Volume>();
    I3M::read(reader, *m_volume);
    initSliceInfos();
    initTextures();
}

void VolumeDataset::initSliceInfos() {
    BoundingBox bb = boundingBox();
    const auto& volumeInfo = m_volume->info;
    for (size_t dir = 0; dir < 3; ++dir) {
        for (size_t i = 0; i < volumeInfo.size[dir]; ++i) {
            float normalizedPosInStack = static_cast<float>(i) / static_cast<float>(volumeInfo.size[dir] - 1);
            float depth = bb.min[dir] * (1.0f - normalizedPosInStack) + bb.max[dir] * normalizedPosInStack;
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
        int sizeU = 0, sizeV = 0, sizeW = 0;
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
                    pixels[v * sizeU + u] = m_volume->voxels[index];
                }
            }
            m_textures[dir].push_back(std::make_unique<GLTexture2D>(pixels.data(), GLTexture2D::TextureData::Color, sizeU, sizeV));
        }
    }
}

size_t VolumeDataset::texelIndexInVolume(size_t x, size_t y, size_t z) {
    return x + y * m_volume->info.size.x + z * m_volume->info.size.x * m_volume->info.size.y;
}