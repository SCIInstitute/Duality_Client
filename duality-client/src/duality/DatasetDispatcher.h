#pragma once

class GeometryDataset;

class DatasetDispatcher {
public:
    virtual void dispatch(GeometryDataset& dataset) = 0;
    // virtual void dispatch(VolumeNode& dataset) = 0;
};