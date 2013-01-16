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
#include "G2DConfig.h"
#include "Texture2D.h"
#include "G2DMacros.h"
#include "OAGlobalObject.h"
#include "NamespaceG2D.h"
#include "G2DConfiguration.h"

#import <UIKit/UIKit.h>
#import <CoreGraphics/CGContext.h>
#import <CoreGraphics/CGBitmapContext.h>

#include "iOSUtils.h"
#include "ResourceManager.h"

#if (defined(__ARM_NEON__) || TARGET_IPHONE_SIMULATOR) && CC_USE_LA88_LABELS_ON_NEON_ARCH
#define USE_TEXT_WITH_A8_TEXTURES 0

#else
#define USE_TEXT_WITH_A8_TEXTURES 1
#endif

namespace Aphid {
	ATF::PassRefPtr<Texture2D> Texture2D::create(const char *url, bool antialias)
	{
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		UIImage* image = ResourceManager::sharedManager()->loadUIImageInBundle(url);
		PassRefPtr<Texture2D> texture2d = create([image CGImage], antialias);
		[pool release];
		return texture2d;
	}
	
	PassRefPtr<Texture2D> Texture2D::createWithString(const AJ::UString& text, const Size& d, Font* f, TextAlignment alignment, LineBreakMode lineBreakMode)
	{
		ASSERT(f);
		
		UIFont* font = toUIFont(*f);
		
		NSString * string = toNSString(text);
		
		Size dimensions = d;
		
		if (SizeEqual(dimensions, SizeZero))
			dimensions = [string sizeWithFont:font 
											constrainedToSize:SizeMake(G2DConfiguration::maxTextureSize(), G2DConfiguration::maxTextureSize()) 
													lineBreakMode:(UILineBreakMode)lineBreakMode]; //TODO: enhancement? support specified maxWidth?
		
		if (SizeEqual(dimensions, SizeZero))
			return create();
		
		NSUInteger POTWide = ccNextPOT(dimensions.width);
		NSUInteger POTHigh = ccNextPOT(dimensions.height);
		unsigned char*			data;
		
		CGContextRef			context;
		CGColorSpaceRef			colorSpace;
		
#if USE_TEXT_WITH_A8_TEXTURES
		data = (unsigned char*)calloc(POTHigh, POTWide);
#else
		data = calloc(POTHigh, POTWide * 2);
#endif
		
		colorSpace = CGColorSpaceCreateDeviceGray();
		context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, POTWide, colorSpace, kCGImageAlphaNone);  //TODO: why use POTWide asbytesPerRow
		CGColorSpaceRelease(colorSpace);
		
		if( ! context ) {
			OA_FREE(data);
			oa_error("Can't create bitmap context");
			ASSERT_NOT_REACHED();
			return 0;
		}
		
		CGContextSetGrayFillColor(context, 1.0f, 1.0f);
		CGContextTranslateCTM(context, 0.0f, POTHigh);
		CGContextScaleCTM(context, 1.0f, -1.0f); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
		
		UIGraphicsPushContext(context);
		
		// normal fonts
		if( [font isKindOfClass:[UIFont class] ] )
			[string drawInRect:CGRectMake(0, 0, dimensions.width, dimensions.height) withFont:font lineBreakMode:(UILineBreakMode)lineBreakMode alignment:(UITextAlignment)alignment]; //TODO: ugly casting...
		
#if CC_FONT_LABEL_SUPPORT
		else // ZFont class 
			[string drawInRect:CGRectMake(0, 0, dimensions.width, dimensions.height) withZFont:uifont lineBreakMode:UILineBreakModeTailTruncation alignment:UITextAlignmentCenter];
#endif
		
		UIGraphicsPopContext();
		
#if USE_TEXT_WITH_A8_TEXTURES
		RefPtr<Texture2D> texture = create();
		
		texture->initWithData(data, kCCTexture2DPixelFormat_A8, POTWide, POTHigh, dimensions);
		
#else // ! USE_TEXT_WITH_A8_TEXTURES
		NSUInteger textureSize = POTWide*POTHigh;
		unsigned short *la88_data = (unsigned short*)data;
		for(int i = textureSize-1; i>=0; i--) //Convert A8 to AI88
			la88_data[i] = (data[i] << 8) | 0xff;
		
		self = [self initWithData:data pixelFormat:kCCTexture2DPixelFormat_AI88 pixelsWide:POTWide pixelsHigh:POTHigh contentSize:dimensions];
#endif // ! USE_TEXT_WITH_A8_TEXTURES
		
		CGContextRelease(context);
		texture->releaseData(data);
		return texture.release();
	}
	
	ATF::PassRefPtr<Texture2D> Texture2D::createWithString(const AJ::UString& content, const Size &dimensions, float fontSize)
	{
		RefPtr<Font> font = Font::create(fontSize);
		return createWithString(content, dimensions, font.get(), TextAlignmentLeft, LineBreakModeHeadTruncation);
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

	PassRefPtr<Texture2D> Texture2D::create(CGImageRef imageRef, bool antialias)
	{
		RefPtr<Texture2D> texture = create(antialias);
		texture->initWithImage(imageRef);
		return texture.release();
	}
}
