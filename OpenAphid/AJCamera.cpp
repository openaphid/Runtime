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

#include "AJCamera.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

//referred
#include "AJVector3.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJCamera_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJCamera::AJCamera(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Camera> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJCamera");
	}
	
	AJCamera::~AJCamera()
	{
		//oa_debug("AJCamera destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJCamera");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJCameraConstructor::constructCamera(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		UNUSED_PARAM(args);
		RefPtr<Camera> impl = Camera::create();
		AJOAGlobalObject* globalObject = ajoa_cast<AJCameraConstructor*>(constructor)->globalObject();
		return createAJOAWrapper<AJCamera>(exec, globalObject, impl.get());
	}
	
	ConstructType AJCameraConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJCameraConstructor::constructCamera;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Camera* toCamera(AJValue value)
	{
		return value.inherits(&AJCamera::s_info) ? ajoa_cast<AJCamera*>(asObject(value))->impl() : 0;
	}
	
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Camera* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJCamera>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJ::AJValue ajCameraEye(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJCamera* jsCamera = ajoa_cast<AJCamera*>(asObject(thisValue));
		return toAJ(exec, jsCamera->globalObject(), jsCamera->impl()->eye());
	}
	
	void setAJCameraEye(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJVector3>(value, false)) {
			AJCamera* jsThis = ajoa_cast<AJCamera*>(thisObject);
			jsThis->impl()->setEye(ajoa_cast<AJVector3*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "eye", "Vector3");
	}
	
	AJ::AJValue ajCameraCenter(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJCamera* jsCamera = ajoa_cast<AJCamera*>(asObject(thisValue));
		return toAJ(exec, jsCamera->globalObject(), jsCamera->impl()->center());
	}
	
	void setAJCameraCenter(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJVector3>(value, false)) {
			AJCamera* jsThis = ajoa_cast<AJCamera*>(thisObject);
			jsThis->impl()->setCenter(ajoa_cast<AJVector3*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "center", "Vector3");
	}
	
	AJ::AJValue ajCameraUp(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJCamera* jsCamera = ajoa_cast<AJCamera*>(asObject(thisValue));
		return toAJ(exec, jsCamera->globalObject(), jsCamera->impl()->up());
	}
	
	void setAJCameraUp(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJVector3>(value, false)) {
			AJCamera* jsThis = ajoa_cast<AJCamera*>(thisObject);
			jsThis->impl()->setUp(ajoa_cast<AJVector3*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "up", "Vector3");
	}
	
	AJ::AJValue ajCameraConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJCameraBindingType, exec, ajoa_cast<AJCamera*>(asObject(thisValue))->globalObject());
	}
}
