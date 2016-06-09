#pragma once

#include "IVDA/Vectors.h"
#include "src/duality/AbstractIO.h"

#include <array>
#include <vector>

class I3M {
public:
    struct VolumeInfo {
        IVDA::Vec3ui size;
        IVDA::Vec3f scale;
    };

    struct Volume {
        VolumeInfo info;
        std::vector<std::array<uint8_t, 4>> voxels;
    };

    static void read(AbstractReader& reader, Volume& volume);

private:
    static void readHeader(AbstractReader& reader, VolumeInfo& info);
};