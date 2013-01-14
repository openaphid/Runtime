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

#include "AJVector3.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJVector3_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJVector3::AJVector3(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, Vector3 impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJVector3");
	}
	
	AJVector3::~AJVector3()
	{
		//oa_debug("AJVector3 destructed");
		AJ_LEAK_DETECT_DEC("AJVector3");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJVector3Constructor::constructVector3(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Vector3(float x, float y, float z)
		if (args.size() == 3) {
			if (
					ensureFloatArg(exec, args, 0, "x")
					&& ensureFloatArg(exec, args, 1, "y")
					&& ensureFloatArg(exec, args, 2, "z")
					) {
				float x = args.at(0).toFloat(exec);
				float y = args.at(1).toFloat(exec);
				float z = args.at(2).toFloat(exec);
				
				Vector3 data = Vector3Make(x, y, z);
				AJOAGlobalObject* globalObject = ajoa_cast<AJVector3Constructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJVector3>(exec, globalObject, data);
			}
		}
		
		exec->clearException();
		//Vector3(Vector3 v3)
		if (args.size() == 1) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "v3", Vector3)
					) {
				Vector3 v3 = ajoa_cast<AJVector3*>(asObject(args.at(0)))->data();
				
				AJOAGlobalObject* globalObject = ajoa_cast<AJVector3Constructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJVector3>(exec, globalObject, v3);
			}
		}
		
		exec->clearException();
		//Vector3()
		if (args.size() == 0) {
			AJOAGlobalObject* globalObject = ajoa_cast<AJVector3Constructor*>(constructor)->globalObject();
			return createAJDataWrapper<AJVector3>(exec, globalObject, Vector3Zero);
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Vector3");
		return 0;
	}
	
	ConstructType AJVector3Constructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJVector3Constructor::constructVector3;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Vector3 toVector3(AJValue value)
	{
		return value.inherits(&AJVector3::s_info) ? ajoa_cast<AJVector3*>(asObject(value))->data() : Vector3();
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Vector3 impl)
	{
		return new (exec) AJVector3(getOAStructure(AJVector3BindingType, exec, globalObject), globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajVector3X(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector3* thisObject = ajoa_cast<AJVector3*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().x);
	}
	
	void setAJVector3X(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJVector3* thisVector3 = ajoa_cast<AJVector3*>(thisObject);
			Vector3 v = thisVector3->data();
			v.x = value.toFloat(exec);
			thisVector3->setData(v);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "x", "float");
	}
	
	AJ::AJValue ajVector3Y(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector3* thisObject = ajoa_cast<AJVector3*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().y);
	}
	
	void setAJVector3Y(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJVector3* thisVector3 = ajoa_cast<AJVector3*>(thisObject);
			Vector3 v = thisVector3->data();
			v.y = value.toFloat(exec);
			thisVector3->setData(v);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "y", "float");
	}
	
	AJ::AJValue ajVector3Z(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector3* thisObject = ajoa_cast<AJVector3*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().z);
	}
	
	void setAJVector3Z(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJVector3* thisVector3 = ajoa_cast<AJVector3*>(thisObject);
			Vector3 v = thisVector3->data();
			v.z = value.toFloat(exec);
			thisVector3->setData(v);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "z", "float");
	}
	
	AJ::AJValue JSC_HOST_CALL ajVector3FunctionSetXYZ(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{		
		if (args.size() >= 3) {
			if (!args.at(0).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "x", "float");
			else if (!args.at(1).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "y", "float");
			else if (!args.at(2).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "z", "float");
			else {
				AJVector3* thisVector3 = ajoa_cast<AJVector3*>(asObject(thisValue));
				Vector3 v = {args.at(0).toFloat(exec), args.at(1).toFloat(exec), args.at(2).toFloat(exec)};
				thisVector3->setData(v);
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 3, args.size());
		return jsUndefined();
	}
	
	AJ::AJValue ajVector3Constructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJVector3BindingType, exec, ajoa_cast<AJVector3*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajVector3PrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJVector3);
		
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJVector3::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "v", "Vector3"); //TODO: return false???
			else {
				AJVector3* thisObject = ajoa_cast<AJVector3*>(asObject(thisValue));
				AJVector3* other = ajoa_cast<AJVector3*>(asObject(args.at(0)));
				return jsBoolean(Vector3Equal(thisObject->data(), other->data()));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajVector3PrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJVector3);
		AJVector3* thisObject = ajoa_cast<AJVector3*>(asObject(thisValue));
		return jsString(exec, toUString(thisObject->data()));
	}
	
	AJ::AJValue JSC_HOST_CALL ajVector3PrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJVector3);
		AJVector3* thisObject = ajoa_cast<AJVector3*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajVector3ConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJVector3Constructor* jsVector3 = ajoa_cast<AJVector3Constructor*>(asObject(thisValue));
		return toAJ(exec, jsVector3->globalObject(), Vector3Zero);
	}
}
