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

#include "AJEvent.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "AJEventListener.h"

namespace Aphid {
	using namespace AJ;

#include "AJEvent_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJEvent::AJEvent(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Event> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJEvent");
	}
	
	AJEvent::~AJEvent()
	{
		//oa_debug("AJEvent destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJEvent");
	}
	
	void AJEvent::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Event* toEvent(AJValue value)
	{
		return value.inherits(&AJEvent::s_info) ? ajoa_cast<AJEvent*>(asObject(value))->impl() : 0;
	}

	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Event* impl)
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
	AJ::AJValue ajEventType(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return jsString(exec, toUString(ajEvent->impl()->type()));
	}

	AJ::AJValue ajEventTarget(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return toAJ(exec, ajEvent->globalObject(), ajEvent->impl()->target());
	}

	AJ::AJValue ajEventCurrentTarget(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return toAJ(exec, ajEvent->globalObject(), ajEvent->impl()->currentTarget());
	}

	AJ::AJValue ajEventEventPhase(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return jsNumber(exec, ajEvent->impl()->eventPhase());
	}

	AJ::AJValue ajEventBubbles(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return jsBoolean(ajEvent->impl()->bubbles());
	}

	AJ::AJValue ajEventCancelable(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return jsBoolean(ajEvent->impl()->cancelable());
	}

	AJ::AJValue ajEventTimestamp(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJEvent* ajEvent = ajoa_cast<AJEvent*>(asObject(thisValue));
		return jsNumber(exec, ajEvent->impl()->timestamp());
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajEventPrototypeFunctionStopPropagation(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJEvent);
		
		AJEvent* thisObject = ajoa_cast<AJEvent*>(asObject(thisValue));
		thisObject->impl()->stopPropagation();
		return jsUndefined();
	}

	AJ::AJValue JSC_HOST_CALL ajEventPrototypeFunctionPreventDefault(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJEvent);
		AJEvent* thisObject = ajoa_cast<AJEvent*>(asObject(thisValue));
		thisObject->impl()->preventDefault();
		return jsUndefined();
	}

}
