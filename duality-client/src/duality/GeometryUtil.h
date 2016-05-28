#pragma once

#include "src/duality/GeometryDataset.h"

class GeometryUtil {
public:
    enum Axis { AxisX = 0, AxisY = 1, AxisZ = 2 };
    static GeometryDataset clipGeometry(const GeometryDataset& geo, Axis axis, float position);

private:
    static bool planeIntersection(const float* triPositions, const float* triColors, Axis axis, float axisPosition, float* linePositions,
                                  float* lineColors, size_t& numberOfPoints);
};