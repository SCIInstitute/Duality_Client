#include "Scene/GeometryRenderer.h"

#if __APPLE__ && TARGET_OS_IPHONE
GeometryRenderer::GeometryRenderer(ScreenInfo screenInfo)
    : m_impl(std::make_unique<GeometryRendererImpl>()) {}
#else
GeometryRenderer::GeometryRenderer(ScreenInfo screenInfo)
    : m_impl(nullptr) {}
#endif

void GeometryRenderer::render(const GeometryNode& dataset) {
#if __APPLE__ && TARGET_OS_IPHONE
    m_impl->render(dataset);
#endif
}