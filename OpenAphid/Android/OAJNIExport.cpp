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

#include "org_openaphid_gl_AphidRenderer.h"
#include "org_openaphid_internal_AppDelegate.h"

#include "OAJNIExport.h"
#include "OAUtil.h"
#include "OAJNIUtil.h"

#include "ScriptBridge+Android.h"
#include "Director.h"
#include "Touch.h"

#include "OpenGL_Internal.h"

using namespace ATF;
using namespace Aphid;

namespace Aphid {
	static ScriptBridge* s_oa_script_bridge = 0;
}

///---------------------------------------------------------------------------------------------------------------------
/// JNI lifecycle
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	oa_info("JNI_OnLoad");
	JNI::setGlobalJavaVM(vm);
	return JNI_VERSION_1_2;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	oa_info("JNI_OnUnload");
	JNI::setGlobalJavaVM(0);
}

///---------------------------------------------------------------------------------------------------------------------
/// org_openaphid_gl_AphidRenderer.h
JNIEXPORT void JNICALL Java_org_openaphid_gl_AphidRenderer_nativeOnSurfaceCreated
  (JNIEnv *, jobject)
{
	OA_TODO();
	ASSERT(Director::sharedDirector());
}

JNIEXPORT void JNICALL Java_org_openaphid_gl_AphidRenderer_nativeOnSurfaceChanged
  (JNIEnv *, jobject, jint width, jint height)
{
	OA_TODO();
	Director::sharedDirector()->setSurfaceSize(width, height);
}

JNIEXPORT void JNICALL Java_org_openaphid_gl_AphidRenderer_nativeOnDrawFrame
  (JNIEnv *, jobject)
{
	//OA_TODO();
	if (Aphid::Director::sharedDirector() != 0)
		Aphid::Director::sharedDirector()->drawScene();
	CHECK_GL_ERROR(); //TODO: disable it in release mode?
}

JNIEXPORT void JNICALL Java_org_openaphid_gl_AphidRenderer_nativeOnSurfaceDestroyed
  (JNIEnv *, jobject)
{
	OA_TODO();
	
	if (Aphid::s_oa_script_bridge) {
		OA_DELETE(Aphid::s_oa_script_bridge);
	}
}

JNIEXPORT jboolean JNICALL Java_org_openaphid_gl_AphidRenderer_nativeEvaluateScriptFile
  (JNIEnv *env, jobject, jstring filename)
{
	RELEASE_ASSERT(Aphid::s_oa_script_bridge);
	
	return Aphid::s_oa_script_bridge->evaluateScriptFile(JNI::toString(env, filename)) ? JNI_TRUE : JNI_FALSE;
}


JNIEXPORT void JNICALL Java_org_openaphid_gl_AphidRenderer_nativeOnSingleTouch
  (JNIEnv *env, jobject, jint eventHash, jlong eventTime, jint phase, jobject jtouch)
{
	TouchDispatcher::sharedDispatcher()->handleAndroidSingleTouch(env, eventHash, eventTime, (EventFlag)phase, jtouch);
}

///---------------------------------------------------------------------------------------------------------------------
/// org_openaphid_internal_AppDelegate.h
JNIEXPORT void JNICALL Java_org_openaphid_internal_AppDelegate_nativeInitialize
  (JNIEnv *env, jclass, jboolean mode, jstring urlString)
{
	RELEASE_ASSERT(Aphid::s_oa_script_bridge == 0);
	
	Aphid::s_oa_script_bridge = new ScriptBridge();
	Aphid::s_oa_script_bridge->initialize();
	
	Aphid::s_oa_script_bridge->enableDeveloperMode(mode ? true : false);
	Aphid::s_oa_script_bridge->setBaseUrlString(JNI::toString(env, urlString));
}
