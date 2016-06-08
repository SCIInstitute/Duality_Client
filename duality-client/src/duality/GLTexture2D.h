#pragma once

#include <OpenGLES/ES3/gl.h>

#include <cstdint>

class GLTexture2D {
public:
    enum class TextureData { Scalar, Color };
    GLTexture2D(const void* data, TextureData dataType, uint32_t sizeU, uint32_t sizeV);
    ~GLTexture2D();

    void bindWithUnit(uint32_t unit) const;

private:
    uint32_t m_textureID;
    uint32_t m_sizeU;
    uint32_t m_sizeV;
};