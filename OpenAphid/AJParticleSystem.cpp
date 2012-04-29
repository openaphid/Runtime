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

#include "AJParticleSystem.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJTexture2D.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJParticleSystem_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJParticleSystem::AJParticleSystem(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<ParticleSystem> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJParticleSystem");
	}
	
	AJParticleSystem::~AJParticleSystem()
	{
		//oa_debug("AJParticleSystem destructed");
		AJ_LEAK_DETECT_DEC("AJParticleSystem");
	}
	
	void AJParticleSystem::markChildren(MarkStack& markStack)
	{
		Base::markChildren(markStack);
		impl()->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJParticleSystemConstructor::constructParticleSystem(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//ParticleSystem(object config, [string type])
		if (args.size() >= 1) {
			if (
					ensureFunctionArg(exec, args, 0, "config", true)
					) {
				AJObject* config = valueAsObject(args.at(0));
				const UString& type = optStringArg(exec, args, 1, "quad");
				
				ParticleConfig cfg(exec, config);
				
				AJOAGlobalObject* globalObject = ajoa_cast<AJParticleSystemConstructor*>(constructor)->globalObject();
				if (type == "point") {
					RefPtr<ParticleSystemPoint> impl = ParticleSystemPoint::create(cfg);					
					return createAJOAWrapper<AJParticleSystem>(exec, globalObject, impl.get());
				} else {
					RefPtr<ParticleSystemQuad> impl = ParticleSystemQuad::create(cfg);					
					return createAJOAWrapper<AJParticleSystem>(exec, globalObject, impl.get());
				}
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ParticleSystem");
		return 0;
	}
	
	ConstructType AJParticleSystemConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJParticleSystemConstructor::constructParticleSystem;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	ParticleSystem* toParticleSystem(AJValue value)
	{
		return value.inherits(&AJParticleSystem::s_info) ? ajoa_cast<AJParticleSystem*>(asObject(value))->impl() : 0;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajParticleSystemConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJParticleSystemBindingType, exec, ajoa_cast<AJParticleSystem*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajParticleSystemTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJParticleSystem* jsParticleSystem = ajoa_cast<AJParticleSystem*>(asObject(thisValue));
		return toAJ(exec, jsParticleSystem->globalObject(), jsParticleSystem->impl()->texture());
	}
	
	void setAJParticleSystemTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		
		if (valueIsOAObject<AJTexture2D>(value, true)) {
			AJParticleSystem* jsThis = ajoa_cast<AJParticleSystem*>(thisObject);
			if (value.isNull())
				jsThis->impl()->setTexture(0);
			else
				jsThis->impl()->setTexture(ajoa_cast<AJTexture2D*>(asObject(value))->impl());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "texture", "Texture2D");
	}
	
	AJ::AJValue ajParticleSystemBlendAdditive(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJParticleSystem* jsParticleSystem = ajoa_cast<AJParticleSystem*>(asObject(thisValue));
		return jsBoolean(jsParticleSystem->impl()->blendAdditive());
	}
	
	void setAJParticleSystemBlendAdditive(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJParticleSystem* jsThis = ajoa_cast<AJParticleSystem*>(thisObject);
			jsThis->impl()->setBlendAdditive(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "blendAdditive", "bool");
	}
	
}
