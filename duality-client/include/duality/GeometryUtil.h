#pragma once

#include "duality/GeometryDataset.h"

class GeometryUtil {
public:
    enum Axis { AxisX, AxisY, AxisZ };
    static GeometryDataset clipGeometry(const GeometryDataset& geo, Axis axis, float position);

private:
    static bool planeIntersection(const float* triPositions, const float* triColors, Axis axis, float axisPosition, float* linePositions,
                                  float* lineColors, size_t& numberOfPoints);
};