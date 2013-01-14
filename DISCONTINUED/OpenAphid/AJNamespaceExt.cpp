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

#include "AJNamespaceExt.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;

#include "AJNamespaceExt_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceExt::AJNamespaceExt(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceExt> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceExt");
	}
	
	AJNamespaceExt::~AJNamespaceExt()
	{
		//oa_debug("AJNamespaceExt destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceExt");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceExt* toNamespaceExt(AJValue value)
	{
		return value.inherits(&AJNamespaceExt::s_info) ? ajoa_cast<AJNamespaceExt*>(asObject(value))->impl() : 0;
	}

	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceExt* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceExt>(exec, globalObject, impl);
	}

}
