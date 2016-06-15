#pragma once

#include "src/duality/G3D.h"

#include "src/duality/Dataset.h"

#include "IVDA/GLMatrix.h"
#include "IVDA/Vectors.h"

class DataProvider;
class NodeDispatcher;

class GeometryDataset : public Dataset {
public:
    GeometryDataset(std::unique_ptr<DataProvider> provider, std::vector<IVDA::Mat4f> transforms = {});

    const G3D::GeometrySoA& geometry() const;

private:
    void readData(const std::vector<uint8_t>& data) override;

private:
    std::unique_ptr<G3D::GeometrySoA> m_geometry;
    std::vector<IVDA::Mat4f> m_transforms;

};