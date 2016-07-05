#include "src/duality/BoundingBox.h"

#include "src/duality/GeometryNode.h"
#include "src/duality/VolumeNode.h"

#include <limits>
#include <cassert>

BoundingBoxCalculator::BoundingBoxCalculator() {
    m_min = IVDA::Vec3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_max = IVDA::Vec3f(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
}

void BoundingBoxCalculator::dispatch(GeometryNode& node) {
    const auto& dataset = node.dataset();
    BoundingBox bb = dataset.boundingBox();
    m_min.StoreMin(bb.min);
    m_max.StoreMax(bb.max);
}

void BoundingBoxCalculator::dispatch(VolumeNode& node) {
    const auto& dataset = node.dataset();
    BoundingBox bb = dataset.boundingBox();
    m_min.StoreMin(bb.min);
    m_max.StoreMax(bb.max);
}

BoundingBox BoundingBoxCalculator::boundingBox() const {
    return BoundingBox{m_min, m_max};
}
