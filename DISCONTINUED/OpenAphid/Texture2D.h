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

#ifndef OpenAphid_Texture2D_h
#define OpenAphid_Texture2D_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "ccTypes.h"
#include "Font.h"

#if PLATFORM(IPHONE)
#include <CoreGraphics/CGImage.h> //TODO: iOS platform dependent
#elif PLATFORM(ANDROID)
#include "OAJNIUtil.h"
#endif

#ifdef __OBJC__
#import <UIKit/UIFont.h>
#endif

namespace Aphid {
	class AJTexture2D;
	
	typedef enum {
		kCCTexture2DPixelFormat_Automatic = 0,
		//! 32-bit texture: RGBA8888
		kCCTexture2DPixelFormat_RGBA8888,
		//! 16-bit texture without Alpha channel
		kCCTexture2DPixelFormat_RGB565,
		//! 8-bit textures used as masks
		kCCTexture2DPixelFormat_A8,
		//! 8-bit intensity texture
		kCCTexture2DPixelFormat_I8,
		//! 16-bit textures used as masks
		kCCTexture2DPixelFormat_AI88,
		//! 16-bit textures: RGBA4444
		kCCTexture2DPixelFormat_RGBA4444,
		//! 16-bit textures: RGB5A1
		kCCTexture2DPixelFormat_RGB5A1,	
		//! 4-bit PVRTC-compressed texture: PVRTC4
		kCCTexture2DPixelFormat_PVRTC4,
		//! 2-bit PVRTC-compressed texture: PVRTC2
		kCCTexture2DPixelFormat_PVRTC2,
		
		//! Default texture format: RGBA8888
		kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,
	} CCTexture2DPixelFormat;
	
	enum TextAlignment {
		TextAlignmentLeft = 0,
		TextAlignmentCenter,
		TextAlignmentRight
	};
	
	enum LineBreakMode {
		LineBreakModeWordWrap = 0,
    LineBreakModeCharacterWrap,
    LineBreakModeClip,
    LineBreakModeHeadTruncation,
    LineBreakModeTailTruncation,
    LineBreakModeMiddleTruncation,
	};
	
	typedef struct _ccTexParams {
		GLuint	minFilter;
		GLuint	magFilter;
		GLuint	wrapS;
		GLuint	wrapT;
	} ccTexParams;
	
	TextAlignment toTextAlignment(const AJ::UString& s);
	LineBreakMode toLineBreakMode(const AJ::UString& s);
	
	///-------------------------------------------------------------------------------------------------------------------
	class Texture2D : public RefCounted<Texture2D>, public DirectWrapper<AJTexture2D> {
	public:
		virtual ~Texture2D();
		
		static PassRefPtr<Texture2D> create(const char* url, bool antialias=true);
		static PassRefPtr<Texture2D> create(const AJ::UString& url, bool antialias=true);

		static PassRefPtr<Texture2D> createWithString(const AJ::UString& content, const Size& dimensions, Font* font, TextAlignment alignment, LineBreakMode lineBreakMode);
		static PassRefPtr<Texture2D> createWithString(const AJ::UString& content, const Size& dimensions, float fontSize);
		
#if PLATFORM(IPHONE)
		static PassRefPtr<Texture2D> create(CGImageRef imageRef, bool antialias=true);
#endif
		static PassRefPtr<Texture2D> create(const void* data, CCTexture2DPixelFormat pixelFormat, int width, int height, const Size& size);
		
		//Getter and setters
		int pixelWidth() const {return m_width;}
		int pixelHeight() const {return m_height;}
		Size contentSize() const;
		Size contentSizeInPixels() const {return m_size;}		
		GLuint name() const {return m_name;}		
		bool hasPremultipliedAlpha() const {return m_hasPremultipliedAlpha;}
		CCTexture2DPixelFormat pixelFormat() const {return m_format;}
		GLfloat maxS() const {return m_maxS;}
		GLfloat maxT() const {return m_maxT;}
		
		//Drawing
		void drawAtPoint(const Point& point);
		void drawInRect(const Rect& rect);
		
		//GLFilter
		void setAntiAliasTexParameters();
		void setTexParameters(ccTexParams* texParams);
		void setAliasTexParameters();
		void generateMipmap();
		
		//PixelFormat
		static CCTexture2DPixelFormat s_default_alpha_pixel_format;
		static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format) {s_default_alpha_pixel_format = format;}
		static CCTexture2DPixelFormat defaultAlphaPixelFormat() {return s_default_alpha_pixel_format;}
		int bitsPerPixelForFormat();
		
		//Utilities
		void releaseData(void* data) {OA_FREE(data);} //TODO: remove these meanless methods
		void* keepData(void* data, int length)
		{
			UNUSED_PARAM(length);
			return data;
		}
		
		bool antialias() const {return m_antialias;}
		
	protected:
		Texture2D(bool antialias=true);
		
		static PassRefPtr<Texture2D> create(bool antialias=true)
		{
			return adoptRef(new Texture2D(antialias));
		}
		
		void initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, int width, int height, const Size& size, bool isPOTData=true);

#if PLATFORM(IPHONE)
		void initWithImage(CGImageRef CGImage);
#elif PLATFORM(ANDROID)
		void initWithBitmap(jobject bitmap);
#endif
		
	private:
		GLuint m_name;
		Size m_size;
		int m_width;
		int m_height;
		
		CCTexture2DPixelFormat m_format;
		
		GLfloat m_maxS;
		GLfloat m_maxT;
		
		bool m_hasPremultipliedAlpha;
		bool m_antialias;
	}; // class Texture2D
	
	///-------------------------------------------------------------------------------------------------------------------
	class Grabber : public ATF::RefCounted<Grabber> {
		typedef Grabber Self;
	public:
		virtual ~Grabber();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		void grab(Texture2D* texture);
		void beforeRender(Texture2D* texture);
		void afterRender(Texture2D* texture);
		
	protected:
		Grabber();
		
		GLuint m_fbo;
		GLint m_oldFBO;
	};
	
}// namespace Aphid

#endif
