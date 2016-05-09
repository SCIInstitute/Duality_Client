#pragma once

#include "AbstractRenderer.h"

class GeometryRenderer : public AbstractRenderer {
public:
    void render(const GeometryDataset& dataset) override;
};