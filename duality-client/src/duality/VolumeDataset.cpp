#include "src/duality/AbstractIO.h"
#include "src/duality/VolumeDataset.h"

#include <algorithm>
#include <cmath>

VolumeDataset::VolumeDataset(Dataset::Visibility visibility)
    : Dataset({}, visibility) {}

void VolumeDataset::accept(DatasetDispatcher& dispatcher) {
    dispatcher.dispatch(*this);
}

const std::array<std::vector<VolumeDataset::SliceInfo>, 3>& VolumeDataset::sliceInfos() const {
    return m_sliceInfos;
}

BoundingBox VolumeDataset::boundingBox() const {
    return BoundingBox{-0.5f * m_volume->info().scale, 0.5f * m_volume->info().scale};
}

void VolumeDataset::bindTextures(size_t dir, size_t slice) const {
    m_tf->bindWithUnit(0);
    m_textures[dir][slice].bindWithUnit(1);
}

void VolumeDataset::read(std::shared_ptr<std::vector<uint8_t>> data) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(data->data()), data->size());
    m_volume = std::make_unique<I3M::Volume>();
    I3M::read(reader, *m_volume);
    initTransferFunction(duality::defaultTransferFunction());
    initSliceInfos(m_volume->info);
    initTextures();
}

void VolumeDataset::applyTransform(const IVDA::Mat4f& matrix) {
    // FIXME: how to apply transforms to volumetric dataset?
}

void VolumeDataset::initSliceInfos(const I3M::VolumeInfo& volumeInfo) {
    BoundingBox bb = boundingBox();
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
                    pixels[v * sizeU + u] = {m_volume->voxels[index][0], m_volume->voxels[index][1], m_volume->voxels[index][2],
                                             m_volume->voxels[index][3]};
                }
            }
        }
        m_textures[dir].emplace_back(pixels.data(), GLTexture2D::TextureData::Color, sizeU, sizeV);
    }
}

size_t VolumeDataset::texelIndexInVolume(size_t x, size_t y, size_t z) {
    return x + y * m_volume->info.size.x + z * m_volume->info.size.x * m_volume->info.size.y;
}

void VolumeDataset::initTransferFunction(const TransferFunction& tf) {
    // apply opacity correction
    TransferFunction correctedTf = tf;
    float quality = 1.0f;
    for (int i = 0; i < 256; i++) {
        double alpha = correctedTf[i][3] / 255.0;
        alpha = 1.0 - std::pow(1.0 - alpha, 1.0 / quality);
        correctedTf[i][3] = static_cast<uint8_t>(255.0 * alpha);
    }
    m_tf = std::make_unique<GLTexture2D>(correctedTf.data(), GLTexture2D::TextureData::Color, 256, 1);
}
