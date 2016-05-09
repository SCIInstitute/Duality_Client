#pragma once

#include "AbstractRenderer.h"
#include "ScreenInfo.h"
#include "IVDA/GLFrameBufferObject.h"

#include <memory>

class GLFrameBufferObject;

class GeometryRenderer : public AbstractRenderer {
public:
    GeometryRenderer(ScreenInfo screenInfo);
    void render(const GeometryDataset& dataset) override;
    
private:
    ScreenInfo m_screenInfo;
    std::unique_ptr<GLFrameBufferObject> m_fbo;
};