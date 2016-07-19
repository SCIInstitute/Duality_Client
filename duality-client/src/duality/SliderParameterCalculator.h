#pragma once

#include "duality/CoordinateSystem.h"
#include "src/duality/SliderParameter.h"

#include <array>

class Scene;

class SliderParameterCalculator {
public:
    SliderParameterCalculator(const Scene& scene);

    bool supportsSlice() const;
    SliderParameter parameterForSlice(int slice, CoordinateAxis axis) const;
    SliderParameter parameterForDepth(float depth, CoordinateAxis axis) const;

    int numSlicesForAxis(CoordinateAxis axis) const;
    std::pair<float, float> boundsForAxis(CoordinateAxis axis) const;
    
private:
    bool m_supportsSlice;
    std::array<int, 3> m_numSlices;
    std::array<std::pair<float, float>, 3> m_bounds;
};