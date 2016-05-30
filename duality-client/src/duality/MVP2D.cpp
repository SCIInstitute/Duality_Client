#include "src/duality/MVP2D.h"

#include <cmath>

using namespace IVDA;

MVP2D::MVP2D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox) {
    updateScreenInfo(screenInfo);
    updateBoundingBox(boundingBox);
}

GLMatrix MVP2D::calculate(const RenderParameters2D& parameters) const {
    const Mat3i svm = getSliceViewMatrix(parameters.axis());
    const Vec3i ex(1, 0, 0), ey(0, 1, 0);
    const Vec3i svex = svm * ex;
    const Vec3i svey = svm * ey;

    const Axis ax = duality::vectorToAxis(svex);
    const Axis ay = duality::vectorToAxis(svey);

    const Vec3f size3 = m_boundingBox.max - m_boundingBox.min;
    const Vec3f center3 = m_boundingBox.min + size3 / 2;

    const Vec2f size = Vec2f(size3[ax.axis], size3[ay.axis]);

    GLMatrix mvp;
    // move center of bbox to origin
    mvp.translate(-center3.x, -center3.y, -center3.z);

    // apply view direction and orientation
    mvp.multiply((GLMatrix)Mat4f(svm));

    // apply user interaction parameters
    mvp.translate(parameters.transation().x, parameters.transation().y, 0);
    // mvp.Scale(m_fZoom, m_fZoom, 1);
    mvp.rotate(-parameters.rotationAngle() * (180 / static_cast<float>(M_PI)), 0, 0, 1);

    // match object and screen aspects to fit the screen
    const float objectAspect = size.x / size.y;
    // const float sAspect = float(m_ScreenInfo.width) / float(m_ScreenInfo.height);
    if (objectAspect < m_screenAspect) {
        // norm y axis to two
        const float s = 2 / size.y;
        mvp.scale(s, s, 1);
        // stretch x axis to screen aspect
        mvp.scale(1 / m_screenAspect, 1, 1);
    } else {
        // norm x axis to two
        const float s = 2 / size.x;
        mvp.scale(s, s, 1);
        // stretch y axis to screen aspect
        mvp.scale(1, m_screenAspect, 1);
    }

    // setup projection parameters
    const auto min3 = m_boundingBox.min - center3;
    const auto max3 = m_boundingBox.max - center3;
    const float d = 0.1f; // some delta to make sure that our geometry does not get clipped away due to floating point precision errors
    switch (parameters.axis()) {
    case CoordinateAxis::X_Axis: {
        const float off = size3.x * d;
        const float min = min3.x - off;
        const float max = max3.x + off;
        mvp.ortho(-1, 1, -1, 1, min, max);
        break;
    }
    case CoordinateAxis::Y_Axis: {
        const float off = size3.y * d;
        const float min = min3.y - off;
        const float max = max3.y + off;
        mvp.ortho(-1, 1, -1, 1, min, max);
        break;
    }
    case CoordinateAxis::Z_Axis: {
        const float off = size3.z * d;
        const float min = min3.z - off;
        const float max = max3.z + off;
        mvp.ortho(-1, 1, -1, 1, -max, -min);
        break;
    }
    }
    return mvp;
}

void MVP2D::updateScreenInfo(const ScreenInfo& screenInfo) {
    m_screenAspect = static_cast<float>(screenInfo.width) / static_cast<float>(screenInfo.height);
}

void MVP2D::updateBoundingBox(const BoundingBox& boundingBox) {
    m_boundingBox = boundingBox;
}

Mat3i MVP2D::getSliceViewerBasis(const Axis viewerUp, const Axis viewerFace) {
    const auto viewerUpVec = duality::axisToVector(viewerUp);     // y
    const auto viewerFaceVec = duality::axisToVector(viewerFace); // z
    const auto viewerRightVec = viewerUpVec % viewerFaceVec;      // x

    std::array<Vec3i, 3> const viewerAxes = {viewerRightVec, viewerUpVec, viewerFaceVec};
    return Mat3i(viewerAxes.data()).Transpose();
}

Mat3i MVP2D::getSliceViewMatrix(const CoordinateAxis axis) {
    CoordinateSystem cs; // FIXME: default
    const Axis viewerFace(axis, true);

    const auto userUp = cs.mappedDirectionToAxis(cs.orientation.userUp);
    const auto userFace = cs.mappedDirectionToAxis(cs.orientation.userFace);

    if (userUp.axis != viewerFace.axis) {
        return getSliceViewerBasis(userUp, viewerFace);
    } else {
        return getSliceViewerBasis(userFace, viewerFace);
    }
}
