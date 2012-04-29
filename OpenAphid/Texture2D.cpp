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

#include <config.h>
#include "Texture2D.h"
#include "AJTexture2D.h"
#include "OAUtil.h"

#include "G2DMacros.h"
#include "G2DConfiguration.h"

#include "runtime/AJObject.h"

#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGBitmapContext.h>

namespace Aphid {
	TextAlignment toTextAlignment(const AJ::UString& s)
	{
		if (s == "left")
			return TextAlignmentLeft;
		else if (s == "center")
			return TextAlignmentCenter;
		else if (s == "right")
			return TextAlignmentRight;
		
		if (s.size() > 0)
			oa_info("unknown text alignment %s, use default value [left]", s.UTF8String().data());
		return TextAlignmentLeft;
	}
	
	LineBreakMode toLineBreakMode(const AJ::UString& s)
	{
		if (s == "word-wrap")
			return LineBreakModeWordWrap;
		else if (s == "character-wrap")
			return LineBreakModeCharacterWrap;
		else if (s == "clip")
			return LineBreakModeClip;
		else if (s == "head-truncation")
			return LineBreakModeHeadTruncation;
		else if (s == "tail-truncation")
			return LineBreakModeTailTruncation;
		else if (s == "middle-truncation")
			return LineBreakModeMiddleTruncation;
		
		if (s.size() > 0)
			oa_info("unknown line break mode %s, use default value [clip]", s.UTF8String().data());
		
		return LineBreakModeClip;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Texture2D::Texture2D(bool antialias)
	: m_name(0)
	, m_size(SizeZero)
	, m_width(0)
	, m_height(0)
	, m_format(kCCTexture2DPixelFormat_Automatic)
	, m_maxS(0)
	, m_maxT(0)
	, m_hasPremultipliedAlpha(false)
	, m_antialias(antialias)
	{
		LEAK_DETECT_INC("Texture2D");
	}
	
	Texture2D::~Texture2D()
	{
		//TODO:
		if(m_name)
			glDeleteTextures(1, &m_name);
		
		LEAK_DETECT_DEC("Texture2D");
	}
	
	PassRefPtr<Texture2D> Texture2D::create(CGImageRef imageRef, bool antialias)
	{
		RefPtr<Texture2D> texture = create(antialias);
		texture->initWithImage(imageRef);
		return texture.release();
	}
	
	ATF::PassRefPtr<Texture2D> Texture2D::create(const AJ::UString &url, bool antialias)
	{
		return create(url.UTF8String().data(), antialias);
	}
	
	PassRefPtr<Texture2D> Texture2D::create(const void* data, CCTexture2DPixelFormat pixelFormat, int width, int height, const Size& size)
	{
		RefPtr<Texture2D> texture = create();
		
		texture->initWithData(data, pixelFormat, width, height, size);
		
		return texture.release();
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	//Getter
	
	Size Texture2D::contentSize() const
	{
		return toPoints(m_size);
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	//Drawing
	
	void Texture2D::drawAtPoint(const Point& point)
	{
		if (!m_name)
			return;
		
		GLfloat		coordinates[] = { 0.0f,	m_maxT,
			m_maxS,	m_maxT,
			0.0f,	0.0f,
			m_maxS,	0.0f };
		GLfloat		width = (GLfloat)m_width * m_maxS,
		height = (GLfloat)m_height * m_maxT;
		
		GLfloat		vertices[] = {	point.x,			point.y,	0.0f,
			width + point.x,	point.y,	0.0f,
			point.x,			height  + point.y,	0.0f,
			width + point.x,	height  + point.y,	0.0f };
		
		glBindTexture(GL_TEXTURE_2D, m_name);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	void Texture2D::drawInRect(const Rect& rect)
	{
		if (!m_name)
			return;
		
		GLfloat	 coordinates[] = {  0.0f,	m_maxT,
			m_maxS,	m_maxT,
			0.0f,	0.0f,
			m_maxS,	0.0f  };
		GLfloat	vertices[] = {	rect.origin.x,							rect.origin.y,							/*0.0f,*/
			rect.origin.x + rect.size.width,		rect.origin.y,							/*0.0f,*/
			rect.origin.x,							rect.origin.y + rect.size.height,		/*0.0f,*/
			rect.origin.x + rect.size.width,		rect.origin.y + rect.size.height,		/*0.0f*/ };
		
		glBindTexture(GL_TEXTURE_2D, m_name);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	//GLFilter	
	
	void Texture2D::setTexParameters(ccTexParams* texParams)
	{
		if (!m_name)
			return;
		
		ASSERT( (m_width == ccNextPOT(m_width) && m_height == ccNextPOT(m_height)) ||
					 (texParams->wrapS == GL_CLAMP_TO_EDGE && texParams->wrapT == GL_CLAMP_TO_EDGE));
		glBindTexture( GL_TEXTURE_2D, m_name );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams->minFilter );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams->magFilter );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams->wrapS );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams->wrapT );
	}
	
	void Texture2D::setAntiAliasTexParameters()
	{
		ccTexParams texParams = { GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
		setTexParameters(&texParams);
		m_antialias = true;
	}
	
	void Texture2D::setAliasTexParameters()
	{
		ccTexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
		setTexParameters(&texParams);
		m_antialias = false;
	}
	
	void Texture2D::generateMipmap()
	{
		if (!m_name)
			return;
		
		ASSERT( m_width == ccNextPOT(m_width) && m_height == ccNextPOT(m_height));
		glBindTexture( GL_TEXTURE_2D, m_name);
		ccglGenerateMipmap(GL_TEXTURE_2D);
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	//PixelFormat
	
	CCTexture2DPixelFormat Texture2D::s_default_alpha_pixel_format = kCCTexture2DPixelFormat_Default;
	
	int Texture2D::bitsPerPixelForFormat()
	{
		int ret=0;
		
		switch (m_format) {
			case kCCTexture2DPixelFormat_RGBA8888:
				ret = 32;
				break;
			case kCCTexture2DPixelFormat_RGB565:
				ret = 16;
				break;
			case kCCTexture2DPixelFormat_A8:
				ret = 8;
				break;
			case kCCTexture2DPixelFormat_RGBA4444:
				ret = 16;
				break;
			case kCCTexture2DPixelFormat_RGB5A1:
				ret = 16;
				break;
			case kCCTexture2DPixelFormat_PVRTC4:
				ret = 4;
				break;
			case kCCTexture2DPixelFormat_PVRTC2:
				ret = 2;
				break;
			case kCCTexture2DPixelFormat_I8:
				ret = 8;
				break;
			case kCCTexture2DPixelFormat_AI88:
				ret = 16;
				break;
			default:
				ret = -1;
				oa_error("bitsPerPixelForFormat: %ld, unrecognised pixel format", (long)m_format);
				ASSERT_NOT_REACHED();
				break;
		}
		return ret;
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	//Private
	
	void Texture2D::initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, int width, int height, const Size& size)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glGenTextures(1, &m_name);
		glBindTexture(GL_TEXTURE_2D, m_name);
		
		if (m_antialias)
			setAntiAliasTexParameters();
		else
			setAliasTexParameters();
		
		// Specify OpenGL texture image
		
		switch(pixelFormat)
		{
			case kCCTexture2DPixelFormat_RGBA8888:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				break;
			case kCCTexture2DPixelFormat_RGBA4444:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
				break;
			case kCCTexture2DPixelFormat_RGB5A1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
				break;
			case kCCTexture2DPixelFormat_RGB565:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
				break;
			case kCCTexture2DPixelFormat_AI88:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei) width, (GLsizei) height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
				break;
			case kCCTexture2DPixelFormat_A8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei) width, (GLsizei) height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
				break;
			default:
				oa_error("Unknown pixel format: %d", static_cast<int>(pixelFormat));
				
		}
		
		m_size = size;
		m_width = width;
		m_height = height;
		m_format = pixelFormat;
		m_maxS = size.width / (float)width;
		m_maxT = size.height / (float)height;
		
		m_hasPremultipliedAlpha = false;
	}
	
	void Texture2D::initWithImage(CGImageRef CGImage)
	{
		int				POTWide, POTHigh;
		CGContextRef			context = nil;
		void*					data = nil;;
		CGColorSpaceRef			colorSpace;
		void*					tempData;
		unsigned int*			inPixel32;
		unsigned short*			outPixel16;
		bool					hasAlpha;
		CGImageAlphaInfo		info;
		Size					imageSize;
		CCTexture2DPixelFormat	pixelFormat;
		
		if(CGImage == NULL) {
			ASSERT_NOT_REACHED();
		}
		
#if CC_TEXTURE_NPOT_SUPPORT
		if( G2DConfiguration::supportsNPOT()] ) {
			POTWide = CGImageGetWidth(CGImage);
			POTHigh = CGImageGetHeight(CGImage);
			
		} else 
#endif
		{
			POTWide = ccNextPOT(CGImageGetWidth(CGImage));
			POTHigh = ccNextPOT(CGImageGetHeight(CGImage));
		}
		
		int maxTextureSize = G2DConfiguration::maxTextureSize();
		if( POTHigh > maxTextureSize || POTWide > maxTextureSize ) {
			oa_error("WARNING: Image (%lu x %lu) is bigger than the supported %ld x %ld",
							 (long)POTWide, (long)POTHigh,
							 (long)maxTextureSize, (long)maxTextureSize);
			ASSERT_NOT_REACHED();
			return;
		}
		
		info = CGImageGetAlphaInfo(CGImage);
		hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? true : false);
		
		size_t bpp = CGImageGetBitsPerComponent(CGImage);
		colorSpace = CGImageGetColorSpace(CGImage);
		
		if(colorSpace) {
			if(hasAlpha || bpp >= 8)
				pixelFormat = Texture2D::defaultAlphaPixelFormat();
			else {
				oa_info("Texture2D: Using RGB565 texture since image has no alpha");
				pixelFormat = kCCTexture2DPixelFormat_RGB565;
			}
		} else {
			// NOTE: No colorspace means a mask image
			oa_info("Texture2D: Using A8 texture since image is a mask");
			pixelFormat = kCCTexture2DPixelFormat_A8;
		}
		
		imageSize = CGSizeMake(CGImageGetWidth(CGImage), CGImageGetHeight(CGImage));
		
		// Create the bitmap graphics context
		
		switch(pixelFormat) {          
			case kCCTexture2DPixelFormat_RGBA8888:
			case kCCTexture2DPixelFormat_RGBA4444:
			case kCCTexture2DPixelFormat_RGB5A1:
				colorSpace = CGColorSpaceCreateDeviceRGB();
				data = malloc(POTHigh * POTWide * 4);
				info = hasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast; 
				//			info = kCGImageAlphaPremultipliedLast;  // issue #886. This patch breaks BMP images.
				context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, 4 * POTWide, colorSpace, info | kCGBitmapByteOrder32Big);				
				CGColorSpaceRelease(colorSpace);
				break;
				
			case kCCTexture2DPixelFormat_RGB565:
				colorSpace = CGColorSpaceCreateDeviceRGB();
				data = malloc(POTHigh * POTWide * 4);
				info = kCGImageAlphaNoneSkipLast;
				context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, 4 * POTWide, colorSpace, info | kCGBitmapByteOrder32Big);
				CGColorSpaceRelease(colorSpace);
				break;
			case kCCTexture2DPixelFormat_A8:
				data = malloc(POTHigh * POTWide);
				info = kCGImageAlphaOnly; 
				context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, POTWide, NULL, info);
				break;                    
			default:
				oa_error("Invalid pixel format");
				ASSERT_NOT_REACHED();
		}
		
		
		CGContextClearRect(context, CGRectMake(0, 0, POTWide, POTHigh));
		CGContextTranslateCTM(context, 0, POTHigh - imageSize.height);
		CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(CGImage), CGImageGetHeight(CGImage)), CGImage);
		
		// Repack the pixel data into the right format
		
		if(pixelFormat == kCCTexture2DPixelFormat_RGB565) {
			//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
			tempData = malloc(POTHigh * POTWide * 2);
			inPixel32 = (unsigned int*)data;
			outPixel16 = (unsigned short*)tempData;
			for(unsigned int i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
				*outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
			free(data);
			data = tempData;
			
		}
		else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444) {
			//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
			tempData = malloc(POTHigh * POTWide * 2);
			inPixel32 = (unsigned int*)data;
			outPixel16 = (unsigned short*)tempData;
			for(unsigned int i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
				*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
				((((*inPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
				((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
				((((*inPixel32 >> 24) & 0xFF) >> 4) << 0); // A
			
			
			free(data);
			data = tempData;
			
		}
		else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1) {
			//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
			tempData = malloc(POTHigh * POTWide * 2);
			inPixel32 = (unsigned int*)data;
			outPixel16 = (unsigned short*)tempData;
			for(unsigned int i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
				*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
				((((*inPixel32 >> 8) & 0xFF) >> 3) << 6) | // G
				((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
				((((*inPixel32 >> 24) & 0xFF) >> 7) << 0); // A
			
			
			free(data);
			data = tempData;
		}
		
		initWithData(data, pixelFormat, POTWide, POTHigh, imageSize);
		
		// should be after calling super init
		m_hasPremultipliedAlpha = (info == kCGImageAlphaPremultipliedLast || info == kCGImageAlphaPremultipliedFirst);
		
		CGContextRelease(context);
		releaseData(data);
	}

	///-------------------------------------------------------------------------------------------------------------------
	/// Grabber
	Grabber::Grabber()
	{
		LEAK_DETECT_INC("Grabber");
		
		ccglGenFramebuffers(1, &m_fbo);
	}
	
	Grabber::~Grabber()
	{
		ccglDeleteFramebuffers(1, &m_fbo);
		LEAK_DETECT_DEC("Grabber");
	}
	
	void Grabber::grab(Aphid::Texture2D *texture)
	{
		glGetIntegerv(CC_GL_FRAMEBUFFER_BINDING, &m_oldFBO);
		
		// bind
		ccglBindFramebuffer(CC_GL_FRAMEBUFFER, m_fbo);
		
		// associate texture with FBO
		ccglFramebufferTexture2D(CC_GL_FRAMEBUFFER, CC_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->name(), 0);
		
		// check if it worked (probably worth doing :) )
		GLuint status = ccglCheckFramebufferStatus(CC_GL_FRAMEBUFFER);
		if (status != CC_GL_FRAMEBUFFER_COMPLETE) {
			oa_error("Could not attach texture to framebuffer");
			CRASH(); //TODO: throw an exception?
		}
		
		ccglBindFramebuffer(CC_GL_FRAMEBUFFER, m_oldFBO);
	}
	
	void Grabber::beforeRender(Aphid::Texture2D *texture)
	{
		UNUSED_PARAM(texture);
		
		glGetIntegerv(CC_GL_FRAMEBUFFER_BINDING, &m_oldFBO);
		ccglBindFramebuffer(CC_GL_FRAMEBUFFER, m_fbo);
		
		// BUG TODO: doesn't work with RGB565.
		
		
		glClearColor(0,0,0,0);
		
		// BUG #631: To fix #631, uncomment the lines with #631
		// Warning: But it CCGrabber won't work with 2 effects at the same time
		//	glClearColor(0.0f,0.0f,0.0f,1.0f);	// #631
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		
		//	glColorMask(TRUE, TRUE, TRUE, FALSE);	// #631
	}
	
	void Grabber::afterRender(Aphid::Texture2D *texture)
	{
		UNUSED_PARAM(texture);
		
		ccglBindFramebuffer(CC_GL_FRAMEBUFFER, m_oldFBO);
		//	glColorMask(TRUE, TRUE, TRUE, TRUE);	// #631
	}	
} //namespace Aphid
