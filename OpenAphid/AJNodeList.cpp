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

#include "AJNodeList.h"

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
	
#include "AJNodeList_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNodeList::AJNodeList(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NodeList> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNodeList");
	}
	
	AJNodeList::~AJNodeList()
	{
		//oa_debug("AJNodeList destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNodeList");
	}
	
	void AJNodeList::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	NodeList* toNodeList(AJValue value)
	{
		return value.inherits(&AJNodeList::s_info) ? ajoa_cast<AJNodeList*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NodeList* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNodeList>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajNodeListLength(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNodeList* thisObject = ajoa_cast<AJNodeList*>(asObject(thisValue));
		return jsNumber(exec, thisObject->impl()->size());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajNodeListPrototypeFunctionItem(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNodeList);
		//TODO: AJNodeList* thisObject = ajoa_cast<AJNodeList*>(asObject(thisValue));
		if (args.size() >= 1) {
			if (!args.at(0).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "index", "int");
			else {
				AJNodeList* thisObject = ajoa_cast<AJNodeList*>(asObject(thisValue));
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->at(args.at(0).toInt32(exec)));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		return jsUndefined();
	}
	
}
