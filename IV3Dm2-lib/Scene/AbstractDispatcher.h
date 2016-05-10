#pragma once

#include "GroupNode.h"

class GeometryNode;

class AbstractDispatcher {
public:
    virtual void dispatch(GeometryNode& node) = 0;
    // virtual void dispatch(VolumeNode& node) = 0;
    virtual void dispatch(GroupNode& node) {
        for (auto& node : node.children()) {
            node->accept(*this);
        }
    }
};