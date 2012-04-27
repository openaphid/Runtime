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

#include "AJNamespaceAphid.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJNamespaceCore.h"
#include "AJNamespaceG2D.h"
#include "AJNamespaceiOS.h"
#include "AJNamespaceJS.h"
//end

#include "OAGlobalObject.h"

namespace Aphid {
	using namespace AJ;

#include "AJNamespaceAphid_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceAphid::AJNamespaceAphid(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceAphid> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceAphid");
	}
	
	AJNamespaceAphid::~AJNamespaceAphid()
	{
		//oa_debug("AJNamespaceAphid destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceAphid");
	}

	///-------------------------------------------------------------------------------------------------------------------
	NamespaceAphid* toNamespaceAphid(AJValue value)
	{
		return value.inherits(&AJNamespaceAphid::s_info) ? ajoa_cast<AJNamespaceAphid*>(asObject(value))->impl() : 0;
	}

	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceAphid* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceAphid>(exec, globalObject, impl);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajNamespaceAphidCore(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNamespaceAphid* jsNamespaceAphid = ajoa_cast<AJNamespaceAphid*>(asObject(thisValue));
		return toAJ(exec, jsNamespaceAphid->globalObject(), jsNamespaceAphid->globalObject()->impl()->namespaceCore());
	}

	AJ::AJValue ajNamespaceAphidG2d(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNamespaceAphid* jsNamespaceAphid = ajoa_cast<AJNamespaceAphid*>(asObject(thisValue));
		return toAJ(exec, jsNamespaceAphid->globalObject(), jsNamespaceAphid->globalObject()->impl()->namespaceG2D());
	}
	
	AJ::AJValue ajNamespaceAphidJs(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJNamespaceAphid* jsNamespaceAphid = ajoa_cast<AJNamespaceAphid*>(asObject(thisValue));
		return toAJ(exec, jsNamespaceAphid->globalObject(), jsNamespaceAphid->globalObject()->impl()->namespaceJS());
	}
	
	AJ::AJValue ajNamespaceAphidIos(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJNamespaceAphid* jsNamespaceAphid = ajoa_cast<AJNamespaceAphid*>(asObject(thisValue));
		return toAJ(exec, jsNamespaceAphid->globalObject(), jsNamespaceAphid->globalObject()->impl()->namespaceiOS());
	}

}
