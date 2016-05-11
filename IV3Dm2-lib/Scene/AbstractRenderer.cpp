#include "Scene/AbstractRenderer.h"

#include <cmath>

AbsstractRenderer::AbsstractRenderer(ScreenInfo screenInfo)
    : m_screenInfo(screenInfo) {
    updateProjectionMatrix();
}

const GLMatrix& AbsstractRenderer::projectionMatrix() const {
    return m_projection;
}

void AbsstractRenderer::updateProjectionMatrix() {
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * (float)tanf(3.1415 / 8.0);
    float aspectRatio = float(m_screenInfo.height) / float(m_screenInfo.width);

    m_projection.loadIdentity();
    m_projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
}
