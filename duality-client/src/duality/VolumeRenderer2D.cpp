#include "src/duality/VolumeRenderer2D.h"

#include "src/IVDA/GLInclude.h"
#include "src/IVDA/GLShader.h"

#include <algorithm>

VolumeRenderer2D::VolumeRenderer2D() {
    GlShaderAttributes attributes;
    attributes.push_back("vPosition");
    attributes.push_back("vTexCoords");
    m_shader = std::make_unique<GLShader>("sliceQuad.vsh", "sliceQuad.fsh", attributes);
    m_shader->Enable();
    m_shader->SetTexture("tf", 0);
    m_shader->SetTexture("slice1", 1);
    m_shader->SetTexture("slice2", 2);

    m_vertices[CoordinateAxis::X_Axis][0] = {0.0f, -0.5, -0.5f};
    m_vertices[CoordinateAxis::X_Axis][1] = {0.0f, -0.5, 0.5f};
    m_vertices[CoordinateAxis::X_Axis][2] = {0.0f, 0.5, -0.5f};
    m_vertices[CoordinateAxis::X_Axis][3] = {0.0f, 0.5, 0.5f};

    m_vertices[CoordinateAxis::Y_Axis][0] = {-0.5f, 0.0f, -0.5f};
    m_vertices[CoordinateAxis::Y_Axis][1] = {-0.5f, 0.0f, 0.5f};
    m_vertices[CoordinateAxis::Y_Axis][2] = {0.5f, 0.0f, -0.5f};
    m_vertices[CoordinateAxis::Y_Axis][3] = {0.5f, 0.0f, 0.5f};

    m_vertices[CoordinateAxis::Z_Axis][0] = {-0.5, -0.5f, 0.0f};
    m_vertices[CoordinateAxis::Z_Axis][1] = {-0.5, 0.5f, 0.0f};
    m_vertices[CoordinateAxis::Z_Axis][2] = {0.5, -0.5f, 0.0f};
    m_vertices[CoordinateAxis::Z_Axis][3] = {0.5, 0.5f, 0.0f};

    m_texCoords[0] = {0.0f, 0.0f};
    m_texCoords[1] = {0.0f, 1.0f};
    m_texCoords[2] = {1.0f, 0.0f};
    m_texCoords[3] = {1.0f, 1.0f};
}

VolumeRenderer2D::~VolumeRenderer2D() = default;

void VolumeRenderer2D::render(const VolumeDataset& dataset, const GLMatrix& mvp, const TransferFunction& tf, CoordinateAxis axis, float slice) {
    float datasetRange = std::abs(dataset.boundingBox().max[axis] - dataset.boundingBox().min[axis]);
    size_t sliceIndex = std::min<size_t>((slice - dataset.boundingBox().min[axis]) / datasetRange * dataset.sliceInfos()[axis].size(),
                                         dataset.sliceInfos()[axis].size() - 1);
    const auto& sliceInfo = dataset.sliceInfos()[axis][sliceIndex];
    size_t texIndex1 = sliceInfo.textureIndex1;
    size_t texIndex2 = sliceInfo.textureIndex2;

    tf.bindTexture();
    dataset.bindTextures(axis, texIndex1, texIndex2);
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices[axis].data()));
    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords.data()));
    GL(glEnableVertexAttribArray(1));

    m_shader->Enable();
    m_shader->SetValue("mvpMatrix", static_cast<IVDA::Mat4f>(mvp));
    m_shader->SetValue("interpolationParameter", sliceInfo.interpolationParam);
    // FIXME: hardcoded values
    m_shader->SetValue("oc", 1.0f);
    m_shader->SetValue("ignoreAlpha", false);

    GL(glDisable(GL_DEPTH_TEST));
    GL(glEnable(GL_BLEND));
    const std::array<GLshort, 6> sliceIndices = {0, 1, 2, 2, 1, 3};
    GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, sliceIndices.data()));

    GL(glEnable(GL_DEPTH_TEST));
    GL(glDisable(GL_BLEND));
    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));
}