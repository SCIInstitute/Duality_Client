#pragma once

class GeometryNode;

class AbstractDispatcher {
public:
    virtual void setup() {}
    virtual void finish() {}
    virtual void dispatch(GeometryNode& node) = 0;
    // virtual void dispatch(VolumeNode& node) = 0;
};