#if PLATFORM(IPHONE)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif PLATFORM(ANDROID)
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#error Unsupported platform
#endif
