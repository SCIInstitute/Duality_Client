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

    GL(glDrawElements(primitiveType, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data()));

    for (int i = 0; i < attributeCount; ++i) {
        GL(glDisableVertexAttribArray(i));
    }
}

void GeometryRenderer3D::renderTransparent(const GeometryDataset& dataset, const MVP3D& mvp) {
    std::vector<IVDA::Vec3f> centroids(dataset.centroids());
    auto permutation = duality::backToFrontPermutation(dataset.centroids(), mvp.eyePos());

    const auto& indices = dataset.indicesTransparent();
    std::vector<uint32_t> indicesSorted(indices.size());
    if (dataset.geometry().info.primitiveType == G3D::Point) {
        duality::applyPermutation<1>(permutation, indices, indicesSorted);
    } else if (dataset.geometry().info.primitiveType == G3D::Line) {
        duality::applyPermutation<2>(permutation, indices, indicesSorted);
    } else if (dataset.geometry().info.primitiveType == G3D::Triangle) {
        duality::applyPermutation<3>(permutation, indices, indicesSorted);
    }

    renderTransparentPartial(dataset, mvp, indicesSorted);
}

void GeometryRenderer3D::renderTransparentPartial(const GeometryDataset& dataset, const MVP3D& mvp, const std::vector<uint32_t>& indices) {
    auto& shader = determineActiveShader(dataset);
    shader.Enable();
    shader.SetValue("mvpMatrix", static_cast<IVDA::Mat4f>(mvp.mvp()));
    
    GL(glEnable(GL_BLEND));
    GL(glEnable(GL_DEPTH_TEST));
    
    int attributeCount = enableAttributeArrays(dataset);
    int primitiveType = primitiveTypeGL(dataset);
    
    GL(glDrawElements(primitiveType, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data()));
    
    for (int i = 0; i < attributeCount; ++i) {
        GL(glDisableVertexAttribArray(i));
    }
    
    GL(glDisable(GL_BLEND));
    GL(glDisable(GL_DEPTH_TEST));
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

std::vector<int32_t> duality::backToFrontPermutation(const std::vector<IVDA::Vec3f>& points, const IVDA::Vec3f& refPoint) {
    auto sorter = [&](size_t index1, size_t index2) {
        float dist1 = (points[index1] - refPoint).sqLength();
        float dist2 = (points[index2] - refPoint).sqLength();
        return dist1 > dist2;
    };

    std::vector<int32_t> permutation(points.size());
    std::iota(begin(permutation), end(permutation), 0);
    std::sort(begin(permutation), end(permutation), sorter);

    return permutation;
}