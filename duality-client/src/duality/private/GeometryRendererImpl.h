#pragma once

#include "duality/private/GLFrameBufferObject.h"
#include "duality/private/GLShader.h"
#include "duality/G3D.h"
#include "duality/GLMatrix.h"
#include "duality/ScreenInfo.h"

#include <memory>

class GeometryDataset;

class GeometryRendererImpl {
public:
    GeometryRendererImpl();
    void render(const GeometryDataset& dataset, const GLMatrix& mvp);

private:
    static int primitiveTypeGL(const GeometryDataset& dataset);
    static int enableAttributeArrays(const GeometryDataset& dataset);
    GLShader& determineActiveShader(const GeometryDataset& dataset) const;

private:
    std::unique_ptr<GLShader> m_normShader;
    std::unique_ptr<GLShader> m_normAlphaShader;
    std::unique_ptr<GLShader> m_normTexAlphaShader;
    std::unique_ptr<GLShader> m_normColShader;
    std::unique_ptr<GLShader> m_colShader;
    std::unique_ptr<GLShader> m_normTexShader;
    std::unique_ptr<GLShader> m_texShader;
};