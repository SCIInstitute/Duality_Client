#include "src/duality/GeometryUtil.h"

std::unique_ptr<G3D::GeometrySoA> GeometryUtil::clipGeometry(const G3D::GeometrySoA& geo, CoordinateAxis axis, float position) {
    // in planeIntersection
    float inPositions[9]; // xyz for 3 vertices of the given triangle
    float inColors[12];   // RGBA for 3 vertices of the given triangle
    // out planeIntersection
    float outPositions[9]; // max 3 lines
    float outColors[12];   // max 3 lines
    size_t points;         // number of intersection points

    const std::vector<uint32_t>& is = geo.indices;
    const float* ps = geo.positions;
    const float* cs = geo.colors;

    // resulting lines
    std::vector<uint32_t> clipIndices;
    std::vector<float> clipPositions;
    std::vector<float> clipColors;

    for (int i = 0; i < geo.info.numberIndices; i += 3) {
        // assemble in parameters for plane intersection
        for (int j = 0; j < 3; j++) // copy all three vertices per triangle
        {
            size_t const ip = is[i + j] * 3;
            size_t const j3 = j * 3;
            inPositions[j3] = ps[ip];
            inPositions[j3 + 1] = ps[ip + 1];
            inPositions[j3 + 2] = ps[ip + 2];

            size_t const ic = is[i + j] * 4;
            size_t const j4 = j * 4;
            inColors[j4] = cs[ic];
            inColors[j4 + 1] = cs[ic + 1];
            inColors[j4 + 2] = cs[ic + 2];
            inColors[j4 + 3] = cs[ic + 3];
        }

        if (planeIntersection(inPositions, inColors, axis, position, outPositions, outColors, points)) {
            if (points >= 2) {
                for (int j = 0; j < 6; j++) // 2 vertices
                {
                    clipPositions.push_back(outPositions[j]);
                }
                for (int j = 0; j < 8; j++) // 2 colors
                {
                    clipColors.push_back(outColors[j]);
                }
                // add indices for last two points
                clipIndices.push_back(static_cast<uint32_t>(clipPositions.size()) / 3 - 2);
                clipIndices.push_back(static_cast<uint32_t>(clipPositions.size()) / 3 - 1);

                // add third point if the given triangle happens to lie in the plane
                if (points == 3) {
                    for (int j = 0; j < 3; j++) {
                        clipPositions.push_back(outPositions[j + 6]);
                    }
                    for (int j = 0; j < 4; j++) {
                        clipColors.push_back(outColors[j + 8]);
                    }
                    // add the other two lines because we have a triangle and not just a line
                    clipIndices.push_back(static_cast<uint32_t>(clipPositions.size()) / 3 - 2);
                    clipIndices.push_back(static_cast<uint32_t>(clipPositions.size()) / 3 - 1);
                    clipIndices.push_back(static_cast<uint32_t>(clipPositions.size()) / 3 - 3);
                    clipIndices.push_back(static_cast<uint32_t>(clipPositions.size()) / 3 - 1);
                }
            }
        }
    }
    auto lines = G3D::createLineGeometry(std::move(clipIndices), std::move(clipPositions), std::move(clipColors));
    return lines;
}

bool GeometryUtil::planeIntersection(const float* triPositions, const float* triColors, CoordinateAxis axis, float axisPosition, float* linePositions, float* lineColors, size_t& numberOfPoints) {
    bool intersection = false;

    // distance clipping plane to vertices per component
    float const distA = axisPosition - triPositions[axis];
    float const distB = axisPosition - triPositions[axis + 3];
    float const distC = axisPosition - triPositions[axis + 6];

    int const otherAxis1 = axis == 0 ? 1 : 0;
    int const otherAxis2 = axis == 2 ? 1 : 2;
    numberOfPoints = distA == 0 + distB == 0 + distC == 0;

    if (numberOfPoints == 3) {
        for (int i = 0; i < numberOfPoints * G3D::floats(G3D::AttributeSemantic::Position); i++) {
            linePositions[i] = triPositions[i];
        }
        for (int i = 0; i < numberOfPoints * G3D::floats(G3D::AttributeSemantic::Color); i++) {
            lineColors[i] = triColors[i];
        }
        intersection = true;
    } else {
        float const absDistA = std::abs(distA);
        float const absDistB = std::abs(distB);
        float const absDistC = std::abs(distC);

        // if dist 0 and other points below 0 we do not create a line
        numberOfPoints = 0;
        if (((distA > 0) ^ (distB > 0)) || ((distA < 0) ^ (distB < 0))) {
            size_t const numberOfPoints3 = numberOfPoints * G3D::floats(G3D::AttributeSemantic::Position);
            linePositions[numberOfPoints3 + axis] = axisPosition;
            linePositions[numberOfPoints3 + otherAxis1] = triPositions[otherAxis1] + (distA / (triPositions[axis] - triPositions[axis + 3]) * (triPositions[otherAxis1] - triPositions[otherAxis1 + 3]));
            linePositions[numberOfPoints3 + otherAxis2] = triPositions[otherAxis2] + (distA / (triPositions[axis] - triPositions[axis + 3]) * (triPositions[otherAxis2] - triPositions[otherAxis2 + 3]));

            float const norm = absDistA + absDistB;
            size_t const numberOfPoints4 = numberOfPoints * G3D::floats(G3D::AttributeSemantic::Color);
            lineColors[numberOfPoints4] = (triColors[0] * absDistB + triColors[4] * absDistA) / norm;
            lineColors[numberOfPoints4 + 1] = (triColors[1] * absDistB + triColors[5] * absDistA) / norm;
            lineColors[numberOfPoints4 + 2] = (triColors[2] * absDistB + triColors[6] * absDistA) / norm;
            lineColors[numberOfPoints4 + 3] = (triColors[3] * absDistB + triColors[7] * absDistA) / norm;

            numberOfPoints++;
            intersection = true;
        }
        if (((distA > 0) ^ (distC > 0)) || ((distA < 0) ^ (distC < 0))) {
            size_t const numberOfPoints3 = numberOfPoints * G3D::floats(G3D::AttributeSemantic::Position);
            linePositions[numberOfPoints3 + axis] = axisPosition;
            linePositions[numberOfPoints3 + otherAxis1] = triPositions[otherAxis1] + (distA / (triPositions[axis] - triPositions[axis + 6]) * (triPositions[otherAxis1] - triPositions[otherAxis1 + 6]));
            linePositions[numberOfPoints3 + otherAxis2] = triPositions[otherAxis2] + (distA / (triPositions[axis] - triPositions[axis + 6]) * (triPositions[otherAxis2] - triPositions[otherAxis2 + 6]));

            float const norm = absDistA + absDistC;
            size_t const numberOfPoints4 = numberOfPoints * G3D::floats(G3D::AttributeSemantic::Color);
            lineColors[numberOfPoints4] = (triColors[0] * absDistC + triColors[8] * absDistA) / norm;
            lineColors[numberOfPoints4 + 1] = (triColors[1] * absDistC + triColors[9] * absDistA) / norm;
            lineColors[numberOfPoints4 + 2] = (triColors[2] * absDistC + triColors[10] * absDistA) / norm;
            lineColors[numberOfPoints4 + 3] = (triColors[3] * absDistC + triColors[11] * absDistA) / norm;

            numberOfPoints++;
            intersection = true;
        }
        if (((distB > 0) ^ (distC > 0)) || ((distB < 0) ^ (distC < 0))) {
            size_t const numberOfPoints3 = numberOfPoints * G3D::floats(G3D::AttributeSemantic::Position);
            linePositions[numberOfPoints3 + axis] = axisPosition;
            linePositions[numberOfPoints3 + otherAxis1] =
                triPositions[otherAxis1 + 3] + (distB / (triPositions[axis + 3] - triPositions[axis + 6]) * (triPositions[otherAxis1 + 3] - triPositions[otherAxis1 + 6]));
            linePositions[numberOfPoints3 + otherAxis2] =
                triPositions[otherAxis2 + 3] + (distB / (triPositions[axis + 3] - triPositions[axis + 6]) * (triPositions[otherAxis2 + 3] - triPositions[otherAxis2 + 6]));

            float const norm = absDistB + absDistC;
            size_t const numberOfPoints4 = numberOfPoints * G3D::floats(G3D::AttributeSemantic::Color);
            lineColors[numberOfPoints4] = (triColors[4] * absDistC + triColors[8] * absDistB) / norm;
            lineColors[numberOfPoints4 + 1] = (triColors[5] * absDistC + triColors[9] * absDistB) / norm;
            lineColors[numberOfPoints4 + 2] = (triColors[6] * absDistC + triColors[10] * absDistB) / norm;
            lineColors[numberOfPoints4 + 3] = (triColors[7] * absDistC + triColors[11] * absDistB) / norm;

            numberOfPoints++;
            intersection = true;
        }
    }
    return intersection;
}