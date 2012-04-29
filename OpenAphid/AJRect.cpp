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

#include "AJRect.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJPoint.h"
#include "AJSize.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJRect_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJRect::AJRect(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, Rect impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJRect");
	}
	
	AJRect::~AJRect()
	{
		//oa_debug("AJRect destructed");
		AJ_LEAK_DETECT_DEC("AJRect");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJRectConstructor::constructRect(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Rect(float x, float y, float width, float height)
		if (args.size() == 4) {
			if (
					ensureFloatArg(exec, args, 0, "x")
					&& ensureFloatArg(exec, args, 1, "y")
					&& ensureFloatArg(exec, args, 2, "width")
					&& ensureFloatArg(exec, args, 3, "height")
					) {
				float x = args.at(0).toFloat(exec);
				float y = args.at(1).toFloat(exec);
				float width = args.at(2).toFloat(exec);
				float height = args.at(3).toFloat(exec);
				
				Rect data = RectMake(x, y, width, height);
				AJOAGlobalObject* globalObject = ajoa_cast<AJRectConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJRect>(exec, globalObject, data);
			}
		}
		
		exec->clearException();
		//Rect(Point origin, Size size)
		if (args.size() == 2) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "origin", Point)
					&& ENSURE_OBJ_NULL_ARG(exec, args, 1, "size", Size)
					) {
				Point origin = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				Size size = ajoa_cast<AJSize*>(asObject(args.at(1)))->data();
				
				Rect data = RectMake2(origin, size);
				AJOAGlobalObject* globalObject = ajoa_cast<AJRectConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJRect>(exec, globalObject, data);
			}
		}
		
		exec->clearException();
		//Rect(Rect rect)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "rect", Rect)
					) {
				Rect rect = ajoa_cast<AJRect*>(asObject(args.at(0)))->data();
				
				AJOAGlobalObject* globalObject = ajoa_cast<AJRectConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJRect>(exec, globalObject, rect);
			}
		}
		
		exec->clearException();
		//Rect()
		if (args.size() == 0) {
			AJOAGlobalObject* globalObject = ajoa_cast<AJRectConstructor*>(constructor)->globalObject();
			return createAJDataWrapper<AJRect>(exec, globalObject, RectZero);
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Rect");
		return 0;
	}
	
	ConstructType AJRectConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJRectConstructor::constructRect;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Rect toRect(AJValue value)
	{
		return value.inherits(&AJRect::s_info) ? ajoa_cast<AJRect*>(asObject(value))->data() : Rect();
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Rect impl)
	{
		return new (exec) AJRect(getOAStructure(AJRectBindingType, exec, globalObject), globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajRectX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().origin.x);
	}
	
	void setAJRectX(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJRect* ajRect = ajoa_cast<AJRect*>(thisObject);
			Rect rect = ajRect->data();
			rect.origin.x = value.toFloat(exec);
			ajRect->setData(rect);
		} else {
			OA_ERROR_INVALID_PROPERTY(exec, "x", "float");
		}
	}
	
	AJ::AJValue ajRectY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().origin.y);
	}
	
	void setAJRectY(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJRect* ajRect = ajoa_cast<AJRect*>(thisObject);
			Rect rect = ajRect->data();
			rect.origin.y = value.toFloat(exec);
			ajRect->setData(rect);
		} else {
			OA_ERROR_INVALID_PROPERTY(exec, "y", "float");
		}
	}
	
	AJ::AJValue ajRectWidth(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().size.width);
	}
	
	void setAJRectWidth(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJRect* ajRect = ajoa_cast<AJRect*>(thisObject);
			Rect rect = ajRect->data();
			rect.size.width = value.toFloat(exec);
			ajRect->setData(rect);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "width", "float");
	}
	
	AJ::AJValue ajRectHeight(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().size.height);
	}
	
	void setAJRectHeight(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJRect* ajRect = ajoa_cast<AJRect*>(thisObject);
			Rect rect = ajRect->data();
			rect.size.height = value.toFloat(exec);
			ajRect->setData(rect);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "height", "float");
	}
	
	AJ::AJValue ajRectOrigin(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data().origin);
	}
	
	void setAJRectOrigin(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJPoint::s_info)) {			
			AJRect* ajRect = ajoa_cast<AJRect*>(thisObject);
			Rect rect = ajRect->data();
			rect.origin = ajoa_cast<AJPoint*>(asObject(value))->data();
			ajRect->setData(rect);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "origin", "Point");
	}
	
	AJ::AJValue ajRectSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data().size);
	}
	
	void setAJRectSize(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJSize::s_info)) {
			AJRect* ajRect = ajoa_cast<AJRect*>(thisObject);
			Rect rect = ajRect->data();
			rect.size = ajoa_cast<AJSize*>(asObject(value))->data();
			ajRect->setData(rect);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "size", "Size");
	}
	
	AJ::AJValue ajRectFunctionSetXYWH(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		CHECK_INHERITS(exec, thisValue, AJRect);
		
		if (args.size() >= 4) {
			if (!args.at(0).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "x", "float");
			else if (!args.at(1).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "y", "float");
			else if (!args.at(2).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "w", "float");
			else if (!args.at(3).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "h", "float");
			else {
				Rect rect;
				rect.origin.x = args.at(0).toFloat(exec);
				rect.origin.y = args.at(1).toFloat(exec);
				rect.size.width = args.at(2).toFloat(exec);
				rect.size.height = args.at(3).toFloat(exec);
				ajoa_cast<AJRect*>(asObject(thisValue))->setData(rect);
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 4, args.size());
		
		return jsUndefined();
	}
	
	AJ::AJValue ajRectConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJRectBindingType, exec, ajoa_cast<AJRect*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajRectPrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJRect);
		
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJRect::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "r", "Rect");
			else {
				AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
				AJRect* other = ajoa_cast<AJRect*>(asObject(args.at(0)));
				return jsBoolean(RectEqual(thisObject->data(), other->data()));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		
		OA_TODO();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajRectPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJRect);
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return jsString(exec, toUString(thisObject->data()));
	}
	
	AJ::AJValue JSC_HOST_CALL ajRectPrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJRect);
		AJRect* thisObject = ajoa_cast<AJRect*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajRectConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJRectConstructor* jsRect = ajoa_cast<AJRectConstructor*>(asObject(thisValue));
		return toAJ(exec, jsRect->globalObject(), RectZero);
	}
}
