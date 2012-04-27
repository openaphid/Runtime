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

#include "AJAction.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJNode.h"
//end

namespace Aphid {
	using namespace AJ;

#include "AJAction_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJAction::AJAction(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Action> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJAction");
	}
	
	AJAction::~AJAction()
	{
		//oa_debug("AJAction destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJAction");
	}
	
	void AJAction::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Action* toAction(AJValue value)
	{
		return value.inherits(&AJAction::s_info) ? ajoa_cast<AJAction*>(asObject(value))->impl() : 0;
	}

	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Action* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(impl->isInheritanceWrapper());
		return impl->createWrapper(exec, globalObject);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajActionBaseType(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAction* ajAction = ajoa_cast<AJAction*>(asObject(thisValue));
		return jsNumber(exec, ajAction->impl()->baseType());
	}

	AJ::AJValue ajActionTypeName(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAction* ajAction = ajoa_cast<AJAction*>(asObject(thisValue));
		return jsString(exec, ajAction->impl()->name());
	}

	AJ::AJValue ajActionIsTimeReversed(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAction* ajAction = ajoa_cast<AJAction*>(asObject(thisValue));
		return jsBoolean(ajAction->impl()->isTimeReversed());
	}

	void setAJActionIsTimeReversed(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJAction* jsThis = ajoa_cast<AJAction*>(thisObject);
			jsThis->impl()->setTimeReversed(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "isTimeReversed", "bool");
	}
	
	AJ::AJValue JSC_HOST_CALL ajActionFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		AJAction* thisObject = ajoa_cast<AJAction*>(asObject(thisValue));
		
		//clone()
		return toAJ(exec, thisObject->globalObject(), ATF::adoptRef(thisObject->impl()->newClone()).get());
	}

	AJ::AJValue JSC_HOST_CALL ajActionFunctionReverse(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		AJAction* thisObject = ajoa_cast<AJAction*>(asObject(thisValue));
		
		//reverse()
		return toAJ(exec, thisObject->globalObject(), ATF::adoptRef(thisObject->impl()->newReverse()).get());
	}

}
