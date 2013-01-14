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

#ifndef OpenAphid_DynamicBinding_Android_h
#define OpenAphid_DynamicBinding_Android_h

#include "DynamicBinding.h"
#include "OAJNIUtil.h"

namespace Aphid {	
	class JavaDynamicBinding : public DynamicBinding {
		typedef DynamicBinding Base;
	public:
		virtual ~JavaDynamicBinding();
		
		static PassRefPtr<JavaDynamicBinding> create(const char* name, PassRefPtr<JNI::GlobalObject> jbinder)
		{
			return adoptRef(new JavaDynamicBinding(name, jbinder));
		}
		
		virtual void listFunctionNames(Vector<AJ::UString>& names);
		virtual bool respondsToFunction(const AJ::UString& functionName);
		virtual AJ::AJValue invokeFunction(AJ::ExecState* exec, AJOAGlobalObject* globalObject, const AJ::UString& functionName, const AJ::ArgList& args);
	protected:
		JavaDynamicBinding(const char* name, PassRefPtr<JNI::GlobalObject> jbinder);
		
		static jobject convertToJava(JNIEnv* env, AJ::ExecState* exec, AJ::AJValue value, bool treatArrayAsObject=false);
		static AJ::AJValue convertToAJ(JNIEnv* env, AJ::ExecState* exec, AJOAGlobalObject* globalObject, jchar type, jobject jobj);
		
		RefPtr<JNI::GlobalObject> m_jbinder;
	};
}

#endif
