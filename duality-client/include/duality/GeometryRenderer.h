#pragma once

#include "duality/AbstractRenderer.h"
#include "duality/G3D.h"
#include "duality/GeometryDataset.h"
#include "duality/ScreenInfo.h"

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
    GeometryRenderer(const ScreenInfo& screenInfo);
    void render(const GeometryDataset& dataset, const GLMatrix& modelView);

private:
    std::unique_ptr<GeometryRendererImpl> m_impl;
};