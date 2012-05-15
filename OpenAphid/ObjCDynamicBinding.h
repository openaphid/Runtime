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

#ifndef OpenAphid_ObjCDynamicBinding_h
#define OpenAphid_ObjCDynamicBinding_h

#ifndef __OBJC__
#error ObjCDynamicBinding.h should not be included in a cpp file
#endif

#include "DynamicBinding.h"

#import <Foundation/Foundation.h>
#import "OABindingProtocol.h"

namespace Aphid {
	class ObjCDynamicBinding : public DynamicBinding {
		typedef DynamicBinding Base;
	public:
		virtual ~ObjCDynamicBinding();
		
		static PassRefPtr<ObjCDynamicBinding> create(const char* name, id<OABindingProtocol> receiver)
		{
			return adoptRef(new ObjCDynamicBinding(name, receiver));
		}
		
		static id convertToObjC(AJ::ExecState* exec, AJ::AJValue value);
		static AJ::AJValue convertToAJ(AJ::ExecState* exec, AJOAGlobalObject* globalObject, id o);
		
		static bool isValidReturnType(const char* type);
		static bool isValidParameterType(const char* type);
		static bool isSignedIntegerType(const char* type);
		static bool isUnsignedIntegerType(const char* type);
		static bool isFloatOrDoubleType(const char* type);
		static bool isNumberType(const char* type);
		
		virtual void listFunctionNames(Vector<AJ::UString>& names);
		virtual bool respondsToFunction(const AJ::UString& functionName);
		virtual AJ::AJValue invokeFunction(AJ::ExecState* exec, AJOAGlobalObject* globalObject, const AJ::UString& functionName, const AJ::ArgList& args);
		
	protected:
		ObjCDynamicBinding(const char* name, id<OABindingProtocol> receiver);
		
		AJ::AJValue doInvokeFunction(AJ::ExecState* exec, AJOAGlobalObject* globalObject, const AJ::UString& functionName, const AJ::ArgList& args);
		
		id<OABindingProtocol> m_receiver;
		CFMutableDictionaryRef m_bindings;
	};
}

#endif
