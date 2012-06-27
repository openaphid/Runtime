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
#include <stdarg.h>
#include "OAJNIUtil.h"
#include "OAUtil.h"
#include <wtf/RefPtr.h>
#include <wtf/text/CString.h>

using ATF::ByteArray;

namespace Aphid {
	namespace JNI {
		static JavaVM* s_global_vm = 0;

		JavaVM* getGlobalJavaVM()
		{
			return s_global_vm;
		}

		void setGlobalJavaVM(JavaVM* vm)
		{
			s_global_vm = vm;
		}

		JNIEnv* getJNIEnv()
		{
			if (!s_global_vm) {
				oa_error("global vm is not set!");
				return 0;
			}

			JNIEnv* env = 0;

			if (s_global_vm->GetEnv((void**) &env, JNI_VERSION_1_2) == JNI_OK) {
				return env;
			} else {
				oa_error("Can't get JNIEnv");
				return 0;
			}
		}

		bool checkException(JNIEnv* env)
		{
			if (env->ExceptionCheck() != 0) {
				oa_error("Found Java Exception!");
				env->ExceptionDescribe();
				return true;
			}

			return false;
		}

		bool checkException(JNIEnv* env, const char* file, const char* function, unsigned line)
		{
			if (env->ExceptionCheck() != 0) {
				oa_error("Found Java Exception: %s, %s, %d", file, function, line);
				env->ExceptionDescribe();
				return true;
			}

			return false;
		}

		PassOwnPtr<LocalString> toJNIString(JNIEnv* env, const String& string)
		{
			if (string.isNull())
				return 0;
			OwnPtr<LocalString> ret = LocalString::create(env, env->NewStringUTF(string.utf8().data()));
			return ret.release();
		}

		String toString(JNIEnv* env, jstring str)
		{
			if (!env || !str)
				return String();

			//Note: Aphid::String is also in UTF-16 representation, so GetStringChars is fine here
			const jchar* jchars = env->GetStringChars(str, NULL);
			if (!jchars)
				return String();

			String string(jchars, env->GetStringLength(str));
			env->ReleaseStringChars(str, jchars);
			CHECK_JNI_EXCEPTION(env);

			return string;
		}

		String toString(JNIEnv* env, const LocalObject* localObject)
		{
			if (!localObject)
				return String();
			return toString(env, (jstring) localObject->local());
		}

		PassRefPtr<ByteArray> toByteArray(JNIEnv* env, jbyteArray jbytes)
		{
			if (!env || !jbytes)
				return ByteArray::create(0);

			jsize len = env->GetArrayLength(jbytes);
			if (len <= 0)
				return ByteArray::create(0);

			RefPtr<ByteArray> array = ByteArray::create(len);

			env->GetByteArrayRegion(jbytes, 0, len, (jbyte*) array->data());
			CHECK_JNI_EXCEPTION(env);

			return array.release();
		}

		PassRefPtr<ByteArray> toByteArray(JNIEnv* env, const LocalObject* localObject)
		{
			return toByteArray(env, (jbyteArray) localObject->local());
		}

		PassOwnPtr<LocalClass> findClass(JNIEnv* env, const char* name)
		{
			if (!env || !name)
				return 0;

			jclass klass = env->FindClass(name);
			CHECK_JNI_EXCEPTION(env);

			return LocalClass::create(env, klass);
		}

		jmethodID getMethodID(JNIEnv* env, jclass klass, const char* name, const char* sig)
		{
			if (!env || !klass || !name || !sig)
				return 0;

			jmethodID method = env->GetMethodID(klass, name, sig);
			CHECK_JNI_EXCEPTION(env);

			return method;
		}

		jmethodID getStaticMethodID(JNIEnv* env, jclass klass, const char* name, const char* sig)
		{
			if (!env || !klass || !name || !sig)
				return 0;

			jmethodID method = env->GetStaticMethodID(klass, name, sig);
			CHECK_JNI_EXCEPTION(env);

			return method;
		}

		PassOwnPtr<LocalObject> callObjectMethod(JNIEnv* env, jobject obj, jmethodID method, ...)
		{
			va_list args;

			va_start(args, method);
			jobject jret = Caller<jobject>::callV(env, obj, method, args);
			va_end(args);

#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

			return LocalObject::create(env, jret);
		}

		PassOwnPtr<LocalObject> callStaticObjectMethod(JNIEnv* env, jclass klass, jmethodID method, ...)
		{
			va_list args;

			va_start(args, method);
			jobject jret = Caller<jobject>::callStaticV(env, klass, method, args);
			va_end(args);

#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

			return LocalObject::create(env, jret);
		}
		
		RefPtr<JNI::GlobalClass> Cache::s_appdelegate_jclass = 0;
		jmethodID Cache::s_appdelegate_jmethod_setFrameInterval = 0;

		RefPtr<JNI::GlobalClass> Cache::s_diagnostic_jclass = 0;
		jmethodID Cache::s_diagnostic_jmethod_error = 0;
		jmethodID Cache::s_diagnostic_jmethod_warn = 0;
		jmethodID Cache::s_diagnostic_jmethod_info = 0;

		RefPtr<JNI::GlobalObject> Cache::s_resourcemanager_jobject_assetsbundle = 0;
		jmethodID Cache::s_resourcemanager_jmethod_loadData = 0;
		jmethodID Cache::s_resourcemanager_jmethod_loadString = 0;
		jmethodID Cache::s_resourcemanager_jmethod_loadBitmap = 0;
		
		jmethodID Cache::s_bitmap_jmethod_recycle = 0;
		
		RefPtr<JNI::GlobalClass> Cache::s_ui_jclass = 0;
		jmethodID Cache::s_ui_jmethod_stringToBitmap = 0;
		
		jmethodID Cache::s_aphidtouch_jmethod_getIdentifier = 0;
		jmethodID Cache::s_aphidtouch_jmethod_getScreenX = 0;
		jmethodID Cache::s_aphidtouch_jmethod_getScreenY = 0;
		jmethodID Cache::s_aphidtouch_jmethod_getClientX = 0;
		jmethodID Cache::s_aphidtouch_jmethod_getClientY = 0;
		jmethodID Cache::s_aphidtouch_jmethod_getTimestamp = 0;

		void Cache::initialize(JNIEnv* env)
		{
			
			{// AppDelegate
				s_appdelegate_jclass = GlobalClass::create(env, findClass(env, "org/openaphid/internal/AppDelegate")->local());
				s_appdelegate_jmethod_setFrameInterval = getStaticMethodID(env, s_appdelegate_jclass->global(), "setFrameInterval", "(D)V");
				
				RELEASE_ASSERT(
						s_appdelegate_jclass->global() &&
						s_appdelegate_jmethod_setFrameInterval
						);
			}
			
			//cache diagnostic related ids
			{
				s_diagnostic_jclass = GlobalClass::create(env, findClass(env, "org/openaphid/internal/Diagnostic")->local());
				s_diagnostic_jmethod_error = getStaticMethodID(env, s_diagnostic_jclass->global(), "error",
						"(Ljava/lang/String;Z)V");
				s_diagnostic_jmethod_warn = getStaticMethodID(env, s_diagnostic_jclass->global(), "warn",
						"(Ljava/lang/String;Z)V");
				s_diagnostic_jmethod_info = getStaticMethodID(env, s_diagnostic_jclass->global(), "info",
						"(Ljava/lang/String;Z)V");

				RELEASE_ASSERT(
						s_diagnostic_jclass && s_diagnostic_jmethod_error && s_diagnostic_jmethod_warn && s_diagnostic_jmethod_info);
			}//diagnostic

			//cache ResourceManager related ids
			{
				{ // get ref to game bundle					
					jmethodID method = getStaticMethodID(env, s_appdelegate_jclass->global(), "getGameBundle",
							"()Lorg/openaphid/internal/AssetsBundle;");
					RELEASE_ASSERT(method);
					OwnPtr<LocalObject> obj = callStaticObjectMethod(env, s_appdelegate_jclass->global(), method);
					RELEASE_ASSERT(obj);
					s_resourcemanager_jobject_assetsbundle = GlobalObject::create(env, obj->local());
				}

				OwnPtr<LocalClass> klass = findClass(env, "org/openaphid/internal/AssetsBundle");
				s_resourcemanager_jmethod_loadData = getMethodID(env, klass->local(), "loadData", "(Ljava/lang/String;Z)[B");
				s_resourcemanager_jmethod_loadString = getMethodID(env, klass->local(), "loadString", "(Ljava/lang/String;)Ljava/lang/String;");
				s_resourcemanager_jmethod_loadBitmap = getMethodID(env, klass->local(), "loadBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
				RELEASE_ASSERT(klass && s_resourcemanager_jmethod_loadData && s_resourcemanager_jmethod_loadString && s_resourcemanager_jmethod_loadBitmap);
			}//ResourceManager
			
			{
				OwnPtr<LocalClass> klass = findClass(env, "android/graphics/Bitmap");
				s_bitmap_jmethod_recycle = getMethodID(env, klass->local(), "recycle", "()V");
				RELEASE_ASSERT(s_bitmap_jmethod_recycle);
			}//Bitmap
			
			{
				s_ui_jclass = GlobalClass::create(env, findClass(env, "org/openaphid/internal/utils/UI")->local());
				s_ui_jmethod_stringToBitmap = getStaticMethodID(env, s_ui_jclass->global(), "stringToBitmap", "(Ljava/lang/String;Ljava/lang/String;FIIII)Landroid/graphics/Bitmap;");
				RELEASE_ASSERT(s_ui_jclass && s_ui_jmethod_stringToBitmap);
			}//UI
			
			{
				OwnPtr<LocalClass> klass = findClass(env, "org/openaphid/gl/AphidTouch");
				
				s_aphidtouch_jmethod_getIdentifier = getMethodID(env, klass->local(), "getIdentifier", "()I");
				s_aphidtouch_jmethod_getScreenX = getMethodID(env, klass->local(), "getScreenX", "()F");
				s_aphidtouch_jmethod_getScreenY = getMethodID(env, klass->local(), "getScreenY", "()F");
				s_aphidtouch_jmethod_getClientX = getMethodID(env, klass->local(), "getClientX", "()F");
				s_aphidtouch_jmethod_getClientY = getMethodID(env, klass->local(), "getClientY", "()F");
				s_aphidtouch_jmethod_getTimestamp = getMethodID(env, klass->local(), "getTimestamp", "()J");
				RELEASE_ASSERT(s_aphidtouch_jmethod_getIdentifier && s_aphidtouch_jmethod_getScreenX && s_aphidtouch_jmethod_getScreenY && s_aphidtouch_jmethod_getClientX && s_aphidtouch_jmethod_getClientY && s_aphidtouch_jmethod_getTimestamp);
			}//AphidTouch
		}
	}
}
