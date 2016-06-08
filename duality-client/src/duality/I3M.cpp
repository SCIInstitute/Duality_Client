#include "src/duality/I3M.h"

#include "duality/Error.h"

#include "mocca/base/BinaryUtil.h"

#include <algorithm>

void I3M::read(AbstractReader& reader, Volume& volume) {
    readHeader(reader, volume.info);

    int32_t size = volume.info.size.volume();
    volume.voxels.resize(size);

    constexpr size_t chunkSize = 102400;
    size_t loopCount = size / chunkSize;
    for (size_t i = 0; i < loopCount; ++i) {
        int32_t chunkSizeBytes = chunkSize * 4;
        auto ptr = reinterpret_cast<char*>(&volume.voxels[i * chunkSize]);
        reader.read(ptr, chunkSizeBytes);
    }

    size_t lastChunkSize = size % chunkSize;
    if (lastChunkSize > 0) {
        size_t chunkSizeBytes = lastChunkSize * 4;
        auto ptr = reinterpret_cast<char*>(&volume.voxels[loopCount]);
        reader.read(ptr, chunkSizeBytes);
    }
}

void I3M::readHeader(AbstractReader& reader, VolumeInfo& info) {
    if (!reader.isOpen()) {
        throw Error("I3M reader not open", __FILE__, __LINE__);
    }

    int32_t headerLength = 5 * sizeof(int32_t) + 3 * sizeof(float);
    std::vector<uint8_t> buffer(headerLength);
    if (reader.read(reinterpret_cast<char*>(buffer.data()), headerLength) < headerLength) {
        throw Error("I3M header incomplete", __FILE__, __LINE__);
    }

    const uint8_t* readPtr = buffer.data();
    uint32_t magic;
    readPtr = mocca::binaryRead(readPtr, magic);
    if (magic != 69426942) {
        throw Error("I3M header invalid", __FILE__, __LINE__);
    }

    uint32_t version;
    readPtr = mocca::binaryRead(readPtr, version);
    if (version != 1) {
        throw Error("I3M invalid version", __FILE__, __LINE__);
    }

    readPtr = mocca::binaryRead(readPtr, info.size[0]);
    readPtr = mocca::binaryRead(readPtr, info.size[1]);
    readPtr = mocca::binaryRead(readPtr, info.size[2]);

    readPtr = mocca::binaryRead(readPtr, info.scale[0]);
    readPtr = mocca::binaryRead(readPtr, info.scale[1]);
    readPtr = mocca::binaryRead(readPtr, info.scale[2]);

    float maxScale = std::max({info.scale.x, info.scale.y, info.scale.z});
    info.scale /= maxScale;
}
