#include "GLFrameBufferObject.h"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#include <iostream>

#ifndef _DEBUG
#define COUT(stream)
#else
#define COUT(stream) do { std::cout << stream << std::endl; } while(0)
#endif

GLFrameBufferObject::GLFrameBufferObject(unsigned int width, unsigned int height, bool bCreateDepth) :
GLObject(),
m_iUnit(0),
m_width(0),
m_height(0),
m_frameBufferId(-1),
m_textureId(-1),
m_depthBufferId(-1)
{
	Resize(width,height,bCreateDepth);
}

GLFrameBufferObject::~GLFrameBufferObject()
{
	FreeGL();
}

bool GLFrameBufferObject::Resize(unsigned int width, unsigned int height, bool bCreateDepth)
{
	if (width == m_width && m_height == height) return true;
	
	m_width = width;
	m_height = height;
	
	if (m_frameBufferId != -1) FreeGL();
	
	GL(glGenFramebuffers(1, &m_frameBufferId));
	GL(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	GL(glGenTextures(1, &m_textureId));
	GL(glBindTexture(GL_TEXTURE_2D, m_textureId));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	
	GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0));
	GL(glBindTexture(GL_TEXTURE_2D, 0));
	
	if (bCreateDepth)
	{
	  GL(glGenRenderbuffers(1, &m_depthBufferId));
	  GL(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId));
	  GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height));
	  GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	} 
	else
	{
		m_depthBufferId = -1;
	}
	
	GLenum eFBStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if (eFBStatus != GL_FRAMEBUFFER_COMPLETE)
	{			
    //	LOG((@"Failed to make complete FBO %x with dim: %dx%d (Status: %X Error: %X)", glCheckFramebufferStatus(GL_FRAMEBUFFER), m_width, m_height, eFBStatus, glGetError()));
		GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return false;
	}
	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return true;
}

void GLFrameBufferObject::FreeGL()
{
	if (m_frameBufferId)
	{
		GL(glDeleteFramebuffers(1, &m_frameBufferId));
		m_frameBufferId = -1;
	}
	if (m_textureId)
	{
		GL(glDeleteTextures(1, &m_textureId));
		m_textureId = -1;
	}
	if (m_depthBufferId) 
	{
		GL(glDeleteRenderbuffers(1, &m_depthBufferId));
		m_depthBufferId = -1;
	}
}

void GLFrameBufferObject::Read(unsigned int iUnit)
{
	m_iUnit = iUnit;
	GL(glActiveTexture(GL_TEXTURE0 + iUnit));
	GL(glBindTexture(GL_TEXTURE_2D, m_textureId));
}

void GLFrameBufferObject::FinishRead()
{
	GL(glActiveTexture(GL_TEXTURE0 + m_iUnit));
	GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLFrameBufferObject::Write()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	if (m_depthBufferId) GL(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId));
}

void GLFrameBufferObject::SetViewport() {
  GL(glViewport(0, 0, m_width, m_height));
}

void GLFrameBufferObject::FinishWrite(unsigned int viewFramebuffer, unsigned int depthBuffer)
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer));
	if (m_depthBufferId) GL(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
}

