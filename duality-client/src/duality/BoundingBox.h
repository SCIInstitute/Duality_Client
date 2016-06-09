#pragma once

#include "src/duality/DatasetDispatcher.h"
#include "IVDA/Vectors.h"

struct BoundingBox {
    IVDA::Vec3f min;
    IVDA::Vec3f max;
};

class BoundingBoxCalculator : public DatasetDispatcher {
public:
    BoundingBoxCalculator();
    
    void dispatch(GeometryDataset& dataset) override;
    void dispatch(VolumeDataset& dataset) override;

    BoundingBox boundingBox() const;
    
private:
    IVDA::Vec3f m_min;
    IVDA::Vec3f m_max;
};