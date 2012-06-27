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

#ifndef OpenAphid_G2DConfiguration_h
#define OpenAphid_G2DConfiguration_h

#include "G2DConfig.h"
#include "ccTypes.h"

namespace Aphid {
#if PLATFORM(IPHONE)
	enum {
		kCCiOSVersion_3_0   = 0x03000000,
		kCCiOSVersion_3_1   = 0x03010000,
		kCCiOSVersion_3_1_1 = 0x03010100,
		kCCiOSVersion_3_1_2 = 0x03010200,
		kCCiOSVersion_3_1_3 = 0x03010300,
		kCCiOSVersion_3_2   = 0x03020000,
		kCCiOSVersion_3_2_1 = 0x03020100,
		kCCiOSVersion_4_0   = 0x04000000,
		kCCiOSVersion_4_0_1 = 0x04000100,
		kCCiOSVersion_4_1   = 0x04010000,
		kCCiOSVersion_4_2   = 0x04020000,
		kCCiOSVersion_4_3   = 0x04030000,
		kCCiOSVersion_4_3_1 = 0x04030100,
		kCCiOSVersion_4_3_2 = 0x04030200,
		kCCiOSVersion_4_3_3 = 0x04030300,
		
		kCCMacVersion_10_5  = 0x0a050000,
		kCCMacVersion_10_6  = 0x0a060000,
		kCCMacVersion_10_7  = 0x0a070000,
	};
#elif PLATFORM(ANDROID)
	//TODO
#else
#error Unsupported platform
#endif

	class AtomicString;
	
	class G2DConfiguration {
	public:
		static GLint maxTextureSize()
		{
			if (!s_inited)
				initConfiguration();
			return s_maxTextureSize;
		}

		static GLint maxModelviewStackDepth()
		{
			if (!s_inited)
				initConfiguration();
			return s_maxModelviewStackDepth;
		}

		static bool supportsNPOT()
		{
			if (!s_inited)
				initConfiguration();
			return s_supportsNPOT;
		}

		static bool supportsPVRTC()
		{
			if (!s_inited)
				initConfiguration();
			return s_supportsPVRTC;
		}

		static bool supportsBGRA8888()
		{
			if (!s_inited)
				initConfiguration();
			return s_supportsBGRA8888;
		}

		static bool supportsDiscardFramebuffer()
		{
			if (!s_inited)
				initConfiguration();
			return s_supportsDiscardFramebuffer;
		}
		
		static bool checkForGLExtension(const char* searchName);
		
	private:
		static void initConfiguration();
		static bool s_inited;
		static GLint s_maxTextureSize;
		static GLint s_maxModelviewStackDepth;
		static bool s_supportsPVRTC, s_supportsNPOT, s_supportsBGRA8888, s_supportsDiscardFramebuffer;
		static unsigned int s_OSVersion;
		static GLint s_maxSamplesAllowed;
		static char* s_glExtensions;
		static AtomicString s_glExtensionsString;
	};
}

#endif
