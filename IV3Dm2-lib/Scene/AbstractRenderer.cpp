#include "Scene/AbstractRenderer.h"

#include <cmath>

AbsstractRenderer::AbsstractRenderer(ScreenInfo screenInfo)
    : m_screenInfo(screenInfo) {
    updateProjectionMatrix();
}

void AbsstractRenderer::startDraw() {
#if __APPLE__ && TARGET_OS_IPHONE
    m_impl->startDraw();
#endif
}

void AbsstractRenderer::finishDraw() {
#if __APPLE__ && TARGET_OS_IPHONE
    m_impl->finishDraw();
#endif
}

const GLMatrix& AbsstractRenderer::projectionMatrix() const {
    return m_projection;
}

void AbsstractRenderer::updateProjectionMatrix() {
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * tanf(3.1415f / 8.0f);
    float aspectRatio = float(m_screenInfo.height) / float(m_screenInfo.width);

    m_projection.loadIdentity();
    m_projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
}
