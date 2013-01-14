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

#include "AJOAGlobalObject.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include <runtime/AJLock.h>

#include "OAUtil.h"

#include "OAGlobalObject.h"
#include "Console.h"

#include "AJConsole.h"
#include "AJNamespaceAphid.h"

#include "Node.h"
#include "Sprite.h"
#include "Scheduler.h"
#include "AJCallback.h"

#include "AJOABindingTypeHeaders.h"

namespace Aphid {
	using namespace AJ;	
	
#include "AJOAGlobalObject_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------
	AJOAGlobalObject::AJOAGlobalObject(ATF::NonNullPassRefPtr<Structure> structure, PassRefPtr<OAGlobalObject> impl)
	: AJGlobalObject(structure, new AJOAGlobalObject::AJOAGlobalObjectData(), this)
	, m_impl(impl)
	{
		m_impl->setWrapper(this); //an exceptional case to setWrapper in constructor
		//ExecState * exec = globalExec();
		//putDirectFunction(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure(), 1, Identifier(exec, "print"), functionPrint));
		//putDirectFunction(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure(), 0, Identifier(exec, "noob"), functionNoob));
		LEAK_DETECT_INC("AJOAGlobalObject");
		AJOAStructureVector& s = this->structures();
		s.fill(0, AJOABindingTypeMax);
		
		AJOAConstructorVector& c = this->constructors();
		c.fill(0, AJOABindingTypeMax);
	}
	
	AJOAGlobalObject::~AJOAGlobalObject()
	{
		oa_debug("AJOAGlobalObject destructed");
		m_impl->clearWrapper(this);
		LEAK_DETECT_DEC("AJOAGlobalObject");
	}

	void AJOAGlobalObject::markChildren(AJ::MarkStack& markStack)
	{			
		Base::markChildren(markStack);
		
		{
			AJOAStructureVector::iterator end = structures().end();
			for (AJOAStructureVector::iterator it = structures().begin(); it != end; ++it)
				if (it->get())
					markStack.append(it->get()->storedPrototype());
		}
		
		{
			size_t size = constructors().size();
			for (size_t i = 0; i < size; i++) {
				AJObject* constructor = constructors().at(i);
				if (constructor)
					markStack.append(constructor);
			}
		}
		
		markDirectWrapper(m_impl.get(), markStack, Heap::s_mark_id);
		
		//AJGlobalData& globalData = *Heap::heap(this)->globalData();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	OAGlobalObject* toOAGlobalObject(AJValue value)
	{
		return value.inherits(&AJOAGlobalObject::s_info) ? ajoa_cast<AJOAGlobalObject*>(asObject(value))->impl() : 0;
	}
	
	AJValue toAJ(ExecState*, AJOAGlobalObject* globalObject, OAGlobalObject* impl)
	{
		UNUSED_PARAM(globalObject);
		
		if (!impl)
			return jsNull();
		
		AJOAGlobalObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		
		ASSERT_NOT_REACHED();
		
		return jsUndefined();
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJValue ajOAGlobalObjectConsole(ExecState* exec, AJValue thisValue, const Identifier&) 
	{
		AJOAGlobalObject* thisObject = ajoa_cast<AJOAGlobalObject*>(asObject(thisValue));
		OAGlobalObject* impl = thisObject->impl();
		return toAJ(exec, thisObject, impl->console());
	}
	
	AJ::AJValue ajOAGlobalObjectAphid(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJOAGlobalObject* thisObject = ajoa_cast<AJOAGlobalObject*>(asObject(thisValue));
		OAGlobalObject* impl = thisObject->impl();
		return toAJ(exec, thisObject, impl->namespaceAphid());
	}
		
	AJ::AJValue ajOAGlobalObjectXMLHttpRequestConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJOAGlobalObject* thisObject = ajoa_cast<AJOAGlobalObject*>(asObject(thisValue));
		return getOAConstructor(AJXMLHttpRequestBindingType, exec, thisObject);
	}
	
#include "AJOAGlobalObject_internal.h"
}
