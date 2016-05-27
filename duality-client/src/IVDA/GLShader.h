#pragma once

#ifndef GLSHADER_H
#define GLSHADER_H

#include "IVDA/GLObject.h"
#include "IVDA/Vectors.h"
#include <map>
#include <string>
#include <vector>

typedef std::vector<std::string> GlShaderAttributes;

class GLTexture;

class GLShader : public GLObject {
public:
    GLShader(const std::string& vertexShader, const std::string& pixelShader, const GlShaderAttributes& attribs);
    ~GLShader();

    void Enable();
    bool IsValid() const { return m_bIsValid; }

    void SetValue(const std::string& name, int val);
    void SetValue(const std::string& name, float val);
    void SetValue(const std::string& name, const IVDA::INTVECTOR2& val);
    void SetValue(const std::string& name, const IVDA::FLOATVECTOR2& val);
    void SetValue(const std::string& name, const IVDA::INTVECTOR3& val);
    void SetValue(const std::string& name, const IVDA::FLOATVECTOR3& val);
    void SetValue(const std::string& name, const IVDA::INTVECTOR4& val);
    void SetValue(const std::string& name, const IVDA::FLOATVECTOR4& val);
    void SetValue(const std::string& name, const IVDA::FLOATMATRIX2& val, bool bTranspose = false);
    void SetValue(const std::string& name, const IVDA::FLOATMATRIX3& val, bool bTranspose = false);
    void SetValue(const std::string& name, const IVDA::FLOATMATRIX4& val, bool bTranspose = false);

    void SetTexture(const std::string& name, int iUnit);

    // make the GLObject Interface happy
    virtual uint64_t GetCPUSize() { return uint64_t(1); }
    virtual uint64_t GetGPUSize() { return uint64_t(1); }

protected:
    bool m_bIsValid;
    unsigned int m_GLID;
    std::map<std::string, int> m_knownIDs;

    unsigned int GetUniformID(const std::string& name);

    std::string FileToString(const std::string& filename) const;

    bool LoadShaders(const std::string& vertexShader, const std::string& pixelShader, const GlShaderAttributes& attribs);
    bool CompileShader(unsigned int* shader, int type, const std::string& filename);
    bool LinkProgram(unsigned int prog);
    bool ValidateProgram(unsigned int prog);
};

#endif // GLSHADER_H
