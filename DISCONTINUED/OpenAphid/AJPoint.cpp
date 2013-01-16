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

#include "AJPoint.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJPoint_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJPoint::AJPoint(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, Point impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJPoint");
	}
	
	AJPoint::~AJPoint()
	{
		//oa_debug("AJPoint destructed");
		AJ_LEAK_DETECT_DEC("AJPoint");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJPointConstructor::constructPoint(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Point(float x, float y)
		if (args.size() == 2) {
			if (
					ensureFloatArg(exec, args, 0, "x")
					&& ensureFloatArg(exec, args, 1, "y")
					) {
				float x = args.at(0).toFloat(exec);
				float y = args.at(1).toFloat(exec);
				
				Point data = PointMake(x, y);
				AJOAGlobalObject* globalObject = ajoa_cast<AJPointConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJPoint>(exec, globalObject, data);
			}
		}
		
		exec->clearException();
		//Point(Point p)
		if (args.size() == 1) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "p", Point)
					) {
				Point p = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				AJOAGlobalObject* globalObject = ajoa_cast<AJPointConstructor*>(constructor)->globalObject();
				return createAJDataWrapper<AJPoint>(exec, globalObject, p);
			}
		}
		
		exec->clearException();
		//Point()
		if (args.size() == 0) {
			AJOAGlobalObject* globalObject = ajoa_cast<AJPointConstructor*>(constructor)->globalObject();
			return createAJDataWrapper<AJPoint>(exec, globalObject, PointZero);
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Point");
		return 0;
	}
	
	ConstructType AJPointConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJPointConstructor::constructPoint;
		return ConstructTypeHost;
	}
	///-------------------------------------------------------------------------------------------------------------------
	Point toPoint(AJValue value)
	{
		return value.inherits(&AJPoint::s_info) ? ajoa_cast<AJPoint*>(asObject(value))->data() : Point();
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Point impl)
	{
		return new (exec) AJPoint(getOAStructure(AJPointBindingType, exec, globalObject), globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajPointX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJPoint* thisObject = ajoa_cast<AJPoint*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().x);
	}
	
	void setAJPointX(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJPoint* ajPoint = ajoa_cast<AJPoint*>(thisObject);
			Point point = ajPoint->data();
			point.x = value.toFloat(exec);
			ajPoint->setData(point);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "x", "float");
	}
	
	AJ::AJValue ajPointY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJPoint* thisObject = ajoa_cast<AJPoint*>(asObject(thisValue));
		return jsNumber(exec, thisObject->data().y);
	}
	
	void setAJPointY(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJPoint* ajPoint = ajoa_cast<AJPoint*>(thisObject);
			Point point = ajPoint->data();
			point.y = value.toFloat(exec);
			ajPoint->setData(point);
		} else {
			OA_ERROR_INVALID_PROPERTY(exec, "y", "float");
		}
	}
	
	AJ::AJValue JSC_HOST_CALL ajPointFunctionSetXY(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		CHECK_INHERITS(exec, thisValue, AJPoint);
		
		if (args.size() >= 2) {
			if (!args.at(0).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "x", "float");
			else if (!args.at(1).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "y", "float");
			else {
				Point p = {args.at(0).toFloat(exec), args.at(1).toFloat(exec)};
				ajoa_cast<AJPoint*>(asObject(thisValue))->setData(p);
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 2, args.size());
		return jsUndefined();
	}
	
	AJ::AJValue ajPointConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJPointBindingType, exec, ajoa_cast<AJPoint*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajPointPrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJPoint);
		
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJPoint::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "p", "Point");
			else {
				AJPoint* thisObject = ajoa_cast<AJPoint*>(asObject(thisValue));
				AJPoint* other = ajoa_cast<AJPoint*>(asObject(args.at(0)));
				return jsBoolean(PointEqual(thisObject->data(), other->data()));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		OA_TODO();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajPointPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJPoint);
		AJPoint* thisObject = ajoa_cast<AJPoint*>(asObject(thisValue));
		return jsString(exec, toUString(thisObject->data()));
	}
	
	AJ::AJValue JSC_HOST_CALL ajPointPrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJPoint);
		AJPoint* thisObject = ajoa_cast<AJPoint*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->data());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajPointConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJPointConstructor* jsPoint = ajoa_cast<AJPointConstructor*>(asObject(thisValue));
		return toAJ(exec, jsPoint->globalObject(), PointZero);
	}
}
