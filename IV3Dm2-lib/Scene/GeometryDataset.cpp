#include "GeometryDataset.h"

GeometryDataset::GeometryDataset(const std::vector<uint8_t>& binaryData)
{
    // TODO: implement
}

std::unique_ptr<Dataset> GeometryDataset::clone() {
    return std::make_unique<GeometryDataset>(*this);
}
