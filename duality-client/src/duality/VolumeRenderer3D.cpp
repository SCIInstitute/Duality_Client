#include "src/duality/VolumeRenderer3D.h"

#include "src/IVDA/GLShader.h"
#include "src/IVDA/GLInclude.h"

using namespace IVDA;

VolumeRenderer3D::VolumeRenderer3D() {
    GlShaderAttributes attributes;
    attributes.push_back("vPosition");
    attributes.push_back("vTexCoords");

    m_shaderL = std::make_unique<GLShader>("vol.vsh", "vol_l.fsh", attributes);
    m_shaderL->Enable();
    m_shaderL->SetTexture("transferFunction", 0);
    m_shaderL->SetTexture("sliceTexture", 1);

    m_shaderNL = std::make_unique<GLShader>("vol.vsh", "vol_nl.fsh", attributes);
    m_shaderNL->Enable();
    m_shaderNL->SetTexture("transferFunction", 0);
    m_shaderNL->SetTexture("sliceTexture", 1);

    m_shaderLTI = std::make_unique<GLShader>("vol.vsh", "vol_lti.fsh", attributes);
    m_shaderLTI->Enable();
    m_shaderLTI->SetTexture("transferFunction", 0);
    m_shaderLTI->SetTexture("sliceTexture1", 1);
    m_shaderLTI->SetTexture("sliceTexture2", 2);

    m_shaderNLTI = std::make_unique<GLShader>("vol.vsh", "vol_nlti.fsh", attributes);
    m_shaderNLTI->Enable();
    m_shaderNLTI->SetTexture("transferFunction", 0);
    m_shaderNLTI->SetTexture("sliceTexture1", 1);
    m_shaderNLTI->SetTexture("sliceTexture2", 2);
}

VolumeRenderer3D::~VolumeRenderer3D() = default;

void VolumeRenderer3D::render(const VolumeDataset& dataset, const MVP3D& mvp) {
    GLShader& shader = determineActiveShader();
    shader.Enable();

    shader.SetValue("mMVP", static_cast<IVDA::Mat4f>(mvp.mvp()));

    GL(glDepthMask(GL_FALSE));
    GL(glEnable(GL_BLEND));
    GL(glEnableVertexAttribArray(0));
    GL(glEnableVertexAttribArray(1));
}

GLShader& VolumeRenderer3D::determineActiveShader() const {
    return *m_shaderL;
}