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

#ifndef OpenAphid_DynamicBinding_h
#define OpenAphid_DynamicBinding_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/FastAllocBase.h>
#include <AtomicString.h>
#include <runtime/AJValue.h>

#include "ExceptionCode.h"
#include "DirectWrapper.h"

namespace Aphid {
	class AJDynamicBinding;
	class AJOAGlobalObject;
	
	class DynamicBinding : public RefCounted<DynamicBinding>, public DirectWrapper<AJDynamicBinding> {
	public:
		virtual ~DynamicBinding() {}
		
		const AtomicString& name() const {return m_name;}
		
		virtual void listFunctionNames(Vector<AJ::UString>& names) = 0;
		
		virtual bool respondsToFunction(const AJ::UString& functionName) = 0;
		virtual AJ::AJValue invokeFunction(AJ::ExecState* exec, AJOAGlobalObject* globalObject, const AJ::UString& functionName, const AJ::ArgList& args) = 0;
		
	protected:
		DynamicBinding(const char* name) : m_name(name) {}
		
		AtomicString m_name;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	
}


#endif
