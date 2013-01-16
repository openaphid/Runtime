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

#include "AJTouchEvent.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJTouchList.h"
//end

namespace Aphid {
	using namespace AJ;

#include "AJTouchEvent_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJTouchEvent::AJTouchEvent(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<TouchEvent> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJTouchEvent");
	}
	
	AJTouchEvent::~AJTouchEvent()
	{
		//oa_debug("AJTouchEvent destructed");
		AJ_LEAK_DETECT_DEC("AJTouchEvent");
	}
	///-------------------------------------------------------------------------------------------------------------------
	TouchEvent* toTouchEvent(AJValue value)
	{
		return value.inherits(&AJTouchEvent::s_info) ? ajoa_cast<AJTouchEvent*>(asObject(value))->impl() : 0;
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajTouchEventTouches(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouchEvent* ajTouchEvent = ajoa_cast<AJTouchEvent*>(asObject(thisValue));
		return toAJ(exec, ajTouchEvent->globalObject(), ajTouchEvent->impl()->touches());
	}

	AJ::AJValue ajTouchEventTargetTouches(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouchEvent* ajTouchEvent = ajoa_cast<AJTouchEvent*>(asObject(thisValue));
		return toAJ(exec, ajTouchEvent->globalObject(), ajTouchEvent->impl()->targetTouches());
	}

	AJ::AJValue ajTouchEventChangedTouches(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouchEvent* ajTouchEvent = ajoa_cast<AJTouchEvent*>(asObject(thisValue));
		return toAJ(exec, ajTouchEvent->globalObject(), ajTouchEvent->impl()->changedTouches());
	}

	AJ::AJValue ajTouchEventAltKey(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsBoolean(false);
	}

	AJ::AJValue ajTouchEventMetaKey(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsBoolean(false);
	}

	AJ::AJValue ajTouchEventCtrlKey(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsBoolean(false);
	}

	AJ::AJValue ajTouchEventShiftKey(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsBoolean(false);
	}
}
