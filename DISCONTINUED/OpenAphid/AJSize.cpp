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

#include "AJSize.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJSize_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJSize::AJSize(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, Size impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJSize");
	}
	
	AJSize::~AJSize()
	{
		//oa_debug("AJSize destructed");
		AJ_LEAK_DETECT_DEC("AJSize");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJSizeConstructor::constructSize(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Size(float width, float height)
		if (args.size() == 2) {
			if (
					ensureFloatArg(exec, args, 0, "width")
					&& ensureFloatArg(exec, args, 1, "height")
					) {
				float width = args.at(0).toFloat(exec);
				float height = args.at(1).toFloat(exec);
				
				Size data = SizeMake(width, height);
				AJOAGlobalObject* globalObject = ajoa_cast<AJSizeConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJSize>(exec, globalObject, data);
			}
		}
		
		exec->clearException();
		//Size(Size s)
		if (args.size() == 1) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "s", Size)
					) {
				Size s = ajoa_cast<AJSize*>(asObject(args.at(0)))->data();
				AJOAGlobalObject* globalObject = ajoa_cast<AJSizeConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJSize>(exec, globalObject, s);
			}
		}
		
		exec->clearException();
		//Size()
		if (args.size() == 0) {
			AJOAGlobalObject* globalObject = ajoa_cast<AJSizeConstructor*>(constructor)->globalObject();
			return createAJDataWrapper<AJSize>(exec, globalObject, SizeZero);
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Size");
		return 0;
	}
	
	ConstructType AJSizeConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJSizeConstructor::constructSize;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Size toSize(AJValue value)
	{
		return value.inherits(&AJSize::s_info) ? ajoa_cast<AJSize*>(asObject(value))->data() : Size();
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Size impl)
	{
		return new (exec) AJSize(getOAStructure(AJSizeBindingType, exec, globalObject), globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSizeWidth(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSize* thisObject = ajoa_cast<AJSize*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().width);
	}
	
	void setAJSizeWidth(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJSize* ajSize = ajoa_cast<AJSize*>(thisObject);
			Size size = ajSize->data();
			size.width = value.toFloat(exec);
			ajSize->setData(size);
		} else {
			OA_ERROR_INVALID_PROPERTY(exec, "width", "float");
		}
	}
	
	AJ::AJValue ajSizeHeight(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSize* thisObject = ajoa_cast<AJSize*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().height);
	}
	
	void setAJSizeHeight(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJSize* ajSize = ajoa_cast<AJSize*>(thisObject);
			Size size = ajSize->data();
			size.height = value.toFloat(exec);
			ajSize->setData(size);
		} else {
			OA_ERROR_INVALID_PROPERTY(exec, "height", "float");
		}
	}
	
	AJ::AJValue ajSizeFunctionSetWH(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		CHECK_INHERITS(exec, thisValue, AJSize);
		
		if (args.size() >= 2) {
			if (!args.at(0).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "w", "float");
			else if (!args.at(1).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "h", "float");
			else {
				Size size;
				size.width = args.at(0).toFloat(exec);
				size.height = args.at(1).toFloat(exec);
				ajoa_cast<AJSize*>(asObject(thisValue))->setData(size); 
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 2, args.size());
		
		return jsUndefined();
	}
	
	AJ::AJValue ajSizeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSizeBindingType, exec, ajoa_cast<AJSize*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajSizePrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJSize);
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJSize::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "s", "Size");
			else {
				AJSize* thisObject = ajoa_cast<AJSize*>(asObject(thisValue));
				AJSize* other = ajoa_cast<AJSize*>(asObject(args.at(0)));
				return jsBoolean(SizeEqual(thisObject->data(), other->data()));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		OA_TODO();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajSizePrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJSize);
		AJSize* thisObject = ajoa_cast<AJSize*>(asObject(thisValue));
		return jsString(exec, toUString(thisObject->data()));
	}
	
	AJ::AJValue JSC_HOST_CALL ajSizePrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		CHECK_INHERITS(exec, thisValue, AJSize);
		AJSize* thisObject = ajoa_cast<AJSize*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSizeConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJSizeConstructor* thisObject = ajoa_cast<AJSizeConstructor*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), SizeZero);
	}
}
