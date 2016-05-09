#pragma once

#include "AbstractRenderer.h"
#include "ScreenInfo.h"
#include "IVDA/GLFrameBufferObject.h"
#include "IVDA/GLShader.h"

#include <memory>

class GeometryRenderer : public AbstractRenderer {
public:
    GeometryRenderer(ScreenInfo screenInfo);
    void render(const GeometryDataset& dataset) override;
    
private:
    ScreenInfo m_screenInfo;
    std::unique_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GLShader> m_normShader;
    std::unique_ptr<GLShader> m_normAlphaShader;
    std::unique_ptr<GLShader> m_normTexAlphaShader;
    std::unique_ptr<GLShader> m_normColShader;
    std::unique_ptr<GLShader> m_colShader;
    std::unique_ptr<GLShader> m_normTexShader;
    std::unique_ptr<GLShader> m_texShader;
};