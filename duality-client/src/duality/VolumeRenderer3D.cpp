#include "src/duality/VolumeRenderer3D.h"

#include "src/IVDA/GLInclude.h"
#include "src/IVDA/GLShader.h"

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
    updateStackDirection(static_cast<Mat4f>(mvp.mv()));

    GLShader& shader = determineActiveShader();
    shader.Enable();
    shader.SetValue("mMVP", static_cast<IVDA::Mat4f>(mvp.mvp()));

    GL(glDepthMask(GL_FALSE));
    GL(glEnable(GL_BLEND));
    
    GL(glEnableVertexAttribArray(0));
    GL(glEnableVertexAttribArray(1));

    size_t stackSize = dataset.sliceInfos()[m_stackDirection].size();
    for (size_t slice = 0; slice < stackSize; ++slice) {
        size_t index = m_reverseStackOrdering ? (stackSize - 1 - slice) : slice;
        const auto& si = dataset.sliceInfos()[m_stackDirection][index];

        dataset.bindTextures(m_stackDirection, m_reverseStackOrdering ? si.textureIndex2 : si.textureIndex1);

        BoundingBox bb = dataset.boundingBox();
        switch (m_stackDirection) {
        case 0: {
            std::array<Vec3f, 4> vertices = {Vec3f(si.depth, bb.min.y, bb.min.z), Vec3f(si.depth, bb.min.y, bb.max.z),
                                             Vec3f(si.depth, bb.max.y, bb.min.z), Vec3f(si.depth, bb.max.y, bb.max.z)};
            GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0]));
        } break;
        case 1: {
            std::array<Vec3f, 4> vertices = {Vec3f(bb.min.x, si.depth, bb.min.z), Vec3f(bb.min.x, si.depth, bb.max.z),
                                             Vec3f(bb.max.x, si.depth, bb.min.z), Vec3f(bb.max.x, si.depth, bb.max.z)};
            GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0]));

        } break;
        case 2: {
            std::array<Vec3f, 4> vertices = {Vec3f(bb.min.x, bb.min.y, si.depth), Vec3f(bb.min.x, bb.max.y, si.depth),
                                             Vec3f(bb.max.x, bb.min.y, si.depth), Vec3f(bb.max.x, bb.max.y, si.depth)};
            GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0]));
        } break;
        }

        const std::array<Vec2f, 4> texCoords = {Vec2f(0, 0), Vec2f(0, 1), Vec2f(1, 0), Vec2f(1, 1)};
        GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]));

        const std::array<GLshort, 6> indices = {0, 1, 2, 2, 1, 3};
        GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &indices[0]));
    }

    GL(glDepthMask(GL_TRUE));
    GL(glDisable(GL_BLEND));
    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));
}

GLShader& VolumeRenderer3D::determineActiveShader() const {
    return *m_shaderL;
}

void VolumeRenderer3D::updateStackDirection(const IVDA::Mat4f& mv) {
    Vec4f vertex0(-0.5f, -0.5f, 0.5f, 1.0f);
    Vec4f vertex1(0.5f, -0.5f, 0.5f, 1.0f);
    Vec4f vertex3(-0.5f, 0.5f, 0.5f, 1.0f);
    Vec4f vertex4(-0.5f, -0.5f, -0.5f, 1.0f);
    Vec4f vertex6(0.5f, 0.5f, -0.5f, 1.0f);

    Vec3f center = ((vertex0 * mv).xyz() + (vertex6 * mv).xyz()) / 2.0f;
    Vec3f viewDir(0, 0, -2);

    vertex0 = vertex0 * mv + viewDir;
    vertex1 = vertex1 * mv + viewDir;
    vertex3 = vertex3 * mv + viewDir;
    vertex4 = vertex4 * mv + viewDir;
    vertex6 = vertex6 * mv + viewDir;
    center = center + viewDir;

    Vec3f coordFrame[3] = {(vertex0.xyz() - vertex1.xyz()),  // X
                           (vertex0.xyz() - vertex4.xyz()),  // Y
                           (vertex0.xyz() - vertex3.xyz())}; // Z

    for (size_t i = 0; i < 3; ++i) {
        coordFrame[i].normalize();
    }

    float cosX = center ^ coordFrame[0];
    float cosY = center ^ coordFrame[1];
    float cosZ = center ^ coordFrame[2];

    m_stackDirection = 1;
    m_reverseStackOrdering = cosZ < 0;

    if (fabs(cosX) > fabs(cosY) && fabs(cosX) > fabs(cosZ)) {
        m_stackDirection = 0;
        m_reverseStackOrdering = cosX < 0;
    } else {
        if (fabs(cosY) > fabs(cosX) && fabs(cosY) > fabs(cosZ)) {
            m_stackDirection = 2;
            m_reverseStackOrdering = cosY > 0;
        }
    }
}