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

#include "Director+Android.h"
#include "OAUtil.h"
#include "OAJNIUtil.h"

namespace Aphid {
	CCTexture2DPixelFormat Director::pixelFormat()
	{
		//TODO: read pixel format from java side
		return kCCTexture2DPixelFormat_RGB565;
	}
	
	DirectorAndroid::DirectorAndroid()
	{
		LEAK_DETECT_INC("DirectorAndroid");
	}
	
	DirectorAndroid::~DirectorAndroid()
	{
		LEAK_DETECT_DEC("DirectorAndroid");
	}
	
	void DirectorAndroid::setDeviceOrientation(ccDeviceOrientation orientation)
	{
		OA_TODO();
	}
	
	bool DirectorAndroid::enableRetinaDisplay(bool on)
	{
		return false;
	}
	
	bool DirectorAndroid::multipleTouchEnabled() const
	{
		return JNI::callStaticMethod<jboolean>(JNI::getJNIEnv(), JNI::Cache::s_appdelegate_jclass->global(), JNI::Cache::s_appdelegate_jmethod_isMultitouchEnabled) == JNI_TRUE ? true : false;
	}
	
	void DirectorAndroid::setMultipleTouchEnabled(bool on)
	{
		JNI::callStaticVoidMethod(JNI::getJNIEnv(), JNI::Cache::s_appdelegate_jclass->global(), JNI::Cache::s_appdelegate_jmethod_setMultitouchEnabled, on ? JNI_TRUE : JNI_FALSE);
	}
	
	void DirectorAndroid::startAnimation()
	{
		OA_TODO();
	}
	
	void DirectorAndroid::stopAnimation()
	{
		OA_TODO();
	}
	
	void DirectorAndroid::setAnimationInterval(double interval)
	{
		JNI::callStaticVoidMethod(JNI::getJNIEnv(), JNI::Cache::s_appdelegate_jclass->global(), JNI::Cache::s_appdelegate_jmethod_setFrameInterval, interval);
	}
	
	void DirectorAndroid::setSurfaceSize(int width, int height)
	{
		m_winSizeInPixels = m_winSizeInPoints = SizeMake(width, height);
		setGLDefaultValues();
		
		reshapeProjection(m_winSizeInPoints);
		avoidLayoutFlicker();
	}
}
