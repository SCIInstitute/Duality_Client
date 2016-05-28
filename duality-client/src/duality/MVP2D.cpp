#include "src/duality/MVP2D.h"

#include "src/duality/CoordinateSystem.h"

#include <cmath>

using namespace IVDA;

MVP2D::MVP2D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox)
    : m_screenAspect(static_cast<float>(screenInfo.width) / static_cast<float>(screenInfo.height))
    , m_boundingBox(boundingBox)
    , m_translation(0.0f, 0.0f)
    , m_rotationAngle(0.0f) {}

void MVP2D::addTranslation(const Vec2f& translation) {}

void MVP2D::addRotation(float angle) {}

GLMatrix MVP2D::calculate() const {
    GLMatrix mvp;

    int axis = 2; // FIXME!!!

    // Vec3f const scale3  = m_vScale;
    const Vec3f size3 = m_boundingBox.max - m_boundingBox.min;
    const Vec3f center3 = m_boundingBox.min + size3 / 2;

    const Mat3i svm = getSliceViewMatrix();

    const Vec3i ex(1, 0, 0), ey(0, 1, 0);
    const Vec3i svex = svm * ex;
    const Vec3i svey = svm * ey;

    const Axis ax = duality::vectorToAxis(svex);
    const Axis ay = duality::vectorToAxis(svey);

    // const Vec2f scale = Vec2f(scale3[ax.axis], scale3[ay.axis]);
    const Vec2f size = Vec2f(size3[ax.axis], size3[ay.axis]);

    // move center of bbox to origin
    mvp.translate(-center3.x, -center3.y, -center3.z);

    // apply view direction and orientation
    mvp.multiply((GLMatrix)Mat4f(svm));

    // apply volume scale if there is any
    // if (!bGeometry) {
    //    mvp.Scale(scale.x, scale.y, 1);
    //}

    // apply user interaction parameters
    mvp.translate(m_translation.x, m_translation.y, 0);
    // mvp.Scale(m_fZoom, m_fZoom, 1);
    mvp.rotate(-m_rotationAngle * (180 / static_cast<float>(M_PI)), 0, 0, 1);

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
    switch (axis) {
    case 0: {
        const float off = size3.x * d;
        const float min = min3.x - off;
        const float max = max3.x + off;
        mvp.ortho(-1, 1, -1, 1, min, max);
        break;
    }
    case 1: {
        const float off = size3.y * d;
        const float min = min3.y - off;
        const float max = max3.y + off;
        mvp.ortho(-1, 1, -1, 1, min, max);
        break;
    }
    case 2: {
        const float off = size3.z * d;
        const float min = min3.z - off;
        const float max = max3.z + off;
        mvp.ortho(-1, 1, -1, 1, -max, -min);
        break;
    }
    }
    return mvp;
}

Mat3i MVP2D::getSliceViewerBasis(const Axis viewerUp, const Axis viewerFace) {
    const auto viewerUpVec = duality::axisToVector(viewerUp);     // y
    const auto viewerFaceVec = duality::axisToVector(viewerFace); // z
    const auto viewerRightVec = viewerUpVec % viewerFaceVec;      // x

    std::array<Vec3i, 3> const viewerAxes = {viewerRightVec, viewerUpVec, viewerFaceVec};
    return Mat3i(viewerAxes.data()).Transpose();
}

Mat3i MVP2D::getSliceViewMatrix() {
    CoordinateSystem cs;                                  // FIXME: default
    const Axis viewerFace(CoordinateAxis::Z_Axis, true); // FIXME: default

    const auto userUp = cs.mappedDirectionToAxis(cs.orientation.userUp);
    const auto userFace = cs.mappedDirectionToAxis(cs.orientation.userFace);

    if (userUp.axis != viewerFace.axis) {
        return getSliceViewerBasis(userUp, viewerFace);
    } else {
        return getSliceViewerBasis(userFace, viewerFace);
    }
}
