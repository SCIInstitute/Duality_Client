#pragma once

class GeometryDataset;

class DatasetDispatcher {
public:
    virtual void setup() {}
    virtual void finish() {}
    virtual void dispatch(GeometryDataset& dataset) = 0;
    // virtual void dispatch(VolumeNode& dataset) = 0;
};