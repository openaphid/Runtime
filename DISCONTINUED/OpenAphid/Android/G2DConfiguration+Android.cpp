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
#include <AtomicString.h>
#include "OpenGLES.inc.h"
#include "G2DConfiguration.h"
#include "OpenGL_Internal.h"

namespace Aphid {
	bool G2DConfiguration::s_inited = false;

	GLint G2DConfiguration::s_maxTextureSize = 0;
	GLint G2DConfiguration::s_maxModelviewStackDepth = 0;
	bool G2DConfiguration::s_supportsPVRTC = false,
			G2DConfiguration::s_supportsNPOT = false,
			G2DConfiguration::s_supportsBGRA8888 = false,
			G2DConfiguration::s_supportsDiscardFramebuffer = false;
	unsigned int G2DConfiguration::s_OSVersion = 0;
	GLint G2DConfiguration::s_maxSamplesAllowed = 0;
	char* G2DConfiguration::s_glExtensions = 0;
	AtomicString G2DConfiguration::s_glExtensionsString = "";

	void G2DConfiguration::initConfiguration()
	{
		if (!s_inited) {
			//TODO get os version
			oa_debug("GL_VENDOR:   %s", glGetString(GL_VENDOR));
			oa_debug("GL_RENDERER: %s", glGetString(GL_RENDERER));
			oa_debug("GL_VERSION:  %s", glGetString(GL_VERSION));

			s_glExtensions = (char*) glGetString(GL_EXTENSIONS);
			s_glExtensionsString = s_glExtensions;

			s_inited = true;

			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s_maxTextureSize);
			glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &s_maxModelviewStackDepth);

			s_maxSamplesAllowed = 0; //always 0 on Android

			s_supportsPVRTC = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
			s_supportsNPOT = checkForGLExtension("GL_APPLE_texture_2D_limited_npot");
			bool bgra8a = checkForGLExtension("GL_IMG_texture_format_BGRA8888");
			bool bgra8b = checkForGLExtension("GL_APPLE_texture_format_BGRA8888");
			s_supportsBGRA8888 = bgra8a | bgra8b;

			s_supportsDiscardFramebuffer = checkForGLExtension("GL_EXT_discard_framebuffer");

			oa_debug("GL_MAX_TEXTURE_SIZE: %d", s_maxTextureSize);
			oa_debug("GL_MAX_MODELVIEW_STACK_DEPTH: %d", s_maxModelviewStackDepth);
			oa_debug("GL_MAX_SAMPLES: %d", s_maxSamplesAllowed);
			oa_debug("GL supports PVRTC: %s", (s_supportsPVRTC ? "YES" : "NO"));
			oa_debug(
					"GL supports BGRA8888 textures: %s", (s_supportsBGRA8888 ? "YES" : "NO"));
			oa_debug("GL supports NPOT textures: %s", (s_supportsNPOT ? "YES" : "NO"));
			oa_debug(
					"GL supports discard_framebuffer: %s", (s_supportsDiscardFramebuffer ? "YES" : "NO"));
			oa_debug("compiled with NPOT support: %s",
#if CC_TEXTURE_NPOT_SUPPORT
					"YES"
#else
					"NO"
#endif
					);
			oa_debug("compiled with VBO support in TextureAtlas : %s",
#if CC_USES_VBO
					"YES"
#else
					"NO"
#endif
			);

			oa_debug("compiled with Affine Matrix transformation in CCNode : %s",
#if CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
					"YES"
#else
					"NO"
#endif
					);

			oa_debug("compiled with Profiling Support: %s",
#if CC_ENABLE_PROFILERS
					"YES - *** Disable it when you finish profiling ***"
#else
					"NO"
#endif
					);

			CHECK_GL_ERROR();
		}
	}
	
	bool G2DConfiguration::checkForGLExtension(const char* searchName)
	{
		if (!s_inited)
			initConfiguration();
		
		return s_glExtensionsString.find(String::format("%s ", searchName), true) >= 0;
	}
}
