#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#include "GLObject.h"

class GLFrameBufferObject : public GLObject {
public:
	GLFrameBufferObject(unsigned int width, unsigned int height, bool bCreateDepth=false);
	virtual ~GLFrameBufferObject();
	
	bool Resize(unsigned int width, unsigned int height, bool bCreateDepth=false);
	
	void Write();
	void SetViewport();
	void Read(unsigned int iUnit=0);
	void FinishWrite(unsigned int viewFramebuffer, unsigned int depthBuffer);
	void FinishRead();
	
	virtual uint64_t GetCPUSize() {return GetGPUSize();}
  virtual uint64_t GetGPUSize() {return m_width * m_height * 4 + (m_depthBufferId ? (m_width * m_height * 2) : 0);}
  
private:
	unsigned int m_iUnit;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_frameBufferId;
	unsigned int m_textureId;
	unsigned int m_depthBufferId;
	
	void FreeGL();
};

#endif  // GLFRAMEBUFFEROBJECT_H
