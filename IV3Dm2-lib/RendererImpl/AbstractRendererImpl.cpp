#include "RendererImpl/AbstractRendererImpl.h"

#include "RendererImpl/GLInclude.h"

#include <OpenGLES/ES3/gl.h>

AbstractRendererImpl::AbstractRendererImpl(const ScreenInfo& screenInfo)
    : m_fbo(std::make_unique<GLFrameBufferObject>(static_cast<unsigned int>(screenInfo.width / screenInfo.standardDownSampleFactor),
                                                  static_cast<unsigned int>(screenInfo.height / screenInfo.standardDownSampleFactor),
                                                  true)) {}

void AbstractRendererImpl::startDraw() {
    GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void AbstractRendererImpl::finishDraw() {
    m_fbo->Read(0);
}