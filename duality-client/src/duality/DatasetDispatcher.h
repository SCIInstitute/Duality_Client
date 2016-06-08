#pragma once

class GeometryDataset;
class VolumeDataset;

class DatasetDispatcher {
public:
    virtual void dispatch(GeometryDataset& dataset) = 0;
    virtual void dispatch(VolumeDataset& dataset) {
        // FIXME: should be pure virtual
    }
};