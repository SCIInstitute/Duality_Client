#pragma once

class GeometryDataset;

class AbstractRenderer {
public:
    virtual void render(const GeometryDataset& dataset) = 0;
};