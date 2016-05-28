#pragma once

#ifndef GLINCLUDE_H
#define GLINCLUDE_H

#include "IVDA/StdDefines.h"
#include "src/IVDA/DebugOutHandler.h"
#include <limits>

#define MAX_GL_ERROR_COUNT 20

#ifdef _DEBUG
# define GL(stmt)                                                 \
do {                                                              \
  GLenum glerr;                                                   \
  unsigned int iCounter = 0;                                      \
  while((glerr = glGetError()) != GL_NO_ERROR) {                  \
    IVDA_ERROR("GL error calling "<< #stmt << " before line " << __LINE__ << " (" << __FILE__ << "): " << static_cast<unsigned>(glerr));  \
    iCounter++;                                                   \
    if (iCounter > MAX_GL_ERROR_COUNT) break;                     \
  }                                                               \
  stmt;                                                           \
  iCounter = 0;                                                   \
  while((glerr = glGetError()) != GL_NO_ERROR) {                  \
    IVDA_ERROR(#stmt << " on line " << __LINE__ << " (" << __FILE__ << ") caused GL error " << static_cast<unsigned>(glerr));  \
    iCounter++;                                                   \
    if (iCounter > MAX_GL_ERROR_COUNT) break;                     \
  }                                                               \
} while(0)
#else
# define GL(stmt) do { stmt; } while(0)
#endif

#endif // GLINCLUDE_H
