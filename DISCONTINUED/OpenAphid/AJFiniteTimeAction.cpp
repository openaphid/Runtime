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

#include "AJFiniteTimeAction.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;

#include "AJFiniteTimeAction_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJFiniteTimeAction::AJFiniteTimeAction(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<FiniteTimeAction> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJFiniteTimeAction");
	}
	
	AJFiniteTimeAction::~AJFiniteTimeAction()
	{
		//oa_debug("AJFiniteTimeAction destructed");
		AJ_LEAK_DETECT_DEC("AJFiniteTimeAction");
	}
	///-------------------------------------------------------------------------------------------------------------------
	FiniteTimeAction* toFiniteTimeAction(AJValue value)
	{
		return value.inherits(&AJFiniteTimeAction::s_info) ? ajoa_cast<AJFiniteTimeAction*>(asObject(value))->impl() : 0;
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajFiniteTimeActionDuration(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJFiniteTimeAction* ajFiniteTimeAction = ajoa_cast<AJFiniteTimeAction*>(asObject(thisValue));
		return jsNumber(exec, ajFiniteTimeAction->impl()->duration());
	}

	void setAJFiniteTimeActionDuration(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJFiniteTimeAction* jsThis = ajoa_cast<AJFiniteTimeAction*>(thisObject);
			jsThis->impl()->setDuration(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "duration", "float");
	}

}
