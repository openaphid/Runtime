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

#include "AJScene.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;

#include "AJScene_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJScene::AJScene(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Scene> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJScene");
	}
	
	AJScene::~AJScene()
	{
		//oa_debug("AJScene destructed");
		AJ_LEAK_DETECT_DEC("AJScene");
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJSceneConstructor::constructScene(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		UNUSED_PARAM(args);
		RefPtr<Scene> impl = Scene::create();
		AJOAGlobalObject* globalObject = ajoa_cast<AJSceneConstructor*>(constructor)->globalObject();
		return createAJOAWrapper<AJScene>(exec, globalObject, impl.get());
	}
	
	ConstructType AJSceneConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJSceneConstructor::constructScene;
		return ConstructTypeHost;
	}

	///-------------------------------------------------------------------------------------------------------------------
	Scene* toScene(AJValue value)
	{
		return value.inherits(&AJScene::s_info) ? ajoa_cast<AJScene*>(asObject(value))->impl() : 0;
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSceneConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSceneBindingType, exec, ajoa_cast<AJScene*>(asObject(thisValue))->globalObject());
	}

}
