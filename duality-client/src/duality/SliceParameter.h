#pragma once

#include "duality/CoordinateSystem.h"

#include <array>
#include <vector>

class Scene;

class SliderParameter {
public:
    explicit SliderParameter(int slice, float depth);
    
    int slice() const;
    float depth() const;

private:
    int m_slice;
    float m_depth;
};

class SliderParameterCalculator {
public:
    SliderParameterCalculator(const Scene& scene);
    
    bool supportsSlice() const;
    SliderParameter parameterForSlice(int slice, CoordinateAxis axis) const;
    SliderParameter parameterForDepth(float depth, CoordinateAxis axis) const;
    
private:
    bool m_supportsSlice;
    std::array<int, 3> m_numSlices;
    std::array<std::pair<float, float>, 3> m_bounds;
};