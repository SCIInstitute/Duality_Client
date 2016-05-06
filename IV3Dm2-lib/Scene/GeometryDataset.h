#pragma once

#include "Dataset.h"

class GeometryDataset : public Dataset {
public:
    GeometryDataset(const std::vector<uint8_t>& binaryData);

    // virtual void render(Renderer& renderer) const override;
    std::unique_ptr<Dataset> clone() override;
};