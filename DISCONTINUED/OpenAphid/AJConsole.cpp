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

#include "AJConsole.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

#include "ScriptCallStack.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJConsole_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJConsole::AJConsole(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Console> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJConsole");
	}
	
	AJConsole::~AJConsole()
	{
		//oa_debug("AJConsole destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJConsole");
	}
	
	
	///-------------------------------------------------------------------------------------------------------------------
	Console* toConsole(AJValue value)
	{
		return value.inherits(&AJConsole::s_info) ? ajoa_cast<AJConsole*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Console* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJConsole>(exec, globalObject, impl);		
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionDebug(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		ScriptCallStack stack(exec, args, 0);
		thisObject->impl()->debug(stack);
		return jsUndefined();
	}
	
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionError(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		ScriptCallStack stack(exec, args, 0);
		thisObject->impl()->error(stack);
		return jsUndefined();
	}
	
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionInfo(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		ScriptCallStack stack(exec, args, 0);
		thisObject->impl()->info(stack);
		return jsUndefined();
	}
	
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionLog(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		ScriptCallStack stack(exec, args, 0);
		thisObject->impl()->log(stack);
		return jsUndefined();
	}
	
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionWarn(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		ScriptCallStack stack(exec, args, 0);
		thisObject->impl()->warn(stack);
		return jsUndefined();
	}
	
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionTrace(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		ScriptCallStack stack(exec, args, 0);
		thisObject->impl()->trace(stack);
		return jsUndefined();
	}
	
	AJValue JSC_HOST_CALL ajConsolePrototypeFunctionAssert(ExecState* exec, AJObject*, AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJConsole);
		AJConsole* thisObject = ajoa_cast<AJConsole*>(asObject(thisValue));
		if (args.size() >= 1) {
			if (!args.at(0).isBoolean())
				OA_ERROR_INVALID_ARGUMENT(exec, "condition", "bool");
			else {
				ScriptCallStack stack(exec, args, 1);
				thisObject->impl()->assertCondition(args.at(0).toBoolean(exec), stack);
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		
		return jsUndefined();
	}	
}
