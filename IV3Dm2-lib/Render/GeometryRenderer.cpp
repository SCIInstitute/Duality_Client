#include "GeometryRenderer.h"

GeometryRenderer::GeometryRenderer(ScreenInfo screenInfo)
    : m_screenInfo(std::move(screenInfo))
    , m_fbo(std::make_unique<GLFrameBufferObject>(screenInfo.width / screenInfo.standardDownSampleFactor,
                                                  screenInfo.height / screenInfo.standardDownSampleFactor, true)) {

    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        attributes.push_back("normal");
        m_normShader = std::make_unique<GLShader>("geo_pn.vsh", "geo_pn.fsh", attributes);
    }
    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        attributes.push_back("normal");
        attributes.push_back("alpha");
        m_normAlphaShader = std::make_unique<GLShader>("geo_pna.vsh", "geo_pna.fsh", attributes);
    }
    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        //  attributes.push_back("normal"); // FIXME: needed or not??
        attributes.push_back("tex");
        attributes.push_back("alpha");
        m_normTexAlphaShader = std::make_unique<GLShader>("geo_pnta.vsh", "geo_pnta.fsh", attributes);
        m_normTexAlphaShader->Enable();
        m_normTexAlphaShader->SetTexture("texture", 0);
    }
    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        attributes.push_back("normal");
        attributes.push_back("color");
        m_normColShader = std::make_unique<GLShader>("geo_pnc.vsh", "geo_pnc.fsh", attributes);
    }
    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        attributes.push_back("color");
        m_colShader = std::make_unique<GLShader>("geo_pc.vsh", "geo_pc.fsh", attributes);
    }
    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        attributes.push_back("normal");
        attributes.push_back("tex");
        m_normTexShader = std::make_unique<GLShader>("geo_pnt.vsh", "geo_pnt.fsh", attributes);
        m_normTexShader->Enable();
        m_normTexShader->SetTexture("texture", 0);
    }
    {
        GlShaderAttributes attributes;
        attributes.push_back("position");
        attributes.push_back("tex");
        m_texShader = std::make_unique<GLShader>("geo_pt.vsh", "geo_pt.fsh", attributes);
        m_texShader->Enable();
        m_texShader->SetTexture("texture", 0);
    }
}

void GeometryRenderer::render(const GeometryDataset& dataset) {
    // TODO: implement
}
