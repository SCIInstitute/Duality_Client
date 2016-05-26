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
    : m_projection(createProjectionMatrix(screenInfo))
    , m_impl(std::make_unique<GeometryRendererImpl>()) {}
#else
GeometryRenderer::GeometryRenderer(const ScreenInfo& screenInfo)
    : AbsstractRenderer(screenInfo)
    , m_impl(nullptr) {}
#endif

GeometryRenderer::~GeometryRenderer() = default;

void GeometryRenderer::render(const GeometryDataset& dataset, const GLMatrix& modelView) {
    GLMatrix mvp = modelView;
    mvp.multiply(m_projection);
#if __APPLE__ && TARGET_OS_IPHONE
    m_impl->render(dataset, mvp);
#endif
}

GLMatrix GeometryRenderer::createProjectionMatrix(const ScreenInfo& screenInfo) {
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * tanf(3.1415f / 8.0f);
    float aspectRatio = float(screenInfo.height) / float(screenInfo.width);

    GLMatrix projection;
    projection.loadIdentity();
    projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
    return projection;
}