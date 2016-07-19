#include "src/duality/BoundingBox.h"

bool operator==(const BoundingBox& lhs, const BoundingBox& rhs) {
    return lhs.min == rhs.min && lhs.max == rhs.max;
}

bool operator!=(const BoundingBox& lhs, const BoundingBox& rhs) {
    return !(lhs == rhs);
}
