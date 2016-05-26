#include "duality/GeometrySliceRenderer.h"

#include "duality/private/GLShader.h"
#include "duality/GeometryUtil.h"

GeometrySliceRenderer::GeometrySliceRenderer(const ScreenInfo& screenInfo)
    : m_projection(createProjectionMatrix(screenInfo)) {
    GlShaderAttributes attributes;
    attributes.push_back("position");
    attributes.push_back("color");
    m_shader = std::make_unique<GLShader>("geo_pc.vsh", "geo_pc.fsh", attributes);
}

GeometrySliceRenderer::~GeometrySliceRenderer() = default;

void GeometrySliceRenderer::render(const GeometryDataset& dataset, const GLMatrix& modelView) {
    GLMatrix mvp = modelView;
    mvp.multiply(m_projection);
    
    auto lines = GeometryUtil::clipGeometry(dataset, GeometryUtil::Axis::AxisX, 0.5f); // FIXME
}

GLMatrix GeometrySliceRenderer::createProjectionMatrix(const ScreenInfo& screenInfo) {
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * tanf(3.1415f / 8.0f);
    float aspectRatio = float(screenInfo.height) / float(screenInfo.width);

    GLMatrix projection;
    projection.loadIdentity();
    projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
    return projection;
}