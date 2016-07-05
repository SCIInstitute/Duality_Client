#include "src/duality/GeometryRenderer3D.h"

#include "duality/Error.h"
#include "src/IVDA/GLInclude.h"
#include "src/IVDA/GLShader.h"

#include <OpenGLES/ES3/gl.h>

GeometryRenderer3D::GeometryRenderer3D() {
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

GeometryRenderer3D::~GeometryRenderer3D() = default;

void GeometryRenderer3D::renderOpaque(const GeometryDataset& dataset, const MVP3D& mvp) {
    auto& shader = determineActiveShader(dataset);
    shader.Enable();

    shader.SetValue("mvpMatrix", static_cast<IVDA::Mat4f>(mvp.mvp()));

    GL(glEnable(GL_DEPTH_TEST));

    int attributeCount = enableAttributeArrays(dataset);

    int primitiveType = primitiveTypeGL(dataset);
    const auto& indices = dataset.indicesOpaque();
    if (indices.empty()) return;
    
    GL(glDrawElements(primitiveType, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data()));

    for (int i = 0; i < attributeCount; ++i) {
        GL(glDisableVertexAttribArray(i));
    }
}

void GeometryRenderer3D::renderTransparent(const GeometryDataset& dataset, const MVP3D& mvp) {
    // FIXME: Code duplication

    auto& shader = determineActiveShader(dataset);
    shader.Enable();

    shader.SetValue("mvpMatrix", static_cast<IVDA::Mat4f>(mvp.mvp()));
    
    // disable depth write
    //GL(glDepthMask(GL_FALSE));
    // enable alpha blending
    GL(glEnable(GL_BLEND));
    GL(glEnable(GL_DEPTH_TEST));

    int attributeCount = enableAttributeArrays(dataset);

    int primitiveType = primitiveTypeGL(dataset);
    auto indices = dataset.indicesTransparentSorted(mvp.eyePos());
    if (indices.empty()) return;
    
    GL(glDrawElements(primitiveType, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data()));

    for (int i = 0; i < attributeCount; ++i) {
        GL(glDisableVertexAttribArray(i));
    }
    
    // disable depth write
    //GL(glDepthMask(GL_FALSE));
    // enable alpha blending
    GL(glEnable(GL_BLEND));
}

int GeometryRenderer3D::primitiveTypeGL(const GeometryDataset& dataset) {
    switch (dataset.geometry().info.primitiveType) {
    case G3D::Point:
        return GL_POINTS;
    case G3D::Line:
        return GL_LINES;
    case G3D::Triangle:
        return GL_TRIANGLES;
    case G3D::TriangleAdj:
        return GL_TRIANGLES; // FIXME: is this correct?
    default:
        assert("Invalid primitive type");
        return 0;
    }
}

GLShader& GeometryRenderer3D::determineActiveShader(const GeometryDataset& dataset) const {
    if (dataset.geometry().normals && !dataset.geometry().colors && !dataset.geometry().texcoords && !dataset.geometry().alphas)
        return *m_normShader;
    else if (dataset.geometry().normals && !dataset.geometry().colors && !dataset.geometry().texcoords && dataset.geometry().alphas)
        return *m_normAlphaShader;
    else if (dataset.geometry().normals && !dataset.geometry().colors && dataset.geometry().texcoords && dataset.geometry().alphas)
        return *m_normTexAlphaShader;
    else if (dataset.geometry().normals && dataset.geometry().colors && !dataset.geometry().texcoords && !dataset.geometry().alphas)
        return *m_normColShader;
    else if (!dataset.geometry().normals && dataset.geometry().colors && !dataset.geometry().texcoords && !dataset.geometry().alphas)
        return *m_colShader;
    else if (dataset.geometry().normals && !dataset.geometry().colors && dataset.geometry().texcoords && !dataset.geometry().alphas)
        return *m_normTexShader;
    else if (!dataset.geometry().normals && !dataset.geometry().colors && dataset.geometry().texcoords && !dataset.geometry().alphas)
        return *m_texShader;
    throw Error("Cannot determine shader for geometry dataset", __FILE__, __LINE__);
}

int GeometryRenderer3D::enableAttributeArrays(const GeometryDataset& dataset) {
    int attributeIndex = 0;
    if (dataset.geometry().positions) {
        GL(glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, 0, 0, dataset.geometry().positions));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    if (dataset.geometry().normals) {
        GL(glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, 0, 0, dataset.geometry().normals));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    if (dataset.geometry().tangents) {
        GL(glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, 0, 0, dataset.geometry().tangents));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    if (dataset.geometry().colors) {
        GL(glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, 0, 0, dataset.geometry().colors));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    // FIXME: textures

    // if (dataset.getTexCoords() && dataset.getTexture())
    //{
    //    GL(glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, 0, 0, dataset.getTexCoords()));
    //    GL(glEnableVertexAttribArray(attributeIndex++));
    //    dataset.getTexture()->bind();
    //}
    if (dataset.geometry().alphas) {
        GL(glVertexAttribPointer(attributeIndex, 1, GL_FLOAT, 0, 0, dataset.geometry().alphas));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }

    return attributeIndex - 1;
}