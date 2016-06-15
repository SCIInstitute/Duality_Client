#pragma once

#include "src/duality/NodeDispatcher.h"
#include "IVDA/Vectors.h"

struct BoundingBox {
    IVDA::Vec3f min;
    IVDA::Vec3f max;
};

class BoundingBoxCalculator : public NodeDispatcher {
public:
    BoundingBoxCalculator();
    
    void dispatch(GeometryNode& node) override;
    void dispatch(VolumeNode& node) override;

    BoundingBox boundingBox() const;
    
private:
    IVDA::Vec3f m_min;
    IVDA::Vec3f m_max;
};