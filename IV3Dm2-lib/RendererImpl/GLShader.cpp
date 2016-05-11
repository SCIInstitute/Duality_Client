#include "GLShader.h"
#include "iOS.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <fstream>
#include "SysTools.h"
#include "Macros.h"



GLShader::GLShader(const std::string& vertexShader, const std::string& pixelShader, const std::vector< std::string> & attribs = std::vector< std::string>()) :
GLObject(),
m_bIsValid(false),
m_GLID(0)
{
	m_bIsValid = LoadShaders(vertexShader,pixelShader,attribs);
}

GLShader::~GLShader() {
  if (m_GLID)
  {
    GL(glDeleteProgram(m_GLID));
    m_GLID = 0;
  }	
}

// reads an entire file into a string.
std::string GLShader::FileToString(const std::string& filename) const
{
	// open in append mode so the file pointer will be at EOF and we can
	// therefore easily/quickly figure out the file size.
	std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::ate);
	if(!ifs.is_open()) {
		return "";
	}
	std::ifstream::pos_type len = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	
	std::vector<char> shader(len+std::ifstream::pos_type(1), 0);
	size_t offset=0;
	do {
		std::streamsize length = std::streamsize(len) - std::streamsize(offset);
		ifs.read(&shader[offset], length);
		offset += ifs.gcount();
	} while(!ifs.eof() && std::ifstream::pos_type(offset) < len);
	ifs.close();
	
	return std::string(&shader[0]);
}

bool GLShader::CompileShader(unsigned int *shader, int type, const std::string& filename)
{
  GLint status;
  const GLchar *source;
	
	std::string fileStr = FileToString(filename);
  source = (GLchar *)fileStr.c_str();
  if (!source)
  {
    return false;
  }
	
  *shader = glCreateShader(type);
  GL(glShaderSource(*shader, 1, &source, NULL));
  GL(glCompileShader(*shader));
	
#if defined(_DEBUG)
  GLint logLength;
  GL(glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength));
  if (logLength > 0)
  {
    GLchar *log = new GLchar[logLength];
    GL(glGetShaderInfoLog(*shader, logLength, &logLength, log));
		IVDA_MESSAGE("Shader info log:\n" + std::string(log));
    delete [] log;
  }
#endif
	
  GL(glGetShaderiv(*shader, GL_COMPILE_STATUS, &status));
  if (status == 0)
  {
    GL(glDeleteShader(*shader));
    return false;
  }
	
  return true;
}

bool GLShader::LinkProgram(unsigned int prog)
{
  GLint status;
	
  GL(glLinkProgram(prog));
	
#if defined(_DEBUG)
  GLint logLength;
  GL(glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength));
  if (logLength > 0)
  {
    GLchar *log = new GLchar[logLength];
    GL(glGetProgramInfoLog(prog, logLength, &logLength, log));
		IVDA_MESSAGE("Program info log:\n" + std::string(log));
    delete [] log;
  }
#endif
	
  GL(glGetProgramiv(prog, GL_LINK_STATUS, &status));
  if (status == 0)
    return false;
	
  return true;
}

bool GLShader::ValidateProgram(unsigned int prog)
{
  GLint logLength, status;
	
  GL(glValidateProgram(prog));
  GL(glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength));
  if (logLength > 0)
  {
    GLchar *log = new GLchar[logLength];
    GL(glGetProgramInfoLog(prog, logLength, &logLength, log));
    IVDA_MESSAGE("Program info log:\n" + std::string(log));
    delete [] log;
  }
	
  GL(glGetProgramiv(prog, GL_VALIDATE_STATUS, &status));
  if (status == 0)
    return false;
	
  return true;
}


bool GLShader::LoadShaders(const std::string& vertexShader, const std::string& pixelShader, const std::vector< std::string> & attribs)
{
  unsigned int vertShader, fragShader;
	
  // Create shader program
  m_GLID = glCreateProgram();
	
  // Create and compile vertex shader
  std::string vertShaderPathname = iOS::GetBundleFilename(vertexShader);
  if (!CompileShader(&vertShader, GL_VERTEX_SHADER, vertShaderPathname))
  {
		IVDA_ERROR("Failed to compile vertex shader: " << vertexShader);
    return false;
  }
	
  // Create and compile fragment shader
  std::string fragShaderPathname = iOS::GetBundleFilename(pixelShader);
  if (!CompileShader(&fragShader, GL_FRAGMENT_SHADER, fragShaderPathname))
  {
		IVDA_ERROR("Failed to compile fragment shader: " << pixelShader);
    return false;
  }
	
  // Attach vertex shader to program
  GL(glAttachShader(m_GLID, vertShader));
	
  // Attach fragment shader to program
  GL(glAttachShader(m_GLID, fragShader));
	
  // Bind attribute locations
  // this needs to be done prior to linking
	for (size_t i = 0;i<attribs.size();i++) {
		GL(glBindAttribLocation(m_GLID, GLuint(i), attribs[i].c_str()));		
	}
	
  // Link program
  if (!LinkProgram(m_GLID))
  {
    IVDA_ERROR("Failed to link program: "+IVDA::SysTools::ToString(m_GLID));
		
    if (vertShader)
    {
      GL(glDeleteShader(vertShader));
      vertShader = 0;
    }
    if (fragShader)
    {
      GL(glDeleteShader(fragShader));
      fragShader = 0;
    }
    if (m_GLID)
    {
      GL(glDeleteProgram(m_GLID));
      m_GLID = 0;
    }
    
    return false;
  }
	
  // Release vertex and fragment shaders
  if (vertShader)
    GL(glDeleteShader(vertShader));
  if (fragShader)
    GL(glDeleteShader(fragShader));
  
  return true;
}


unsigned int GLShader::GetUniformID(const std::string& name) {
	std::map<std::string, int>::iterator elem = m_knownIDs.find(name);
	
	if (elem != m_knownIDs.end()) {
		return elem->second;
	} else {
		unsigned int newID = glGetUniformLocation(m_GLID, name.c_str());
		m_knownIDs[name] = newID;
		return newID;		
	}
}

void GLShader::Enable() {
	GL(glUseProgram(m_GLID));
}

void GLShader::SetValue(const std::string& name, int val) {
	GLint location = GetUniformID(name);
	GL(glUniform1i(location, val));
}

void GLShader::SetValue(const std::string& name, float val) {
	GLint location = GetUniformID(name);
	GL(glUniform1f(location, val));
}

void GLShader::SetValue(const std::string& name, const IVDA::INTVECTOR2& val) {
	GLint location = GetUniformID(name);
	GL(glUniform2i(location, val.x, val.y));	
}

void GLShader::SetValue(const std::string& name, const IVDA::FLOATVECTOR2& val) {
	GLint location = GetUniformID(name);
	GL(glUniform2f(location, val.x, val.y));	
}

void GLShader::SetValue(const std::string& name, const IVDA::INTVECTOR3& val) {
	GLint location = GetUniformID(name);
	GL(glUniform3i(location, val.x, val.y, val.z));		
}

void GLShader::SetValue(const std::string& name, const IVDA::FLOATVECTOR3& val) {
	GLint location = GetUniformID(name);
	GL(glUniform3f(location, val.x, val.y, val.z));		
}

void GLShader::SetValue(const std::string& name, const IVDA::INTVECTOR4& val) {
	GLint location = GetUniformID(name);
	GL(glUniform4i(location, val.x, val.y, val.z, val.w));
}

void GLShader::SetValue(const std::string& name, const IVDA::FLOATVECTOR4& val) {
	GLint location = GetUniformID(name);
	GL(glUniform4f(location, val.x, val.y, val.z, val.w));
}

void GLShader::SetValue(const std::string& name, const IVDA::FLOATMATRIX2& val, bool bTranspose) {
	GLint location = GetUniformID(name);
	GL(glUniformMatrix2fv(location, 1, bTranspose, val.array));
}

void GLShader::SetValue(const std::string& name, const IVDA::FLOATMATRIX3& val, bool bTranspose) {
	GLint location = GetUniformID(name);
	GL(glUniformMatrix3fv(location, 1, bTranspose, val.array));	
}

void GLShader::SetValue(const std::string& name, const IVDA::FLOATMATRIX4& val, bool bTranspose) {
	GLint location = GetUniformID(name);
	GL(glUniformMatrix4fv(location, 1, bTranspose, val.array));	
}

void GLShader::SetTexture(const std::string& name, int iUnit) {
	SetValue(name, iUnit);
}
