#pragma once

#include "Scene/AbstractRenderer.h"
#include "Scene/G3D.h"
#include "Scene/GeometryNode.h"
#include "Scene/ScreenInfo.h"

#include <memory>

#if __APPLE__
#include "TargetConditionals.h"
#endif

#if !__APPLE__ || !TARGET_OS_IPHONE
class GeometryRendererImpl {
    ~GeometryRendererImpl() = default;
};
#else
#include "RendererImpl/GeometryRendererImpl.h"
#endif

class GeometryRenderer : public AbsstractRenderer {
public:
    GeometryRenderer(ScreenInfo screenInfo);
    void render(const GeometryNode& dataset, const GLMatrix& modelView);

private:
    std::unique_ptr<GeometryRendererImpl> m_impl;
};