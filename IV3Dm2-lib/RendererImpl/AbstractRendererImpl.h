#pragma once

#include "Scene/ScreenInfo.h"
#include "RendererImpl/GLFrameBufferObject.h"

class AbstractRendererImpl {
public:
    AbstractRendererImpl(const ScreenInfo& screenInfo);
    
    void startDraw();
    void finishDraw();
    
private:
    std::unique_ptr<GLFrameBufferObject> m_fbo;
};