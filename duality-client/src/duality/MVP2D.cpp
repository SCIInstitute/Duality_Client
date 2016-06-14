#include "src/duality/MVP2D.h"

#include <cmath>

using namespace IVDA;

MVP2D::MVP2D(const ScreenInfo& screenInfo, const BoundingBox& boundingBox, const RenderParameters2D& initialParameters)
    : m_screenAspect(static_cast<float>(screenInfo.width) / static_cast<float>(screenInfo.height))
    , m_boundingBox(boundingBox) {
    updateParameters(initialParameters);
}

void MVP2D::updateParameters(const RenderParameters2D& parameters) {
    const Mat3i svm = getSliceViewMatrix(parameters.axis());
    const Vec3i ex(1, 0, 0), ey(0, 1, 0);
    const Vec3i svex = svm * ex;
    const Vec3i svey = svm * ey;

    const Axis ax = duality::vectorToAxis(svex);
    const Axis ay = duality::vectorToAxis(svey);

    const Vec3f size3 = m_boundingBox.max - m_boundingBox.min;
    const Vec3f center3 = m_boundingBox.min + size3 / 2;

    const Vec2f size = Vec2f(size3[ax.axis], size3[ay.axis]);

    m_mvp.loadIdentity();
    // move center of bbox to origin
    m_mvp.translate(-center3.x, -center3.y, -center3.z);

    // apply view direction and orientation
    m_mvp.multiply((GLMatrix)Mat4f(svm));

    // apply user interaction parameters
    m_mvp.translate(parameters.transation().x, parameters.transation().y, 0);
    m_mvp.scale(parameters.zoom(), parameters.zoom(), 1);
    m_mvp.rotate(-parameters.rotationAngle() * (180 / static_cast<float>(M_PI)), 0, 0, 1);

    // match object and screen aspects to fit the screen
    const float objectAspect = size.x / size.y;
    // const float sAspect = float(m_ScreenInfo.width) / float(m_ScreenInfo.height);
    if (objectAspect < m_screenAspect) {
        // norm y axis to two
        const float s = 2 / size.y;
        m_mvp.scale(s, s, 1);
        // stretch x axis to screen aspect
        m_mvp.scale(1 / m_screenAspect, 1, 1);
    } else {
        // norm x axis to two
        const float s = 2 / size.x;
        m_mvp.scale(s, s, 1);
        // stretch y axis to screen aspect
        m_mvp.scale(1, m_screenAspect, 1);
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
        m_mvp.ortho(-1, 1, -1, 1, min, max);
        break;
    }
    case CoordinateAxis::Y_Axis: {
        const float off = size3.y * d;
        const float min = min3.y - off;
        const float max = max3.y + off;
        m_mvp.ortho(-1, 1, -1, 1, min, max);
        break;
    }
    case CoordinateAxis::Z_Axis: {
        const float off = size3.z * d;
        const float min = min3.z - off;
        const float max = max3.z + off;
        m_mvp.ortho(-1, 1, -1, 1, -max, -min);
        break;
    }
    }
}

const GLMatrix& MVP2D::mvp() const {
    return m_mvp;
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
