#include "Dataset.h"

#include "GeometryDataset.h"

std::unique_ptr<Dataset> Dataset::create(const std::vector<uint8_t>& binaryData) {
    return std::make_unique<GeometryDataset>(binaryData);
}
