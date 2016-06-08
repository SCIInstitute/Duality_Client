#include "src/duality/GLTexture2D.h"

#include "src/IVDA/GLInclude.h"

GLTexture2D::GLTexture2D(const void* data, TextureData dataType, uint32_t sizeU, uint32_t sizeV)
    : m_sizeU(sizeU)
    , m_sizeV(sizeV) {
    GL(glActiveTexture(GL_TEXTURE0)); // unfortunately we need to bind the texture thus overriding some state
                                      // but lets at least always override unit 0 to make this call more predictable
    GL(glGenTextures(1, &m_textureID));
    GL(glBindTexture(GL_TEXTURE_2D, m_textureID));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    if (dataType == TextureData::Scalar) {
        GL(glPixelStorei(GL_PACK_ALIGNMENT, 1));
        GL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, sizeU, sizeV, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid*)data));
    } else if (dataType == TextureData::Color) {
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeU, sizeV, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data));
    }
}

GLTexture2D::~GLTexture2D() {
    GL(glDeleteTextures(1, &m_textureID));
}

void GLTexture2D::bindWithUnit(uint32_t unit) const {
    GL(glActiveTexture(GL_TEXTURE0 + unit));
    GL(glBindTexture(GL_TEXTURE_2D, m_textureID));
}
