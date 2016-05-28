//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

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
    
    void dispatch(GeometryDataset& node) override;
    // void dispatch(VolumeNode& node) override;

    BoundingBox boundingBox() const;
    
private:
    IVDA::Vec3f m_min;
    IVDA::Vec3f m_max;
};