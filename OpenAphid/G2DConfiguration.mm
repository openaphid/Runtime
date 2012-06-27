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

/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2008-2010 Ricardo Quesada
 * Copyright (c) 2011 Zynga Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "G2DConfiguration.h"
#include "OAUtil.h"
#include "OpenGL_Internal.h"

#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace Aphid {
	bool G2DConfiguration::s_inited = false;
	
	GLint G2DConfiguration::s_maxTextureSize = 0;
	GLint G2DConfiguration::s_maxModelviewStackDepth = 0;
	bool G2DConfiguration::s_supportsPVRTC = false, G2DConfiguration::s_supportsNPOT = false, G2DConfiguration::s_supportsBGRA8888 = false, G2DConfiguration::s_supportsDiscardFramebuffer = false;
	unsigned int G2DConfiguration::s_OSVersion = 0;
	GLint G2DConfiguration::s_maxSamplesAllowed = 0;
	char* G2DConfiguration::s_glExtensions = 0;

#define LOG_DETAILS 0
	
#if LOG_DETAILS
#define LOG(...) oa_debug(__VA_ARGS__)
#else
#define LOG(...) do{}while(0)
#endif
	
	void G2DConfiguration::initConfiguration()
	{
		if (!s_inited) {
			
			// Obtain iOS version
			s_OSVersion = 0;
			NSString *OSVer = [[UIDevice currentDevice] systemVersion];

			NSArray *arr = [OSVer componentsSeparatedByString:@"."];		
			int idx=0x01000000;
			for( NSString *str in arr ) {
				int value = [str intValue];
				s_OSVersion += value * idx;
				idx = idx >> 8;
			}
			LOG("OS version: (0x%08x)", s_OSVersion);
			
			LOG("GL_VENDOR:   %s", glGetString(GL_VENDOR) );
			LOG("GL_RENDERER: %s", glGetString ( GL_RENDERER   ) );
			LOG("GL_VERSION:  %s", glGetString ( GL_VERSION    ) );
			
			s_glExtensions = (char*) glGetString(GL_EXTENSIONS);
			
			s_inited = true;
			
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s_maxTextureSize);
			glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &s_maxModelviewStackDepth);
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
			if( s_OSVersion >= kCCiOSVersion_4_0 )
				glGetIntegerv(GL_MAX_SAMPLES_APPLE, &s_maxSamplesAllowed);
			else
				s_maxSamplesAllowed = 0;
#elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
			glGetIntegerv(GL_MAX_SAMPLES, &maxSamplesAllowed_);
#endif
			
			s_supportsPVRTC = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
			s_supportsNPOT = checkForGLExtension("GL_APPLE_texture_2D_limited_npot");
#elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
			supportsNPOT_ = [self checkForGLExtension:@"GL_ARB_texture_non_power_of_two"];
#endif
			// It seems that somewhere between firmware iOS 3.0 and 4.2 Apple renamed
			// GL_IMG_... to GL_APPLE.... So we should check both names
			
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
			BOOL bgra8a = checkForGLExtension("GL_IMG_texture_format_BGRA8888");
			BOOL bgra8b = checkForGLExtension("GL_APPLE_texture_format_BGRA8888");
			s_supportsBGRA8888 = bgra8a | bgra8b;
#elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
			supportsBGRA8888_ = [self checkForGLExtension:@"GL_EXT_bgra"];
#endif
			
			s_supportsDiscardFramebuffer = checkForGLExtension("GL_EXT_discard_framebuffer");
			
			LOG("GL_MAX_TEXTURE_SIZE: %d", s_maxTextureSize);
			LOG("GL_MAX_MODELVIEW_STACK_DEPTH: %d", s_maxModelviewStackDepth);
			LOG("GL_MAX_SAMPLES: %d", s_maxSamplesAllowed);
			LOG("GL supports PVRTC: %s", (s_supportsPVRTC ? "YES" : "NO") );
			LOG("GL supports BGRA8888 textures: %s", (s_supportsBGRA8888 ? "YES" : "NO") );
			LOG("GL supports NPOT textures: %s", (s_supportsNPOT ? "YES" : "NO") );
			LOG("GL supports discard_framebuffer: %s", (s_supportsDiscardFramebuffer ? "YES" : "NO") );
			LOG("compiled with NPOT support: %s",
#if CC_TEXTURE_NPOT_SUPPORT
						"YES"
#else
						"NO"
#endif
						);
			LOG("compiled with VBO support in TextureAtlas : %s",
#if CC_USES_VBO
						"YES"
#else
						"NO"
#endif
						);
			
			LOG("compiled with Affine Matrix transformation in CCNode : %s",
#if CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
						"YES"
#else
						"NO"
#endif
						);
			
			LOG("compiled with Profiling Support: %s",
#if CC_ENABLE_PROFILERS
						
						"YES - *** Disable it when you finish profiling ***"
#else
						"NO"
#endif
						);
			
			CHECK_GL_ERROR();

			
			s_inited = true;
		}
	}
	
	bool G2DConfiguration::checkForGLExtension(const char* searchName)
	{
		if (!s_inited)
			initConfiguration();
		NSString *extensionsString = [NSString stringWithCString:s_glExtensions encoding:NSASCIIStringEncoding];
    NSArray *extensionsNames = [extensionsString componentsSeparatedByString:@" "];
    return [extensionsNames containsObject: [NSString stringWithCString:searchName encoding:NSASCIIStringEncoding]];
	}
}