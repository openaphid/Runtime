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

#include "AJTouch.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "AJEventListener.h"

namespace Aphid {
	using namespace AJ;

#include "AJTouch_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJTouch::AJTouch(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Touch> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJTouch");
	}
	
	AJTouch::~AJTouch()
	{
		//oa_debug("AJTouch destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJTouch");
	}
	
	void AJTouch::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Touch* toTouch(AJValue value)
	{
		return value.inherits(&AJTouch::s_info) ? ajoa_cast<AJTouch*>(asObject(value))->impl() : 0;
	}

	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Touch* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJTouch>(exec, globalObject, impl);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajTouchIdentifier(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->identifier());
	}

	AJ::AJValue ajTouchTarget(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return toAJ(exec, ajTouch->globalObject(), ajTouch->impl()->target());
	}

	AJ::AJValue ajTouchScreenX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->screenX());
	}

	AJ::AJValue ajTouchScreenY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->screenY());
	}

	AJ::AJValue ajTouchClientX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->clientX());
	}

	AJ::AJValue ajTouchClientY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->clientY());
	}

	AJ::AJValue ajTouchPageX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->pageX());
	}

	AJ::AJValue ajTouchPageY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouch* ajTouch = ajoa_cast<AJTouch*>(asObject(thisValue));
		return jsNumber(exec, ajTouch->impl()->pageY());
	}

}
