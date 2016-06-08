#pragma once

#include "IVDA/Vectors.h"
#include "src/duality/AbstractIO.h"

class I3M {
public:
    struct VolumeInfo {
        IVDA::Vec3ui size;
        IVDA::Vec3f scale;
    };

    struct Volume {
        VolumeInfo info;
        std::vector<uint32_t> voxels;
    };

    void read(AbstractReader& reader, Volume& volume);

private:
    void readHeader(AbstractReader& reader, VolumeInfo& info);
};