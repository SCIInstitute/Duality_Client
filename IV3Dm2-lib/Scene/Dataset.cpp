#include "Dataset.h"

#include "GeometryDataset.h"
#include "AbstractIO.h"

std::unique_ptr<Dataset> Dataset::create(const std::vector<uint8_t>& binaryData) {
    ReaderFromMemory reader(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    return std::make_unique<GeometryDataset>(reader);
}
