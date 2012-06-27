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
	
	void Texture2D::initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, int width, int height, const Size& size, bool isPOTData)
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
				if (isPOTData)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}
				break;
			case kCCTexture2DPixelFormat_RGBA4444:
				if (isPOTData)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 0);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
				}
				break;
			case kCCTexture2DPixelFormat_RGB5A1:
				if (isPOTData)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 0);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
				}
				break;
			case kCCTexture2DPixelFormat_RGB565:
				if (isPOTData)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 0);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
				}
				break;
			case kCCTexture2DPixelFormat_AI88:
				if (isPOTData)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei) width, (GLsizei) height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei) width, (GLsizei) height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 0);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
				}
				break;
			case kCCTexture2DPixelFormat_A8:
				if (isPOTData)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei) width, (GLsizei) height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei) width, (GLsizei) height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_ALPHA, GL_UNSIGNED_BYTE, data);
				}
				break;
			default:
				oa_error("Unknown pixel format: %d", static_cast<int>(pixelFormat));
				break;
		}
		
		m_size = size;
		m_width = width;
		m_height = height;
		m_format = pixelFormat;
		m_maxS = size.width / (float)width;
		m_maxT = size.height / (float)height;
		
		m_hasPremultipliedAlpha = false;
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
