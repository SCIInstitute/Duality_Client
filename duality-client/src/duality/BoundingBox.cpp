//  Created by David McCann on 5/10/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "duality/BoundingBox.h"
#include "duality/GeometryDataset.h"

#include <limits>
#include <cassert>

BoundingBoxCalculator::BoundingBoxCalculator() {
    m_min = IVDA::Vec3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_max = IVDA::Vec3f(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
}

void BoundingBoxCalculator::dispatch(GeometryDataset& node) {
    if (node.geometryInfo() == nullptr) return;
    assert(node.getPositions() != nullptr);
    for (size_t i = 0; i < node.geometryInfo()->numberIndices; ++i) {
        auto offset = 3 * node.getIndices()[i];
        const auto positions = node.getPositions();
        IVDA::Vec3f pos(positions[offset + 0], positions[offset + 1], positions[offset + 2]);
        m_min.StoreMin(pos);
        m_max.StoreMax(pos);
    }
}

BoundingBox BoundingBoxCalculator::boundingBox() const {
    return BoundingBox{m_min, m_max};
}
