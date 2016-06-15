#pragma once

class GeometryNode;
class VolumeNode;

class NodeDispatcher {
public:
    virtual void dispatch(GeometryNode& dataset) = 0;
    virtual void dispatch(VolumeNode& dataset) = 0;
};