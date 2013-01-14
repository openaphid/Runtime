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

#include "AJOABinding.h"

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

		PassOwnPtr<LocalString> toJNIString(JNIEnv* env, const char* string)
		{
			if (!string)
				return LocalString::create(env, 0).release();
			OwnPtr<LocalString> ret = LocalString::create(env, env->NewStringUTF(string));
			return ret.release();
		}

		PassOwnPtr<LocalString> toJNIString(JNIEnv* env, const String& string)
		{
			if (string.isNull())
				return LocalString::create(env, 0).release();
			OwnPtr<LocalString> ret = LocalString::create(env, env->NewStringUTF(string.utf8().data()));
			return ret.release();
		}

		PassOwnPtr<LocalString> toJNIString(JNIEnv* env, const AJ::UString& string)
		{
			return toJNIString(env, Aphid::toString(string));
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
#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

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
			
#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

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
			
			if (!klass)
				oa_warn("Can't find class for %s", name);
			
#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

			return LocalClass::create(env, klass);
		}

		jmethodID getMethodID(JNIEnv* env, jclass klass, const char* name, const char* sig)
		{
			if (!env || !klass || !name || !sig) {
				oa_warn("Invalid arguments for getMethodID");
				return 0;
			}

			jmethodID method = env->GetMethodID(klass, name, sig);
			
			if (!method)
				oa_warn("Can't find method ID for %s with signature %s", name, sig);
			
#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

			return method;
		}

		jmethodID getStaticMethodID(JNIEnv* env, jclass klass, const char* name, const char* sig)
		{
			if (!env || !klass || !name || !sig)
				return 0;

			jmethodID method = env->GetStaticMethodID(klass, name, sig);
			
			if (!method)
				oa_warn("Can't find static method ID for %s with signature %s", name, sig);
			
#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif

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
		
		RefPtr<GlobalClass> Cache::s_object_jclass = 0;

		RefPtr<JNI::GlobalClass> Cache::s_appdelegate_jclass = 0;
		jmethodID Cache::s_appdelegate_jmethod_setFrameInterval = 0;
		jmethodID Cache::s_appdelegate_jmethod_isMultitouchEnabled = 0;
		jmethodID Cache::s_appdelegate_jmethod_setMultitouchEnabled = 0;

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
		
		jmethodID Cache::s_aphidtouchevent_jmethod_getEventIdentifier = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getEventTime = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getEventPhase = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getIds = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getScreenXs = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getScreenYs = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getClientXs = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getClientYs = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getTouchCount = 0;
		jmethodID Cache::s_aphidtouchevent_jmethod_getActionIndex = 0;

		RefPtr<GlobalClass> Cache::s_aphidjsbinder_jclass = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_reapondsToFunction = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_listFunctionNames = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_getParameterTypeSignature = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_getReturnTypeSignature = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toBoolean = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toByte = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toChar_int = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toChar_String = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toShort = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toInteger = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toLong = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toFloat = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_toDouble = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_invokeMethod = 0;
		RefPtr<GlobalObject> Cache::s_aphidjsbinder_error_return = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_getBoolean = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_getInt = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_getFloat = 0;
		jmethodID Cache::s_aphidjsbinder_jmethod_getDouble = 0;
		jmethodID Cache::s_aphidjsbinder_s_jmethod_getObjectTypeSignature = 0;
		jmethodID Cache::s_aphidjsbinder_s_jmethod_getKeyIterator = 0;
		
		RefPtr<GlobalClass> Cache::s_arraylist_jclass = 0;
		jmethodID Cache::s_arraylist_jmethod_constructor_int = 0;
		jmethodID Cache::s_list_jmethod_add = 0;
		jmethodID Cache::s_list_jmethod_size = 0;
		jmethodID Cache::s_list_jmethod_get = 0;
			
		RefPtr<GlobalClass> Cache::s_hashmap_jclass = 0;
		jmethodID Cache::s_hashmap_jmethod_constructor_int = 0;
		jmethodID Cache::s_map_jmethod_put = 0;
		jmethodID Cache::s_map_jmethod_get = 0;
		
		jmethodID Cache::s_iterator_jmethod_hasNext = 0;
		jmethodID Cache::s_iterator_jmethod_next = 0;
		
		RefPtr<GlobalClass> Cache::s_string_jclass = 0;

		void Cache::initialize(JNIEnv* env)
		{
			
			{ //Object
				s_object_jclass = GlobalClass::create(env, findClass(env, "java/lang/Object")->local());

				RELEASE_ASSERT(
						s_object_jclass->global()
						);
			}

			{// AppDelegate
				s_appdelegate_jclass = GlobalClass::create(env, findClass(env, "org/openaphid/internal/AppDelegate")->local());
				s_appdelegate_jmethod_setFrameInterval =
						getStaticMethodID(env, s_appdelegate_jclass->global(), "setFrameInterval", "(D)V");
				s_appdelegate_jmethod_isMultitouchEnabled =
						getStaticMethodID(env, s_appdelegate_jclass->global(), "isMultitouchEnabled", "()Z");
				s_appdelegate_jmethod_setMultitouchEnabled =
						getStaticMethodID(env, s_appdelegate_jclass->global(), "setMultitouchEnabled", "(Z)V");
				
				RELEASE_ASSERT(
						s_appdelegate_jclass->global() &&
						s_appdelegate_jmethod_setFrameInterval &&
						s_appdelegate_jmethod_isMultitouchEnabled &&
						s_appdelegate_jmethod_setMultitouchEnabled
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
				s_resourcemanager_jmethod_loadString =
						getMethodID(env, klass->local(), "loadString", "(Ljava/lang/String;)Ljava/lang/String;");
				s_resourcemanager_jmethod_loadBitmap =
						getMethodID(env, klass->local(), "loadBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
				RELEASE_ASSERT(klass->local() && s_resourcemanager_jmethod_loadData && s_resourcemanager_jmethod_loadString && s_resourcemanager_jmethod_loadBitmap);
			}//ResourceManager
			
			{
				OwnPtr<LocalClass> klass = findClass(env, "android/graphics/Bitmap");
				s_bitmap_jmethod_recycle = getMethodID(env, klass->local(), "recycle", "()V");
				RELEASE_ASSERT(s_bitmap_jmethod_recycle);
			}//Bitmap
			
			{
				s_ui_jclass = GlobalClass::create(env, findClass(env, "org/openaphid/internal/utils/UI")->local());
				s_ui_jmethod_stringToBitmap =
						getStaticMethodID(env, s_ui_jclass->global(), "stringToBitmap", "(Ljava/lang/String;Ljava/lang/String;FIIII)Landroid/graphics/Bitmap;");
				RELEASE_ASSERT(s_ui_jclass->global() && s_ui_jmethod_stringToBitmap);
			}//UI
			
			{
				OwnPtr<LocalClass> klass = findClass(env, "org/openaphid/gl/AphidTouchEvent");
				
				s_aphidtouchevent_jmethod_getEventIdentifier = getMethodID(env, klass->local(), "getEventIdentifier", "()I");
				s_aphidtouchevent_jmethod_getEventTime = getMethodID(env, klass->local(), "getEventTime", "()J");
				s_aphidtouchevent_jmethod_getEventPhase = getMethodID(env, klass->local(), "getEventPhase", "()I");
				s_aphidtouchevent_jmethod_getIds = getMethodID(env, klass->local(), "getIds", "()[I");
				s_aphidtouchevent_jmethod_getScreenXs = getMethodID(env, klass->local(), "getScreenXs", "()[F");
				s_aphidtouchevent_jmethod_getScreenYs = getMethodID(env, klass->local(), "getScreenYs", "()[F");
				s_aphidtouchevent_jmethod_getClientXs = getMethodID(env, klass->local(), "getClientXs", "()[F");
				s_aphidtouchevent_jmethod_getClientYs = getMethodID(env, klass->local(), "getClientYs", "()[F");
				s_aphidtouchevent_jmethod_getTouchCount = getMethodID(env, klass->local(), "getTouchCount", "()I");
				s_aphidtouchevent_jmethod_getActionIndex = getMethodID(env, klass->local(), "getActionIndex", "()I");

				RELEASE_ASSERT(
						klass->local() &&
						s_aphidtouchevent_jmethod_getEventIdentifier &&
						s_aphidtouchevent_jmethod_getEventTime &&
						s_aphidtouchevent_jmethod_getEventPhase &&
						s_aphidtouchevent_jmethod_getIds &&
						s_aphidtouchevent_jmethod_getScreenXs &&
						s_aphidtouchevent_jmethod_getScreenYs &&
						s_aphidtouchevent_jmethod_getClientXs &&
						s_aphidtouchevent_jmethod_getClientYs &&
						s_aphidtouchevent_jmethod_getTouchCount &&
						s_aphidtouchevent_jmethod_getActionIndex
						);

			} //AphidTouchEvent

			{
				s_aphidjsbinder_jclass =
						GlobalClass::create(env, findClass(env, "org/openaphid/internal/AphidJSBinder")->local());

				s_aphidjsbinder_jmethod_reapondsToFunction =
						getMethodID(env, s_aphidjsbinder_jclass->global(), "respondsToFunction", "(Ljava/lang/String;)Z");
				s_aphidjsbinder_jmethod_listFunctionNames =
						getMethodID(env, s_aphidjsbinder_jclass->global(), "listFunctionNames", "()[Ljava/lang/String;");
				s_aphidjsbinder_jmethod_getParameterTypeSignature =
						getMethodID(env, s_aphidjsbinder_jclass->global(), "getParameterTypeSignature", "(Ljava/lang/String;)[C");
				
				s_aphidjsbinder_jmethod_getReturnTypeSignature = 
						getMethodID(env, s_aphidjsbinder_jclass->global(), "getReturnTypeSignature", "(Ljava/lang/String;)C");

				s_aphidjsbinder_jmethod_toBoolean =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toBoolean", "(Z)Ljava/lang/Boolean;");
				s_aphidjsbinder_jmethod_toByte =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toByte", "(I)Ljava/lang/Byte;");
				s_aphidjsbinder_jmethod_toChar_int =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toChar", "(I)Ljava/lang/Character;");
				s_aphidjsbinder_jmethod_toChar_String =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toChar", "(Ljava/lang/String;)Ljava/lang/Character;");
				s_aphidjsbinder_jmethod_toShort =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toShort", "(I)Ljava/lang/Short;");
				s_aphidjsbinder_jmethod_toInteger =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toInteger", "(I)Ljava/lang/Integer;");
				s_aphidjsbinder_jmethod_toLong =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toLong", "(I)Ljava/lang/Long;");
				s_aphidjsbinder_jmethod_toFloat =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toFloat", "(F)Ljava/lang/Float;");
				s_aphidjsbinder_jmethod_toDouble =
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "toDouble", "(D)Ljava/lang/Double;");
				s_aphidjsbinder_jmethod_invokeMethod = 
						getMethodID(env, s_aphidjsbinder_jclass->global(), "invokeMethod", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;");
				
				jmethodID method = getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getErrorReturnObject", "()Ljava/lang/Object;");
				s_aphidjsbinder_error_return = GlobalObject::create(env, callStaticObjectMethod(env, s_aphidjsbinder_jclass->global(), method)->local());
				
				s_aphidjsbinder_jmethod_getBoolean = getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getBoolean", "(Ljava/lang/Boolean;)Z");
				s_aphidjsbinder_jmethod_getInt = getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getInt", "(Ljava/lang/Number;)I");
				s_aphidjsbinder_jmethod_getFloat = getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getFloat", "(Ljava/lang/Number;)F");
				s_aphidjsbinder_jmethod_getDouble = getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getDouble", "(Ljava/lang/Number;)D");
				
				s_aphidjsbinder_s_jmethod_getObjectTypeSignature = 
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getObjectTypeSignature", "(Ljava/lang/Object;)C");
				s_aphidjsbinder_s_jmethod_getKeyIterator = 
						getStaticMethodID(env, s_aphidjsbinder_jclass->global(), "getKeyIterator", "(Ljava/util/Map;)Ljava/util/Iterator;");

				RELEASE_ASSERT(
						s_aphidjsbinder_jclass->global() &&
						s_aphidjsbinder_jmethod_listFunctionNames &&
						s_aphidjsbinder_jmethod_reapondsToFunction &&
						s_aphidjsbinder_jmethod_getParameterTypeSignature &&
						s_aphidjsbinder_jmethod_getReturnTypeSignature &&
						s_aphidjsbinder_jmethod_toBoolean &&
						s_aphidjsbinder_jmethod_toByte &&
						s_aphidjsbinder_jmethod_toChar_int &&
						s_aphidjsbinder_jmethod_toChar_String &&
						s_aphidjsbinder_jmethod_toShort &&
						s_aphidjsbinder_jmethod_toInteger &&
						s_aphidjsbinder_jmethod_toLong &&
						s_aphidjsbinder_jmethod_toFloat &&
						s_aphidjsbinder_jmethod_toDouble &&
						s_aphidjsbinder_error_return->global() &&
						s_aphidjsbinder_jmethod_invokeMethod &&
						s_aphidjsbinder_s_jmethod_getKeyIterator
						);
			} //AphidJSBinder
			
			{
				s_arraylist_jclass = GlobalClass::create(env, findClass(env, "java/util/ArrayList")->local());
				
				ASSERT(s_arraylist_jclass->global());
				
				s_arraylist_jmethod_constructor_int = getMethodID(env, s_arraylist_jclass->global(), "<init>", "(I)V");
				s_list_jmethod_add = getMethodID(env, s_arraylist_jclass->global(), "add", "(Ljava/lang/Object;)Z");
				s_list_jmethod_size = getMethodID(env, s_arraylist_jclass->global(), "size", "()I");
				s_list_jmethod_get = getMethodID(env, s_arraylist_jclass->global(), "get", "(I)Ljava/lang/Object;");
				
				RELEASE_ASSERT(
						s_arraylist_jclass->global() &&
						s_arraylist_jmethod_constructor_int &&
						s_list_jmethod_add &&
						s_list_jmethod_size &&
						s_list_jmethod_get
						);
			} //ArrayList
			
			{
				s_hashmap_jclass = GlobalClass::create(env, findClass(env, "java/util/HashMap")->local());
				s_hashmap_jmethod_constructor_int = getMethodID(env, s_hashmap_jclass->global(), "<init>", "(I)V");
				s_map_jmethod_put = getMethodID(env, s_hashmap_jclass->global(), "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
				s_map_jmethod_get = getMethodID(env, s_hashmap_jclass->global(), "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
				
				RELEASE_ASSERT(
						s_hashmap_jclass->global() &&
						s_hashmap_jmethod_constructor_int &&
						s_map_jmethod_put &&
						s_map_jmethod_get
						);
			}//HashMap
			
			{
				OwnPtr<LocalClass> klass = findClass(env, "java/util/Iterator");
				s_iterator_jmethod_hasNext = getMethodID(env, klass->local(), "hasNext", "()Z");
				s_iterator_jmethod_next = getMethodID(env, klass->local(), "next", "()Ljava/lang/Object;");
				
				RELEASE_ASSERT(
						klass->local() &&
						s_iterator_jmethod_hasNext &&
						s_iterator_jmethod_next
						);
			} //Iterator
			
			{
				s_string_jclass = GlobalClass::create(env, findClass(env, "java/lang/String")->local());
				
				RELEASE_ASSERT(
						s_string_jclass->global()
						);
			} //String
		}
	}
}
