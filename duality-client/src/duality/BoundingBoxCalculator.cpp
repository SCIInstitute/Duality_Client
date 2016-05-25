//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/BoundingBoxCalculator.h"
#include "duality/GeometryDataset.h"

#include <limits>

BoundingBoxCalculator::BoundingBoxCalculator() {
    m_min = IVDA::Vec3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_max = IVDA::Vec3f(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
}

void BoundingBoxCalculator::dispatch(GeometryDataset& node) {
    if (node.geometryInfo() == nullptr) return;
    for (size_t i = 0; i < node.geometryInfo()->numberIndices; ++i) {
        auto offset = 3 * node.getIndices()[i];
        IVDA::Vec3f pos(node.getPositions()[offset + 0], node.getPositions()[offset + 1], node.getPositions()[offset + 2]);
        m_min.StoreMin(pos);
        m_max.StoreMax(pos);
    }
}

std::pair<IVDA::Vec3f, IVDA::Vec3f> BoundingBoxCalculator::getMinMax() const {
    return std::make_pair(m_min, m_max);
}
