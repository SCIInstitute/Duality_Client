#pragma once

#include "IVDA/Vectors.h"

#include <type_traits>

struct BoundingBox {
    IVDA::Vec3f min;
    IVDA::Vec3f max;
};

namespace duality {
namespace impl {
template <typename T> BoundingBox boundingBoxImpl(T obj, std::true_type) {
    return obj->boundingBox();
}

template <typename T> BoundingBox boundingBoxImpl(T obj, std::false_type) {
    return obj.boundingBox();
}
}

template <typename T> BoundingBox boundingBox(const std::vector<T>& objects) {
    IVDA::Vec3f vMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    IVDA::Vec3f vMax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (const auto& obj : objects) {
        BoundingBox bb = impl::boundingBoxImpl(obj, std::is_pointer<T>());
        vMin.StoreMin(bb.min);
        vMax.StoreMax(bb.max);
    }
    return BoundingBox{vMin, vMax};
}
}