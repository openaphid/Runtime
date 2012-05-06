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

#include "AJColor.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJColor_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJColor::AJColor(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, Color impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJColor");
	}
	
	AJColor::~AJColor()
	{
		//oa_debug("AJColor destructed");
		AJ_LEAK_DETECT_DEC("AJColor");
	}
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJColorConstructor::constructColor(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Color(color color)
		if (args.size() >= 1 && args.size() <= 2) {
			if (
					ensureColorArg(exec, args, 0, "color")
					) {
				Color color = toColor(exec, args.at(0));
				int alpha = optIntArg(exec, args, 1, color.alpha());
				color = Color(colorWithAlpha(color.rgba(), alpha));
				AJOAGlobalObject* globalObject = ajoa_cast<AJColorConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJColor>(exec, globalObject, color);
			}
		}
		
		exec->clearException();
		//Color(int r, int g, int b, [int a])
		if (args.size() >= 3) {
			if (
					ensureIntArg(exec, args, 0, "r")
					&& ensureIntArg(exec, args, 1, "g")
					&& ensureIntArg(exec, args, 2, "b")
					) {
				int r = args.at(0).toInt32(exec);
				int g = args.at(1).toInt32(exec);
				int b = args.at(2).toInt32(exec);
				int a = optIntArg(exec, args, 3, 255);
				
				Color data(makeRGBA(r, g, b, a));
				AJOAGlobalObject* globalObject = ajoa_cast<AJColorConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJColor>(exec, globalObject, data);
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Color");
		return 0;
	}
	
	ConstructType AJColorConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJColorConstructor::constructColor;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Color toColor(AJValue value)
	{
		return value.inherits(&AJColor::s_info) ? ajoa_cast<AJColor*>(asObject(value))->data() : Color();
	}
	
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Color impl)
	{
		return new (exec) AJColor(getOAStructure(AJColorBindingType, exec, globalObject), globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajColorR(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJColor* jsColor = ajoa_cast<AJColor*>(asObject(thisValue));
		return jsNumber(exec, jsColor->data().red());
	}
	
	AJ::AJValue ajColorG(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJColor* jsColor = ajoa_cast<AJColor*>(asObject(thisValue));
		return jsNumber(exec, jsColor->data().green());
	}
	
	AJ::AJValue ajColorB(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJColor* jsColor = ajoa_cast<AJColor*>(asObject(thisValue));
		return jsNumber(exec, jsColor->data().blue());
	}
	
	AJ::AJValue ajColorA(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJColor* jsColor = ajoa_cast<AJColor*>(asObject(thisValue));
		return jsNumber(exec, jsColor->data().alpha());
	}
	
	AJ::AJValue ajColorConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJColorBindingType, exec, ajoa_cast<AJColor*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajColorPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJColor);
		
		AJColor* thisObject = ajoa_cast<AJColor*>(asObject(thisValue));
		return jsString(exec, thisObject->data().name()); 
	}
	
}
