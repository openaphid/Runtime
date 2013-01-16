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

#include "AJNamespaceG2D.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJAnimation.h"
#include "AJCamera.h"
#include "AJDirector.h"
#include "AJNamespaceActions.h"
#include "AJNode.h"
#include "AJScene.h"
//#include "JSScheduler.h"
#include "AJSprite.h"
#include "AJSpriteFrame.h"
#include "AJTexture2D.h"
#include "AJFont.h"
#include "AJLabelTTF.h"
//end

#include "ActionManager.h"
#include "OAGlobalObject.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJNamespaceG2D_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceG2D::AJNamespaceG2D(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceG2D> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceG2D");
	}
	
	AJNamespaceG2D::~AJNamespaceG2D()
	{
		oa_debug("AJNamespaceG2D destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceG2D");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceG2D* toNamespaceG2D(AJValue value)
	{
		return value.inherits(&AJNamespaceG2D::s_info) ? ajoa_cast<AJNamespaceG2D*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceG2D* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceG2D>(exec, globalObject, impl);		
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJValue ajNamespaceG2DNodeConstructor(ExecState* exec, AJValue thisValue, const Identifier&) 
	{
		AJNamespaceG2D* thisObject = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return getOAConstructor(AJNodeBindingType, exec, thisObject->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DColorNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJNamespaceG2D* thisObject = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return getOAConstructor(AJColorNodeBindingType, exec, thisObject->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DGradientNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJNamespaceG2D* thisObject = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return getOAConstructor(AJGradientNodeBindingType, exec, thisObject->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DSceneConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJNamespaceG2D* thisObject = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return getOAConstructor(AJSceneBindingType, exec, thisObject->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DCameraConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		AJNamespaceG2D* thisObject = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return getOAConstructor(AJCameraBindingType, exec, thisObject->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DTexture2DConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNamespaceG2D* ajNamespaceG2D = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return getOAConstructor(AJTexture2DBindingType, exec, ajNamespaceG2D->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DSpriteConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSpriteBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DSpriteBatchNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJSpriteBatchNodeBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DSpriteFrameConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJSpriteFrameBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DAnimationConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJAnimationBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DFontConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJFontBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}

	AJ::AJValue ajNamespaceG2DLabelTTFConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJLabelTTFBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}
	
	AJ::AJValue ajNamespaceG2DParticleSystemConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJParticleSystemBindingType, exec, ajoa_cast<AJNamespaceG2D*>(asObject(thisValue))->globalObject());
	}
		
	AJ::AJValue ajNamespaceG2DDirector(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNamespaceG2D* ajNamespaceG2D = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return toAJ(exec, ajNamespaceG2D->globalObject(), ajNamespaceG2D->impl()->director());
	}
	
	AJ::AJValue ajNamespaceG2DActions(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNamespaceG2D* ajNamespaceG2D = ajoa_cast<AJNamespaceG2D*>(asObject(thisValue));
		return toAJ(exec, ajNamespaceG2D->globalObject(), ajNamespaceG2D->globalObject()->impl()->namespaceActions());
	}	
}
