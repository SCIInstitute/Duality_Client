#include "GeometryRenderer.h"

GeometryRenderer::GeometryRenderer(ScreenInfo screenInfo)
    : m_screenInfo(std::move(screenInfo))
    , m_fbo(std::make_unique<GLFrameBufferObject>(screenInfo.width / screenInfo.standardDownSampleFactor,
                                                  screenInfo.height / screenInfo.standardDownSampleFactor, true)) {}

void GeometryRenderer::render(const GeometryDataset& dataset) {
    // TODO: implement
}
