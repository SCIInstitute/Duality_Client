#pragma once

#include "duality/GLMatrix.h"
#include "duality/ScreenInfo.h"

#include <memory>

#if __APPLE__
#include "TargetConditionals.h"
#endif

#if !__APPLE__ || !TARGET_OS_IPHONE
class AbstractRendererImpl {
public:
    AbstractRendererImpl(const ScreenInfo&) {}
    ~AbstractRendererImpl() = default;
};
#else
#include "RendererImpl/AbstractRendererImpl.h"
#endif

class AbsstractRenderer {
public:
    AbsstractRenderer(const ScreenInfo& screenInfo);

    void startDraw();
    void finishDraw();

    const GLMatrix& projectionMatrix() const;

private:
    void updateProjectionMatrix();

private:
    std::unique_ptr<AbstractRendererImpl> m_impl;
    ScreenInfo m_screenInfo;
    GLMatrix m_projection;
};