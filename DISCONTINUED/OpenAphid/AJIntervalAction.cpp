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

#include "AJIntervalAction.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;

#include "AJIntervalAction_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJIntervalAction::AJIntervalAction(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<IntervalAction> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJIntervalAction");
	}
	
	AJIntervalAction::~AJIntervalAction()
	{
		//oa_debug("AJIntervalAction destructed");
		AJ_LEAK_DETECT_DEC("AJIntervalAction");
	}
	///-------------------------------------------------------------------------------------------------------------------
	IntervalAction* toIntervalAction(AJValue value)
	{
		return value.inherits(&AJIntervalAction::s_info) ? ajoa_cast<AJIntervalAction*>(asObject(value))->impl() : 0;
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajIntervalActionElapsed(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJIntervalAction* ajIntervalAction = ajoa_cast<AJIntervalAction*>(asObject(thisValue));
		return jsNumber(exec, ajIntervalAction->impl()->elapsed());
	}

}
