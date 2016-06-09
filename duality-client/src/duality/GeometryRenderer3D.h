#pragma once

#include "src/duality/GeometryDataset.h"
#include "src/duality/MVP3D.h"
#include "duality/ScreenInfo.h"

class GLShader;

class GeometryRenderer3D {
public:
    GeometryRenderer3D();
    ~GeometryRenderer3D();

    void render(const GeometryDataset& dataset, const MVP3D& mvp);

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