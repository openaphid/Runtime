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

#include "AJNamespaceiOS.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "Director.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJNamespaceiOS_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceiOS::AJNamespaceiOS(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceiOS> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceiOS");
	}
	
	AJNamespaceiOS::~AJNamespaceiOS()
	{
		//oa_debug("AJNamespaceiOS destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceiOS");
	}
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceiOS* toNamespaceiOS(AJValue value)
	{
		return value.inherits(&AJNamespaceiOS::s_info) ? ajoa_cast<AJNamespaceiOS*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceiOS* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceiOS>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajNamespaceiOSPrototypeFunctionEnableRetinaDisplay(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNamespaceiOS);
		
		//AJNamespaceiOS* thisObject = ajoa_cast<AJNamespaceiOS*>(asObject(thisValue));
		
		//enableRetinaDisplay(bool enable)
		if (args.size() >= 1) {
			if (
					ensureBoolArg(exec, args, 0, "enable")
					) {
				bool enable = args.at(0).toBoolean(exec);
				return jsBoolean(Director::sharedDirector()->enableRetinaDisplay(enable));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "enableRetinaDisplay");
		return jsUndefined();
	}
	
}
