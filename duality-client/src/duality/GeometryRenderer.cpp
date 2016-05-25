#include "duality/GeometryRenderer.h"

#if __APPLE__
#include "TargetConditionals.h"
#endif

#if !__APPLE__ || !TARGET_OS_IPHONE
class GeometryRendererImpl {
    ~GeometryRendererImpl() = default;
};
#else
#include "duality/private/GeometryRendererImpl.h"
#endif

#if __APPLE__ && TARGET_OS_IPHONE
GeometryRenderer::GeometryRenderer(const ScreenInfo& screenInfo)
    : AbsstractRenderer(screenInfo)
    , m_impl(std::make_unique<GeometryRendererImpl>()) {}
#else
GeometryRenderer::GeometryRenderer(const ScreenInfo& screenInfo)
    : AbsstractRenderer(screenInfo)
    , m_impl(nullptr) {}
#endif

GeometryRenderer::~GeometryRenderer() = default;

void GeometryRenderer::render(const GeometryDataset& dataset, const GLMatrix& modelView) {
    GLMatrix mvp = modelView;
    mvp.multiply(projectionMatrix());
#if __APPLE__ && TARGET_OS_IPHONE
    m_impl->render(dataset, mvp);
#endif
}