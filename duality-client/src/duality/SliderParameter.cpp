#include "src/duality/SliderParameter.h"

SliderParameter::SliderParameter(int slice, float depth)
    : m_hasSlice(true)
    , m_slice(slice)
    , m_depth(depth) {}

SliderParameter::SliderParameter(float depth)
    : m_hasSlice(false)
    , m_slice(-1)
    , m_depth(depth) {}

bool SliderParameter::hasSlice() const {
    return m_hasSlice;
}

int SliderParameter::slice() const {
    return m_slice;
}

float SliderParameter::depth() const {
    return m_depth;
}

bool operator==(const SliderParameter& lhs, const SliderParameter& rhs) {
    return lhs.slice() == rhs.slice() && lhs.depth() == rhs.depth();
}

bool operator!=(const SliderParameter& lhs, const SliderParameter& rhs) {
    return !(lhs == rhs);
}