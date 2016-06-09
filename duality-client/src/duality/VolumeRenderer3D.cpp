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

    size_t dir = 1;
    
    for (size_t slice = 0; slice < dataset.sliceInfos()[dir].size(); ++slice) {
        const auto& si = dataset.sliceInfos()[dir][slice];

        dataset.bindTextures(dir, si.textureIndex2);

        BoundingBox bb = dataset.boundingBox();
        std::array< IVDA::FLOATVECTOR3, 4> vertices = {
            FLOATVECTOR3(si.depth, bb.min.y, bb.min.z),
            FLOATVECTOR3(si.depth, bb.min.y, bb.max.z),
            FLOATVECTOR3(si.depth, bb.max.y, bb.min.z),
            FLOATVECTOR3(si.depth, bb.max.y, bb.max.z)
        };
        GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0]));

        const std::array<Vec2f, 4> texCoords = { Vec2f(0,0), Vec2f(0,1), Vec2f(1,0), Vec2f(1,1) };
        GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]));
        
        const std::array<GLshort, 6> indices = { 0,1,2,2,1,3 };
        GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &indices[0]));
    }
    
    GL(glDepthMask(GL_TRUE));
    GL(glDisable(GL_BLEND));
    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));
}

GLShader& VolumeRenderer3D::determineActiveShader() const {
    return *m_shaderNL;
}