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

#ifndef OA_OAJNIUtil_H
#define OA_OAJNIUtil_H

#include <jni.h>

#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/OwnPtr.h>
#include <wtf/ByteArray.h>
#include <wtf/text/WTFString.h>
#include "OAUtil.h"

namespace Aphid {

	namespace JNI {
		JavaVM* getGlobalJavaVM();
		void setGlobalJavaVM(JavaVM* vm);
		JNIEnv* getJNIEnv();
		bool checkException(JNIEnv* env);
		bool checkException(JNIEnv* env, const char* file, const char* function, unsigned line);

#define CHECK_JNI_EXCEPTION(env) JNI::checkException(env, __FILE__, __FUNCTION__, __LINE__);

		///-----------------------------------------------------------------------------------------------------------------
		template<typename T> struct Caller;

#define DECLARE_CALLER(TYPE, METHOD) \
		template<> struct Caller<TYPE> { \
			static TYPE callV(JNIEnv* env, jobject obj, jmethodID mid, va_list args) \
			{ \
				return env->Call##METHOD##MethodV(obj, mid, args); \
			} \
			static TYPE callStaticV(JNIEnv* env, jclass klass, jmethodID mid, va_list args) \
			{ \
				return env->CallStatic##METHOD##MethodV(klass, mid, args); \
			} \
		};

		DECLARE_CALLER(void, Void);
		DECLARE_CALLER(jobject, Object);
		DECLARE_CALLER(jboolean, Boolean);
		DECLARE_CALLER(jbyte, Byte);
		DECLARE_CALLER(jchar, Char);
		DECLARE_CALLER(jshort, Short);
		DECLARE_CALLER(jint, Int);
		DECLARE_CALLER(jlong, Long);
		DECLARE_CALLER(jfloat, Float);
		DECLARE_CALLER(jdouble, Double);
#undef DECLARE_CALLER

		template<typename T> T callMethod(JNIEnv* env, jobject obj, jmethodID method, ...)
		{
			va_list args;

			va_start(args, method);
			T ret = Caller<T>::callV(env, obj, method, args);
			va_end(args);

#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif
			return ret;
		}
		
		static void callMethod(JNIEnv* env, jobject obj, jmethodID method, ...)
		{
			va_list args;

			va_start(args, method);
			Caller<void>::callV(env, obj, method, args);
			va_end(args);

#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif
		}

		template<typename T> T callStaticMethod(JNIEnv* env, jclass klass, jmethodID method, ...)
		{
			va_list args;

			va_start(args, method);
			T ret = Caller<T>::callStaticV(env, klass, method, args);
			va_end(args);

#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif
			return ret;
		}

		static void callStaticMethod(JNIEnv* env, jclass klass, jmethodID method, ...)
		{
			va_list args;

			va_start(args, method);
			Caller<void>::callStaticV(env, klass, method, args);
			va_end(args);

#if OA_DEV
			CHECK_JNI_EXCEPTION(env);
#endif
		}

		///-----------------------------------------------------------------------------------------------------------------
		template<typename T>
		class LocalRef: public Noncopyable {
		public:
			virtual ~LocalRef()
			{
				if (m_env && m_jobject)
					m_env->DeleteLocalRef(m_jobject);
			}

			//NOTES: caller should gurantee to pass a local reference of jobject; no extra checking is applied as there is no stable API to distinguish global and local reference
			static PassOwnPtr<LocalRef<T> > create(JNIEnv* env, T localObject)
			{
				//if (!localObject)
				//	return 0; //TODO: is it really a good idea to return 0 here?
				OwnPtr<LocalRef<T> > ptr(new LocalRef<T>(env, localObject));
				return ptr.release();
			}

			T local() const
			{
				return m_jobject;
			}

			//TODO: is this operator really necessary?
			bool operator!() const
			{
				return !m_jobject;
			}

			/*
			 typedef T (LocalObject::*UnspecifiedBoolType);
			 operator UnspecifiedBoolType() const
			 {
			 return m_jobject ? &LocalObject<T>::m_jobject : 0;
			 }*/

		private:
			LocalRef(JNIEnv* env, T localObject)
					: m_env(env), m_jobject(localObject)
			{
				ASSERT(env);
			}

			JNIEnv* m_env;
			T m_jobject;
		};

		typedef LocalRef<jobject> LocalObject;
		typedef LocalRef<jstring> LocalString;
		typedef LocalRef<jclass> LocalClass;

		///-----------------------------------------------------------------------------------------------------------------
		template<typename T>
		class GlobalRef: public RefCounted<GlobalRef<T> > {
		public:
			virtual ~GlobalRef()
			{
				if (m_jobject) {
					JNIEnv* env = getJNIEnv();
					env->DeleteGlobalRef(m_jobject);
#if OA_DEV
					CHECK_JNI_EXCEPTION(env);
#endif
				}
			}

			static PassRefPtr<GlobalRef<T> > create(JNIEnv* env, T obj)
			{
				if (!obj)
					return 0;
				return adoptRef(new GlobalRef<T>(env, obj));
			}

			T global() const
			{
				return m_jobject;
			}
		private:
			GlobalRef(JNIEnv* env, jobject obj)
			{
				if (env && obj) {
					m_jobject = (T) env->NewGlobalRef(obj);
					if (!m_jobject)
						oa_error("Can't create global ref to jobject");
#if OA_DEV
					CHECK_JNI_EXCEPTION(env);
#endif
				} else
					m_jobject = 0;
			}

			T m_jobject;
		};

		typedef GlobalRef<jobject> GlobalObject;
		typedef GlobalRef<jstring> GlobalString;
		typedef GlobalRef<jclass> GlobalClass;

		PassOwnPtr<LocalString> toJNIString(JNIEnv* env, const String& string);
		String toString(JNIEnv* env, const LocalObject* localObject);
		String toString(JNIEnv* env, jstring str);

		PassRefPtr<ATF::ByteArray> toByteArray(JNIEnv* env, const LocalObject* localObject);
		ATF::PassRefPtr<ATF::ByteArray> toByteArray(JNIEnv* env, jbyteArray jbytes);

		PassOwnPtr<LocalClass> findClass(JNIEnv* env, const char* name);
		jmethodID getMethodID(JNIEnv* env, jclass klass, const char* name, const char* sig);
		jmethodID getStaticMethodID(JNIEnv* env, jclass klass, const char* name, const char* sig);
		PassOwnPtr<LocalObject> callObjectMethod(JNIEnv* env, jobject obj, jmethodID method, ...);
		PassOwnPtr<LocalObject> callStaticObjectMethod(JNIEnv* env, jclass klass, jmethodID method, ...);

		class Cache: Noncopyable {
		public:
			virtual ~Cache()
			{
			}
			
			static RefPtr<JNI::GlobalClass> s_appdelegate_jclass;
			static jmethodID s_appdelegate_jmethod_setFrameInterval;

			//Diagnostic related
			static RefPtr<JNI::GlobalClass> s_diagnostic_jclass;
			static jmethodID s_diagnostic_jmethod_error;
			static jmethodID s_diagnostic_jmethod_warn;
			static jmethodID s_diagnostic_jmethod_info;

			static RefPtr<JNI::GlobalObject> s_resourcemanager_jobject_assetsbundle;
			static jmethodID s_resourcemanager_jmethod_loadData;
			static jmethodID s_resourcemanager_jmethod_loadString;
			static jmethodID s_resourcemanager_jmethod_loadBitmap;
			
			static jmethodID s_bitmap_jmethod_recycle;
			
			static RefPtr<JNI::GlobalClass> s_ui_jclass;
			static jmethodID s_ui_jmethod_stringToBitmap;
			
			static jmethodID s_aphidtouch_jmethod_getIdentifier;
			static jmethodID s_aphidtouch_jmethod_getScreenX;
			static jmethodID s_aphidtouch_jmethod_getScreenY;
			static jmethodID s_aphidtouch_jmethod_getClientX;
			static jmethodID s_aphidtouch_jmethod_getClientY;
			static jmethodID s_aphidtouch_jmethod_getTimestamp;
			
			static void initialize(JNIEnv* env);

		private:
			Cache()
			{
			}
		};
	} //namespace JNI
}

#endif
