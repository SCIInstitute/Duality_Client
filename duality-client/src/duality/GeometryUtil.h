#pragma once

#include "duality/CoordinateSystem.h"
#include "src/duality/GeometryDataset.h"

class GeometryUtil {
public:
    static GeometryDataset clipGeometry(const GeometryDataset& geo, CoordinateAxis axis, float position);

private:
    static bool planeIntersection(const float* triPositions, const float* triColors, CoordinateAxis axis, float axisPosition, float* linePositions, float* lineColors, size_t& numberOfPoints);
};