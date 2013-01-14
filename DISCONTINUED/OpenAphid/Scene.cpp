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


#include "Scene.h"

#include "OAUtil.h"

#include "AJScene.h"
#include "Director.h"

namespace Aphid {
	
	Scene::Scene()
	{
		LEAK_DETECT_INC("Scene");
		
		setRelativeAnchor(false);
		Point p = PointMake(0.5f, 0.5f);
		setAnchor(p);
		setContentSize(Director::sharedDirector()->winSize());
	}
	
	Scene::~Scene()
	{
		LEAK_DETECT_DEC("Scene");
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	// InheritanceWrapper
	const AJ::ClassInfo* Scene::wrapperClassInfo() const
	{
		return &AJScene::s_info;
	}
	
	AJ::AJObject* Scene::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJScene>(exec, globalObject, this);
	}
} //namespace Aphid
