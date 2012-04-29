/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

 */

#ifndef OpenAphid_G2DMacros_h
#define OpenAphid_G2DMacros_h

#include "OAData.h"
#include "NamespaceG2D.h"

#include "OAUtil.h"

#include "G2DConfig.h"

#import <OpenGLES/ES1/glext.h>

namespace Aphid {
	static inline float G2D_CONTENT_SCALE_FACTOR() 
	{
		return NamespaceG2D::s_content_scale_factor;
	}
	
	static OA_RELEASE_INLINE Rect toPoints(const Rect& r)
	{
		return RectMake(r.origin.x / G2D_CONTENT_SCALE_FACTOR(),
										r.origin.y / G2D_CONTENT_SCALE_FACTOR(),
										r.size.width / G2D_CONTENT_SCALE_FACTOR(),
										r.size.height / G2D_CONTENT_SCALE_FACTOR()
										);
	}
	
	static OA_RELEASE_INLINE Rect toPixels(const Rect& r)
	{
		return RectMake(r.origin.x * G2D_CONTENT_SCALE_FACTOR(),
										r.origin.y * G2D_CONTENT_SCALE_FACTOR(),
										r.size.width * G2D_CONTENT_SCALE_FACTOR(),
										r.size.height * G2D_CONTENT_SCALE_FACTOR()
										);
	}
	
	static OA_RELEASE_INLINE Size toPoints(const Size& size)
	{
		return SizeMake(size.width / G2D_CONTENT_SCALE_FACTOR(),
										size.height / G2D_CONTENT_SCALE_FACTOR()
										);
	}
	
	static OA_RELEASE_INLINE Size toPixels(const Size& size)
	{
		return SizeMake(size.width * G2D_CONTENT_SCALE_FACTOR(),
										size.height * G2D_CONTENT_SCALE_FACTOR()
										);
	}
	
	static OA_RELEASE_INLINE Point toPoints(const Point& p)
	{
		return PointMake(p.x / G2D_CONTENT_SCALE_FACTOR(),
										p.y / G2D_CONTENT_SCALE_FACTOR()
										);
	}
	
	static OA_RELEASE_INLINE Point toPixels(const Point& p)
	{
		return PointMake(p.x * G2D_CONTENT_SCALE_FACTOR(),
										p.y * G2D_CONTENT_SCALE_FACTOR()
										);
	}
	
	static OA_RELEASE_INLINE Point g2d_mult(const Point& p, float mult)
	{
		return PointMake(p.x * mult, p.y * mult);
	}
	
	static OA_RELEASE_INLINE Size g2d_mult(const Size& s, float mult)
	{
		return SizeMake(s.width * mult, s.height * mult);
	}
	
	static OA_RELEASE_INLINE Point g2d_sub(const Point& p1, const Point& p2)
	{
		return PointMake(p1.x - p2.x, p1.y - p2.y);
	}
	
	static OA_RELEASE_INLINE Vector2 g2d_sub(const Vector2& v1, const Vector2& v2)
	{
		return Vector2Make(v1.x - v2.x, v1.y - v2.y);
	}
	
	static OA_RELEASE_INLINE Point g2d_neg(const Point& p1)
	{
		return PointMake(-p1.x, -p1.y);
	}
	
	static OA_RELEASE_INLINE Point g2d_add(const Point& p1, const Point& p2)
	{
		return PointMake(p1.x + p2.x, p1.y + p2.y);
	}
	
	static OA_RELEASE_INLINE unsigned long ccNextPOT(unsigned long x)
	{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
	}
	
#define CC_SWAP( x, y )			\
({ __typeof__(x) temp  = (x);		\
x = y; y = temp;		\
})

	
	/** @def CC_BLEND_SRC
	 default gl blend src function. Compatible with premultiplied alpha images.
	 */
#if CC_OPTIMIZE_BLEND_FUNC_FOR_PREMULTIPLIED_ALPHA
#define CC_BLEND_SRC GL_ONE
#define CC_BLEND_DST GL_ONE_MINUS_SRC_ALPHA
#else
#define CC_BLEND_SRC GL_SRC_ALPHA
#define CC_BLEND_DST GL_ONE_MINUS_SRC_ALPHA
#endif // ! CC_OPTIMIZE_BLEND_FUNC_FOR_PREMULTIPLIED_ALPHA
	
#define CC_GLVIEW					EAGLView
#define ccglOrtho					glOrthof
#define	ccglClearDepth				glClearDepthf
#define ccglGenerateMipmap			glGenerateMipmapOES
#define ccglGenFramebuffers			glGenFramebuffersOES
#define ccglBindFramebuffer			glBindFramebufferOES
#define ccglFramebufferTexture2D	glFramebufferTexture2DOES
#define ccglDeleteFramebuffers		glDeleteFramebuffersOES
#define ccglCheckFramebufferStatus	glCheckFramebufferStatusOES
#define ccglTranslate				glTranslatef
	
#define CC_GL_FRAMEBUFFER			GL_FRAMEBUFFER_OES
#define CC_GL_FRAMEBUFFER_BINDING	GL_FRAMEBUFFER_BINDING_OES
#define CC_GL_COLOR_ATTACHMENT0		GL_COLOR_ATTACHMENT0_OES
#define CC_GL_FRAMEBUFFER_COMPLETE	GL_FRAMEBUFFER_COMPLETE_OES
	
#define CC_ENABLE_DEFAULT_GL_STATES() {				\
glEnableClientState(GL_VERTEX_ARRAY);			\
glEnableClientState(GL_COLOR_ARRAY);			\
glEnableClientState(GL_TEXTURE_COORD_ARRAY);	\
glEnable(GL_TEXTURE_2D);						\
}
	
#define CC_DISABLE_DEFAULT_GL_STATES() {			\
glDisable(GL_TEXTURE_2D);						\
glDisableClientState(GL_TEXTURE_COORD_ARRAY);	\
glDisableClientState(GL_COLOR_ARRAY);			\
glDisableClientState(GL_VERTEX_ARRAY);			\
}

#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
	
	static OA_RELEASE_INLINE float clampf(float value, float min_inclusive, float max_inclusive)
	{
		if (min_inclusive > max_inclusive) {
			CC_SWAP(min_inclusive,max_inclusive);
		}
		return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
	}
	
	static float OA_RELEASE_INLINE PointLength(const Point& p)
	{
		return sqrtf(p.x * p.x + p.y * p.y);
	}
	
	static Point OA_RELEASE_INLINE PointNormalize(const Point& p)
	{
		return g2d_mult(p, 1.0f / PointLength(p));
	}
}

/** @def CCRANDOM_MINUS1_1
 returns a random float between -1 and 1
 */
#define CCRANDOM_MINUS1_1() ((random() / (float)0x3fffffff )-1.0f)

/** @def CCRANDOM_0_1
 returns a random float between 0 and 1
 */
#define CCRANDOM_0_1() ((random() / (float)0x7fffffff ))


#endif
