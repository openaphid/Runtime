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
#include "Diagnostic.h"
#include "OAJNIUtil.h"
#include "OAUtil.h"

#include <wtf/text/CString.h>

namespace Aphid {
	using JNI::Cache;

	void Diagnostic::error(const String& message, bool logInConsole)
	{
		JNIEnv* env = JNI::getJNIEnv();

		JNI::callStaticVoidMethod(env, Cache::s_diagnostic_jclass->global(), Cache::s_diagnostic_jmethod_error,
				JNI::toJNIString(env, message)->local(), logInConsole ? JNI_TRUE : JNI_FALSE);
	}

	void Diagnostic::warn(const String& message, bool logInConsole)
	{
		JNIEnv* env = JNI::getJNIEnv();

		JNI::callStaticVoidMethod(env, Cache::s_diagnostic_jclass->global(), Cache::s_diagnostic_jmethod_warn,
				JNI::toJNIString(env, message)->local(), logInConsole ? JNI_TRUE : JNI_FALSE);
	}

	void Diagnostic::info(const String& message, bool logInConsole)
	{
		JNIEnv* env = JNI::getJNIEnv();

		JNI::callStaticVoidMethod(env, Cache::s_diagnostic_jclass->global(), Cache::s_diagnostic_jmethod_info,
				JNI::toJNIString(env, message)->local(), logInConsole ? JNI_TRUE : JNI_FALSE);
	}
}
