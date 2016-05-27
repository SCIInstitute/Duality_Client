#pragma once

#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "IVDA/GLInclude.h"
#include "GPUObject.h"

/** Base for all OpenGL types which will utilize GPU memory */
class GLObject : public GPUObject{
public:
  virtual ~GLObject() {}
};

#endif // GLOBJECT_H
