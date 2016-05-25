#pragma once

#include "duality/GLMatrix.h"
#include "duality/ScreenInfo.h"

#include <memory>

class AbstractRendererImpl;

class AbsstractRenderer {
public:
    AbsstractRenderer(const ScreenInfo& screenInfo);
    ~AbsstractRenderer();

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