//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "BoundingBoxCalculator.h"
#include "GeometryNode.h"

#include <limits>

BoundingBoxCalculator::BoundingBoxCalculator() {
    m_min = IVDA::Vec3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_max = IVDA::Vec3f(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
}

void BoundingBoxCalculator::dispatch(GeometryNode& node) {
    for (size_t i = 0; i < node.geometryInfo().numberIndices; ++i) {
        IVDA::Vec3f pos(node.getPositions()[3 * node.getIndices()[i] + 0], node.getPositions()[3 * node.getIndices()[i] + 1],
                        node.getPositions()[3 * node.getIndices()[i] + 2]);
        m_min.StoreMin(pos);
        m_max.StoreMax(pos);
    }
}