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

#include "AJTouchList.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJTouch.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJTouchList_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJTouchList::AJTouchList(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<TouchList> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJTouchList");
	}
	
	AJTouchList::~AJTouchList()
	{
		//oa_debug("AJTouchList destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJTouchList");
	}
	
	void AJTouchList::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	TouchList* toTouchList(AJValue value)
	{
		return value.inherits(&AJTouchList::s_info) ? ajoa_cast<AJTouchList*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, TouchList* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJTouchList>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajTouchListLength(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTouchList* ajTouchList = ajoa_cast<AJTouchList*>(asObject(thisValue));
		return jsNumber(exec, ajTouchList->impl()->size());
	}
	
	AJ::AJValue JSC_HOST_CALL ajTouchListFunctionItem(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJTouchList* thisObject = ajoa_cast<AJTouchList*>(asObject(thisValue));
		
		//item(int index)
		if (args.size() >= 1) {
			if (
					ensureIntArg(exec, args, 0, "index")
					) {
				int index = args.at(0).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->at(index));
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "item");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajTouchListFunctionIdentifiedTouch(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJTouchList* thisObject = ajoa_cast<AJTouchList*>(asObject(thisValue));
		
		//identifiedTouch(int identifier)
		if (args.size() >= 1) {
			if (
					ensureIntArg(exec, args, 0, "identifier")
					) {
				int identifier = args.at(0).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->identifiedTouch(identifier));
			}
		}		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "identifiedTouch");
		return jsUndefined();
	}
	
}
