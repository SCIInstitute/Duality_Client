#include "Scene/GeometryRenderer.h"

#if __APPLE__ && TARGET_OS_IPHONE
GeometryRenderer::GeometryRenderer(const ScreenInfo& screenInfo)
    : AbsstractRenderer(screenInfo)
    , m_impl(std::make_unique<GeometryRendererImpl>()) {}
#else
GeometryRenderer::GeometryRenderer(const ScreenInfo& screenInfo)
    : AbsstractRenderer(screenInfo)
    , m_impl(nullptr) {}
#endif

void GeometryRenderer::render(const GeometryNode& dataset, const GLMatrix& modelView) {
    GLMatrix mvp = modelView;
    mvp.multiply(projectionMatrix());
#if __APPLE__ && TARGET_OS_IPHONE
    m_impl->render(dataset, mvp);
#endif
}