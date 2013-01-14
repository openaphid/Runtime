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

#include "AJNamespaceJS.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "Scheduler.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJNamespaceJS_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceJS::AJNamespaceJS(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceJS> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceJS");
	}
	
	AJNamespaceJS::~AJNamespaceJS()
	{
		//oa_debug("AJNamespaceJS destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceJS");
	}
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceJS* toNamespaceJS(AJValue value)
	{
		return value.inherits(&AJNamespaceJS::s_info) ? ajoa_cast<AJNamespaceJS*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceJS* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceJS>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajNamespaceJSOnload(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNamespaceJS* jsNamespaceJS = ajoa_cast<AJNamespaceJS*>(asObject(thisValue));
		return jsNamespaceJS->impl()->onloadCallback();
	}
	
	void setAJNamespaceJSOnload(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJNamespaceJS* jsThis = ajoa_cast<AJNamespaceJS*>(thisObject);
			jsThis->impl()->setOnloadCallback(exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onload", "function");
	}
	
	AJ::AJValue ajNamespaceJSGcFrequency(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, Scheduler::s_gc_frame_interval);
	}
	
	void setAJNamespaceJSGcFrequency(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value)
	{
		UNUSED_PARAM(thisObject);
		
		if (value.isNumber()) {
			Scheduler::s_gc_frame_interval = value.toInt32(exec);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "gcFrequency", "int");
	}	
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajNamespaceJSPrototypeFunctionGc(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJNamespaceJS);
		AJNamespaceJS* thisObject = ajoa_cast<AJNamespaceJS*>(asObject(thisValue));
		return jsBoolean(thisObject->impl()->gc());
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceJSPrototypeFunctionIncludeFile(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNamespaceJS);
		AJNamespaceJS* thisObject = ajoa_cast<AJNamespaceJS*>(asObject(thisValue));
		//includeFile(string filename)
		if (args.size() >= 1) {
			if (
					ensureStringArg(exec, args, 0, "filename")
					) {
				const UString& filename = valueToUString(exec, args.at(0));
				Completion completion = NamespaceJS::includeFile(exec, thisObject->globalObject(), filename);
				if (completion.complType() == Throw)
					exec->setException(completion.value());
				return completion.value();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "includeFile");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceJSPrototypeFunctionIncludeScript(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNamespaceJS);
		AJNamespaceJS* thisObject = ajoa_cast<AJNamespaceJS*>(asObject(thisValue));
		//includeScript(string script)
		if (args.size() >= 1) {
			if (
					ensureStringArg(exec, args, 0, "script")
					) {
				const UString& script = valueToUString(exec, args.at(0));
				Completion completion = NamespaceJS::includeScript(exec, thisObject->globalObject(), UString(), script);
				if (completion.complType() == Throw)
					exec->setException(completion.value());
				return completion.value();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "includeScript");
		return jsUndefined();
	}
}
