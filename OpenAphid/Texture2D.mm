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
}
