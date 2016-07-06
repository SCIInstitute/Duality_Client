#pragma once

#include <vector>

class VolumeNode;
class GeometryNode;

class InterleavingRenderer3D {
public:
    void render(VolumeNode* volumeNode, const std::vector<GeometryNode*> geometryNodes);
};