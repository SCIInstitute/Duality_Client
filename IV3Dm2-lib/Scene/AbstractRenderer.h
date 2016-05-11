#pragma once

#include "Scene/GLMatrix.h"
#include "Scene/ScreenInfo.h"

class AbsstractRenderer {
public:
    AbsstractRenderer(ScreenInfo screenInfo);

    const GLMatrix& projectionMatrix() const;

private:
    void updateProjectionMatrix();

private:
    ScreenInfo m_screenInfo;
    GLMatrix m_projection;
};