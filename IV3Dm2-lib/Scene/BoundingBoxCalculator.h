//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "AbstractDispatcher.h"
#include "IVDA/Vectors.h"


class BoundingBoxCalculator : public AbstractDispatcher {
public:
    BoundingBoxCalculator();
    
    void dispatch(GeometryNode& node) override;
    // void dispatch(VolumeNode& node) override;

    std::pair<IVDA::Vec3f, IVDA::Vec3f> getMinMax() const;
    
private:
    IVDA::Vec3f m_min;
    IVDA::Vec3f m_max;
};