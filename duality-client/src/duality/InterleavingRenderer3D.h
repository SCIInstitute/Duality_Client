#pragma once

#include <memory>
#include <vector>

class VolumeDataset;
class GeometryDataset;
class GeometryRenderer3D;
class VolumeRenderer3D;
class MVP3D;
class TransferFunction;

class InterleavingRenderer3D {
public:
    InterleavingRenderer3D();
    ~InterleavingRenderer3D();

    void render(const VolumeDataset& volumeDataset, const std::vector<const GeometryDataset*>& geometryDatasets, const MVP3D& mvp,
                const TransferFunction& tf);

private:
    std::unique_ptr<GeometryRenderer3D> m_geoRenderer;
    std::unique_ptr<VolumeRenderer3D> m_volRenderer;
};