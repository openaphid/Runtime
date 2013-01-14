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

#include "AJVector2.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJVector2_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJVector2::AJVector2(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, Vector2 impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJVector2");
	}
	
	AJVector2::~AJVector2()
	{
		//oa_debug("AJVector2 destructed");
		AJ_LEAK_DETECT_DEC("AJVector2");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJVector2Constructor::constructVector2(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Vector2(float x, float y)
		if (args.size() == 2) {
			if (
					ensureFloatArg(exec, args, 0, "x")
					&& ensureFloatArg(exec, args, 1, "y")
					) {
				float x = args.at(0).toFloat(exec);
				float y = args.at(1).toFloat(exec);
				
				Vector2 data = Vector2Make(x, y);
				AJOAGlobalObject* globalObject = ajoa_cast<AJVector2Constructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJVector2>(exec, globalObject, data);
			}
		}
		
		exec->clearException();
		//Vector2(Vector2 v2)
		if (args.size() == 1) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "v2", Vector2)
					) {
				Vector2 v2 = ajoa_cast<AJVector2*>(asObject(args.at(0)))->data();
				
				AJOAGlobalObject* globalObject = ajoa_cast<AJVector2Constructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJVector2>(exec, globalObject, v2);
			}
		}
		
		exec->clearException();
		//Vector2()
		if (args.size() == 0) {
			AJOAGlobalObject* globalObject = ajoa_cast<AJVector2Constructor*>(constructor)->globalObject();
			return createAJDataWrapper<AJVector2>(exec, globalObject, Vector2Zero);
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Vector2");
		return 0;
	}
	
	ConstructType AJVector2Constructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJVector2Constructor::constructVector2;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Vector2 toVector2(AJValue value)
	{
		return value.inherits(&AJVector2::s_info) ? ajoa_cast<AJVector2*>(asObject(value))->data() : Vector2();
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Vector2 impl)
	{
		return new (exec) AJVector2(getOAStructure(AJVector2BindingType, exec, globalObject), globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajVector2X(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector2* thisObject = ajoa_cast<AJVector2*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().x);
	}
	
	void setAJVector2X(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJVector2* thisVector2 = ajoa_cast<AJVector2*>(thisObject);
			Vector2 v = thisVector2->data();
			v.x = value.toFloat(exec);
			thisVector2->setData(v);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "x", "float");
	}
	
	AJ::AJValue ajVector2Y(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector2* thisObject = ajoa_cast<AJVector2*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().y);
	}
	
	void setAJVector2Y(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJVector2* thisVector2 = ajoa_cast<AJVector2*>(thisObject);
			Vector2 v = thisVector2->data();
			v.y = value.toFloat(exec);
			thisVector2->setData(v);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "y", "float");
	}
	
	AJ::AJValue JSC_HOST_CALL ajVector2FunctionSetXY(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		if (args.size() >= 2 && args.at(0).isNumber() && args.at(1).isNumber()) {
			AJVector2* thisVector2 = ajoa_cast<AJVector2*>(asObject(thisValue));
			Vector2 v = {args.at(0).toFloat(exec), args.at(1).toFloat(exec)};
			thisVector2->setData(v);
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 2, args.size());
		return jsUndefined();
	}
	
	AJ::AJValue ajVector2Constructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJVector2BindingType, exec, ajoa_cast<AJVector2*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajVector2PrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJVector2);
		//TODO: AJVector2* thisObject = ajoa_cast<AJVector2*>(asObject(thisValue));
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJVector2::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "v", "Vector2");
			else {
				AJVector2* thisObject = ajoa_cast<AJVector2*>(asObject(thisValue));
				AJVector2* other = ajoa_cast<AJVector2*>(asObject(args.at(0)));
				return jsBoolean(Vector2Equal(thisObject->data(), other->data()));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		OA_TODO();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajVector2PrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJVector2);
		AJVector2* thisObject = ajoa_cast<AJVector2*>(asObject(thisValue));
		return jsString(exec, toUString(thisObject->data()));
	}
	
	AJ::AJValue JSC_HOST_CALL ajVector2PrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJVector2);
		AJVector2* thisObject = ajoa_cast<AJVector2*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajVector2ConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector2Constructor* jsVector2 = ajoa_cast<AJVector2Constructor*>(asObject(thisValue));
		return toAJ(exec, jsVector2->globalObject(), Vector2Zero);
	}
}
