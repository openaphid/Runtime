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
#include <android/bitmap.h>
#include "G2DConfig.h"
#include "Texture2D.h"
#include "G2DMacros.h"
#include "OAGlobalObject.h"
#include "NamespaceG2D.h"
#include "G2DConfiguration.h"
#include "ResourceManager.h"
#include "AJOABinding.h"

#define USE_TEXT_WITH_A8_TEXTURES 0

namespace Aphid {
	using JNI::LocalObject;
	
	ATF::PassRefPtr<Texture2D> Texture2D::create(const char *url, bool antialias)
	{
		OwnPtr<LocalObject> bitmap = ResourceManager::sharedManager()->loadBitmapInBundle(url);
		RefPtr<Texture2D> texture = create(antialias);
		
		if (bitmap->local()) {
			texture->initWithBitmap(bitmap->local());		
			JNI::callVoidMethod(JNI::getJNIEnv(), bitmap->local(), JNI::Cache::s_bitmap_jmethod_recycle);
		}
		return texture.release();
	}

	PassRefPtr<Texture2D> Texture2D::createWithString(const AJ::UString& text, const Size& d, Font* f,
			TextAlignment alignment, LineBreakMode lineBreakMode)
	{
		ASSERT(f);

		JNIEnv* env = JNI::getJNIEnv();
		
		OwnPtr<JNI::LocalString> jtext= JNI::toJNIString(env, toString(text));
		OwnPtr<JNI::LocalString> jfontname = JNI::toJNIString(env, toString(f->fontName()));
		
		OwnPtr<LocalObject> bitmap = JNI::callStaticObjectMethod(
				env, 
				JNI::Cache::s_ui_jclass->global(), 
				JNI::Cache::s_ui_jmethod_stringToBitmap, 
				jtext->local(), 
				jfontname->local(), 
				(jfloat)f->fontSize(), 
				d.width, 
				d.height, 
				TextAlignmentLeft, 
				LineBreakModeWordWrap
				);
		
		RefPtr<Texture2D> texture = Texture2D::create(true);
		
		if (bitmap->local()) {
			texture->initWithBitmap(bitmap->local());		
			JNI::callVoidMethod(JNI::getJNIEnv(), bitmap->local(), JNI::Cache::s_bitmap_jmethod_recycle);
		}
		return texture.release();
	}

	PassRefPtr<Texture2D> Texture2D::createWithString(const AJ::UString& content, const Size &dimensions,
			float fontSize)
	{
		RefPtr<Font> font = Font::create(fontSize);
		return createWithString(content, dimensions, font.get(), TextAlignmentLeft, LineBreakModeHeadTruncation);
	}
	
	void Texture2D::initWithBitmap(jobject bitmap)
	{
		if (!bitmap)
			return;
		
		int potW = 0;
		int potH = 0;
		
		JNIEnv* env = JNI::getJNIEnv();
		AndroidBitmapInfo info;
		
		int infoCheck = AndroidBitmap_getInfo(env, bitmap, &info);
		
		if (infoCheck == ANDROID_BITMAP_RESUT_SUCCESS) {
			potW = ccNextPOT(info.width);
			potH = ccNextPOT(info.height);
			
			int maxTextureSize = G2DConfiguration::maxTextureSize();
			if (potW > maxTextureSize || potH > maxTextureSize) {
				oa_error("Fatal: Image (%d x %d) is bigger than the supported maxTextureSize (%d)", potW, potH, maxTextureSize);
				ASSERT_NOT_REACHED();
				return;
			}
			
			CCTexture2DPixelFormat pixelFormat = kCCTexture2DPixelFormat_Default;
			switch(info.format) {
			case ANDROID_BITMAP_FORMAT_NONE:
				pixelFormat = kCCTexture2DPixelFormat_Default;
				break;
			case ANDROID_BITMAP_FORMAT_RGBA_8888:
				pixelFormat = kCCTexture2DPixelFormat_RGBA8888;
				break;
			case ANDROID_BITMAP_FORMAT_RGBA_4444:
				pixelFormat = kCCTexture2DPixelFormat_RGBA4444;
				break;
			case ANDROID_BITMAP_FORMAT_RGB_565:
				pixelFormat = kCCTexture2DPixelFormat_RGB565;
				break;
			case ANDROID_BITMAP_FORMAT_A_8:
				pixelFormat = kCCTexture2DPixelFormat_A8;
				break;
			default:
				oa_warn("Unknown bitmap format: %d", info.format);
				break;
			}
			
			void* data = 0;
			int lockData = AndroidBitmap_lockPixels(env, bitmap, &data);
			if (lockData == ANDROID_BITMAP_RESUT_SUCCESS) 
				initWithData(data, pixelFormat, potW, potH, SizeMake(info.width, info.height), info.width == potW && info.height == potH);
			else
				oa_error("Failed to lock bitmap data, error is %d", lockData);
			AndroidBitmap_unlockPixels(env, bitmap);
		} else
			oa_error("Failed to get info from bitmap, error code is %d", infoCheck);
	}
}
