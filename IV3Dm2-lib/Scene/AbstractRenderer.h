#pragma once

#include "Scene/GLMatrix.h"
#include "Scene/ScreenInfo.h"

#include <memory>

#if __APPLE__
#include "TargetConditionals.h"
#endif

#if !__APPLE__ || !TARGET_OS_IPHONE
class AbsstractRendererImpl {
    ~AbsstractRendererImpl() = default;
};
#else
#include "RendererImpl/AbsstractRendererImpl.h"
#endif

class AbsstractRenderer {
public:
    AbsstractRenderer(ScreenInfo screenInfo);

    void startDraw();
    void finishDraw();

    const GLMatrix& projectionMatrix() const;

private:
    void updateProjectionMatrix();

private:
    std::unique_ptr<AbsstractRendererImpl> m_impl;
    ScreenInfo m_screenInfo;
    GLMatrix m_projection;
};