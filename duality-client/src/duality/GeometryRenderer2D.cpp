#include "duality/GeometryRenderer2D.h"

#include "duality/private/GLShader.h"
#include "duality/GeometryUtil.h"
#include "IVDA/GLInclude.h"

#include <OpenGLES/ES3/gl.h>


GeometryRenderer2D::GeometryRenderer2D(const ScreenInfo& screenInfo)
    : m_projection(createProjectionMatrix(screenInfo)) {
    GlShaderAttributes attributes;
    attributes.push_back("position");
    attributes.push_back("color");
    m_shader = std::make_unique<GLShader>("geo_pc.vsh", "geo_pc.fsh", attributes);
}

GeometryRenderer2D::~GeometryRenderer2D() = default;

void GeometryRenderer2D::render(const GeometryDataset& dataset, const GLMatrix& modelView) {
    auto lines = GeometryUtil::clipGeometry(dataset, GeometryUtil::Axis::AxisX, -16); // FIXME

    if (lines.getPositions()) {
        GL(glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, lines.getPositions()));
        GL(glEnableVertexAttribArray(0));
    }
    if (lines.getColors()) {
        GL(glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, lines.getColors()));
        GL(glEnableVertexAttribArray(1));
    }
    
    m_shader->Enable();
    GLMatrix mvp;
    mvp.loadIdentity();
    m_shader->SetValue("mvpMatrix", (IVDA::Mat4f)mvp);
    
    GL(glLineWidth(5.0));
    GL(glDisable(GL_DEPTH_TEST));
    GL(glEnable(GL_BLEND));
    GL(glDrawElements(GL_LINES, lines.geometryInfo()->numberIndices, GL_UNSIGNED_SHORT, lines.getIndices()));
    
    GL(glLineWidth(1.0));
    GL(glDisable(GL_BLEND));
    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));
    GL(glEnable(GL_DEPTH_TEST));
}

GLMatrix GeometryRenderer2D::createProjectionMatrix(const ScreenInfo& screenInfo) {
    float zNear = 0.01f;
    float zFar = 1000.0f;
    float frustSize = zNear * tanf(3.1415f / 8.0f);
    float aspectRatio = float(screenInfo.height) / float(screenInfo.width);

    GLMatrix projection;
    projection.loadIdentity();
    projection.frustum(-frustSize, frustSize, -frustSize * aspectRatio, frustSize * aspectRatio, zNear, zFar);
    return projection;
}