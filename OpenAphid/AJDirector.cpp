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

#include "AJDirector.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

//referred
#include "AJScene.h"
#include "AJSize.h"
#include "AJPoint.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJDirector_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJDirector::AJDirector(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Director> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJDirector");
	}
	
	AJDirector::~AJDirector()
	{
		//oa_debug("AJDirector destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJDirector");
	}
	
	
	///-------------------------------------------------------------------------------------------------------------------
	Director* toDirector(AJValue value)
	{
		return value.inherits(&AJDirector::s_info) ? ajoa_cast<AJDirector*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Director* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJDirector>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajDirectorWinSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJDirector* ajDirector = ajoa_cast<AJDirector*>(asObject(thisValue));
		return toAJ(exec, ajDirector->globalObject(), ajDirector->impl()->winSize());
	}
	
	AJ::AJValue ajDirectorWinSizeInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJDirector* ajDirector = ajoa_cast<AJDirector*>(asObject(thisValue));
		return toAJ(exec, ajDirector->globalObject(), ajDirector->impl()->winSizeInPixels());
	}
	
	AJ::AJValue ajDirectorContentScaleFactor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJDirector* ajDirector = ajoa_cast<AJDirector*>(asObject(thisValue));
		return jsNumber(exec, ajDirector->impl()->contentScaleFactor());
	}
	
	void setAJDirectorContentScaleFactor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJDirector* jsThis = ajoa_cast<AJDirector*>(thisObject);
			jsThis->impl()->setContentScaleFactor(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "contentScaleFactor", "float");
	}
	
	AJ::AJValue ajDirectorDisplayFPS(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJDirector* jsDirector = ajoa_cast<AJDirector*>(asObject(thisValue));
		return jsBoolean(jsDirector->impl()->displayFPS());
	}
	
	void setAJDirectorDisplayFPS(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		
		if (value.isBoolean()) {
			AJDirector* jsThis = ajoa_cast<AJDirector*>(thisObject);
			jsThis->impl()->setDisplayFPS(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "displayFPS", "bool");
	}
	
	AJ::AJValue ajDirectorRunningScene(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJDirector* jsDirector = ajoa_cast<AJDirector*>(asObject(thisValue));
		return toAJ(exec, jsDirector->globalObject(), jsDirector->impl()->runningScene());
	}
	
	AJ::AJValue ajDirectorFpsInterval(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, Director::s_fps_interval);
	}
	
	void setAJDirectorFpsInterval(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		UNUSED_PARAM(thisObject);
		
		if (value.isNumber() && value.toFloat(exec) > 0) {
			Director::s_fps_interval = value.toFloat(exec);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "fpsInterval", "positive float");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajDirectorPrototypeFunctionConvertToGL(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJDirector);
		AJDirector* thisObject = ajoa_cast<AJDirector*>(asObject(thisValue));
		
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJPoint::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "p", "Point");
			else {
				Point p = thisObject->impl()->convertToGL(ajoa_cast<AJPoint*>(asObject(args.at(0)))->data());				
				return toAJ(exec, thisObject->globalObject(), p);
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajDirectorPrototypeFunctionRunScene(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJDirector);
		AJDirector* thisObject = ajoa_cast<AJDirector*>(asObject(thisValue));
		
		//runScene(Scene scene)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "scene", Scene)
					) {
				Scene* scene = ajoa_cast<AJScene*>(asObject(args.at(0)))->impl();
				thisObject->impl()->replaceScene(scene);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "runScene");
		return jsUndefined();
	}
}
