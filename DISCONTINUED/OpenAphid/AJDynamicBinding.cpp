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

#include "AJDynamicBinding.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;

#include "AJDynamicBinding_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJDynamicBinding::AJDynamicBinding(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<DynamicBinding> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJDynamicBinding");
		ExecState * exec = globalObject->globalExec();
		
		Vector<UString> names;
		m_impl->listFunctionNames(names);
		
		for (int i = 0; i < names.size(); i++) {
			const UString& name = names.at(i);
			putDirectFunction(exec, new (exec) NativeFunctionWrapper(exec, globalObject->prototypeFunctionStructure(), 0, Identifier(exec, name), ajDynamicBindingFunction));
		}
		ASSERT(!exec->hadException());
	}
	
	AJDynamicBinding::~AJDynamicBinding()
	{
		//oa_debug("AJDynamicBinding destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJDynamicBinding");
	}
	///-------------------------------------------------------------------------------------------------------------------
	DynamicBinding* toDynamicBinding(AJValue value)
	{
		return value.inherits(&AJDynamicBinding::s_info) ? ajoa_cast<AJDynamicBinding*>(asObject(value))->impl() : 0;
	}

	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, DynamicBinding* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJDynamicBinding>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajDynamicBindingFunction(AJ::ExecState* exec, AJ::AJObject* functionObject, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		CHECK_INHERITS(exec, thisValue, AJDynamicBinding);
		
		AJDynamicBinding* thisObject = ajoa_cast<AJDynamicBinding*>(asObject(thisValue));
		
		if (functionObject->inherits(&InternalFunction::info)) {
			InternalFunction* function = asInternalFunction(functionObject);
			const UString& name = function->name(exec);
			return thisObject->impl()->invokeFunction(exec, thisObject->globalObject(), name, args);
		}
		return jsUndefined();
	}

}
