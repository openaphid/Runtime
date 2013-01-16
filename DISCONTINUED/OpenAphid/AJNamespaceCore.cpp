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

#include "AJNamespaceCore.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJVector2.h"
#include "AJSize.h"
#include "AJAffineTransformation.h"
#include "AJVector3.h"
#include "AJRect.h"
#include "AJPoint.h"
#include "AJColor.h"
#include "AJNamespaceJS.h"
//end

#include "OAGlobalObject.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJNamespaceCore_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceCore::AJNamespaceCore(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceCore> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceCore");
	}
	
	AJNamespaceCore::~AJNamespaceCore()
	{
		//oa_debug("AJNamespaceCore destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceCore");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceCore* toNamespaceCore(AJValue value)
	{
		return value.inherits(&AJNamespaceCore::s_info) ? ajoa_cast<AJNamespaceCore*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceCore* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceCore>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJ::AJValue ajNamespaceCoreSizeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSizeBindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceCorePointConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJPointBindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceCoreRectConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJRectBindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceCoreAffineTransformationConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJAffineTransformationBindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceCoreVector2Constructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJVector2BindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceCoreVector3Constructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJVector3BindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceCoreColorConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJColorBindingType, exec, ajoa_cast<AJNamespaceCore*>(asObject(thisValue))->globalObject());
	}	
}
