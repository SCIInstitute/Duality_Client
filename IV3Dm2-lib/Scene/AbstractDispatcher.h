#pragma once

class GeometryNode;

class AbstractDispatcher {
public:
    virtual void dispatch(GeometryNode& node) = 0;
    // virtual void dispatch(VolumeNode& node) = 0;
};