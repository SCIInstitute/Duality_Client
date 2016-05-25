//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "duality/DatasetDispatcher.h"
#include "IVDA/Vectors.h"


class BoundingBoxCalculator : public DatasetDispatcher {
public:
    BoundingBoxCalculator();
    
    void dispatch(GeometryDataset& node) override;
    // void dispatch(VolumeNode& node) override;

    std::pair<IVDA::Vec3f, IVDA::Vec3f> getMinMax() const;
    
private:
    IVDA::Vec3f m_min;
    IVDA::Vec3f m_max;
};