#include "src/duality/GeometryRenderer2D.h"

#include "src/IVDA/GLInclude.h"
#include "src/IVDA/GLShader.h"
#include "src/duality/GeometryUtil.h"

#include <OpenGLES/ES3/gl.h>


GeometryRenderer2D::GeometryRenderer2D()
    : m_slice(0.0f) {
    GlShaderAttributes attributes;
    attributes.push_back("position");
    attributes.push_back("color");
    m_shader = std::make_unique<GLShader>("geo_pc.vsh", "geo_pc.fsh", attributes);
}

GeometryRenderer2D::~GeometryRenderer2D() = default;

void GeometryRenderer2D::render(const GeometryDataset& dataset, const GLMatrix& mvp) {
    auto lines = GeometryUtil::clipGeometry(dataset, GeometryUtil::Axis::AxisZ, m_slice); // FIXME

    if (lines.getPositions()) {
        GL(glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, lines.getPositions()));
        GL(glEnableVertexAttribArray(0));
    }
    if (lines.getColors()) {
        GL(glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, lines.getColors()));
        GL(glEnableVertexAttribArray(1));
    }

    m_shader->Enable();
    m_shader->SetValue("mvpMatrix", static_cast<IVDA::Mat4f>(mvp));

    GL(glLineWidth(5.0));
    GL(glDisable(GL_DEPTH_TEST));
    GL(glEnable(GL_BLEND));
    GL(glDrawElements(GL_LINES, (GLsizei)lines.geometryInfo()->numberIndices, GL_UNSIGNED_INT, lines.getIndices().data()));

    GL(glLineWidth(1.0));
    GL(glDisable(GL_BLEND));
    GL(glDisableVertexAttribArray(0));
    GL(glDisableVertexAttribArray(1));
    GL(glEnable(GL_DEPTH_TEST));
}

void GeometryRenderer2D::setSlice(float slice) {
    m_slice = slice;
}