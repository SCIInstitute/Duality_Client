#pragma once

#include "duality/CoordinateSystem.h"
#include "src/duality/G3D.h"

class GeometryUtil {
public:
    static std::unique_ptr<G3D::GeometrySoA> clipGeometry(const G3D::GeometrySoA& geo, CoordinateAxis axis, float position);

private:
    static bool planeIntersection(const float* triPositions, const float* triColors, CoordinateAxis axis, float axisPosition, float* linePositions, float* lineColors, size_t& numberOfPoints);
};