#include "duality/GeometryRenderer3D.h"

#include "IVDA/GLInclude.h"
#include "duality/Error.h"
#include "duality/private/GLShader.h"

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

void GeometryRenderer3D::render(const GeometryDataset& dataset, const GLMatrix& mvp) {
    auto& shader = determineActiveShader(dataset);
    shader.Enable();

    shader.SetValue("mvpMatrix", static_cast<IVDA::Mat4f>(mvp));

    GL(glEnable(GL_DEPTH_TEST));

    int attributeCount = enableAttributeArrays(dataset);

    int primitiveType = primitiveTypeGL(dataset);
    uint32_t numIndices = dataset.geometryInfo()->numberIndices;
    GL(glDrawElements(primitiveType, (GLsizei)numIndices, GL_UNSIGNED_INT, dataset.getIndices()));

    for (int i = 0; i < attributeCount; ++i) {
        GL(glDisableVertexAttribArray(i));
    }
}


int GeometryRenderer3D::primitiveTypeGL(const GeometryDataset& dataset) {
    switch (dataset.geometryInfo()->primitiveType) {
    case G3D::Point:
        return GL_POINTS;
    case G3D::Line:
        return GL_LINES;
    case G3D::Triangle:
        return GL_TRIANGLES;
    case G3D::TriangleAdj:
        return GL_TRIANGLES; // FIXME: is this corrent?
    default:
        assert("Invalid primitive type");
        return 0;
    }
}

GLShader& GeometryRenderer3D::determineActiveShader(const GeometryDataset& dataset) const {
    if (dataset.getNormals() && !dataset.getColors() && !dataset.getTexCoords() && !dataset.getAlphas())
        return *m_normShader;
    else if (dataset.getNormals() && !dataset.getColors() && !dataset.getTexCoords() && dataset.getAlphas())
        return *m_normAlphaShader;
    else if (dataset.getNormals() && !dataset.getColors() && dataset.getTexCoords() && dataset.getAlphas())
        return *m_normTexAlphaShader;
    else if (dataset.getNormals() && dataset.getColors() && !dataset.getTexCoords() && !dataset.getAlphas())
        return *m_normColShader;
    else if (!dataset.getNormals() && dataset.getColors() && !dataset.getTexCoords() && !dataset.getAlphas())
        return *m_colShader;
    else if (dataset.getNormals() && !dataset.getColors() && dataset.getTexCoords() && !dataset.getAlphas())
        return *m_normTexShader;
    else if (!dataset.getNormals() && !dataset.getColors() && dataset.getTexCoords() && !dataset.getAlphas())
        return *m_texShader;
    throw Error("Cannot determine shader for geometry dataset", __FILE__, __LINE__);
}

int GeometryRenderer3D::enableAttributeArrays(const GeometryDataset& dataset) {
    int attributeIndex = 0;
    if (dataset.getPositions()) {
        GL(glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, 0, 0, dataset.getPositions()));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    if (dataset.getNormals()) {
        GL(glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, 0, 0, dataset.getNormals()));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    if (dataset.getTangents()) {
        GL(glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, 0, 0, dataset.getTangents()));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    if (dataset.getColors()) {
        GL(glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, 0, 0, dataset.getColors()));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }
    // FIXME: textures

    // if (dataset.getTexCoords() && dataset.getTexture())
    //{
    //    GL(glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, 0, 0, dataset.getTexCoords()));
    //    GL(glEnableVertexAttribArray(attributeIndex++));
    //    dataset.getTexture()->bind();
    //}
    if (dataset.getAlphas()) {
        GL(glVertexAttribPointer(attributeIndex, 1, GL_FLOAT, 0, 0, dataset.getAlphas()));
        GL(glEnableVertexAttribArray(attributeIndex++));
    }

    return attributeIndex - 1;
}