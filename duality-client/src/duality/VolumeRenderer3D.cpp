#include "src/duality/VolumeRenderer3D.h"

#include "src/IVDA/GLShader.h"

#include "mocca/log/LogManager.h"

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

void VolumeRenderer3D::render(const VolumeDataset& dataset, const GLMatrix& mvp) {
    LINFO("Render volume dataset");
}
