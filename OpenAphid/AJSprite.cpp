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

#include "AJSprite.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJPoint.h"
#include "AJRect.h"
#include "AJTexture2D.h"
#include "AJColor.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJSprite_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJSprite::AJSprite(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Sprite> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJSprite");
	}
	
	AJSprite::~AJSprite()
	{
		//oa_debug("AJSprite destructed");
		AJ_LEAK_DETECT_DEC("AJSprite");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJSpriteConstructor::constructSprite(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Sprite(Texture2D texture, [Rect rect])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_NULL_ARG(exec, args, 0, "texture", Texture2D)
					) {
				Texture2D* texture = OPT_IMPL_FROM_VALUE(Texture2D, args.at(0));
				AJRect* rect = optObjectArg<AJRect>(exec, args, 1, 0);
				
				RefPtr<Sprite> impl = rect ? Sprite::create(texture, rect->data()) : Sprite::create(texture);
				AJOAGlobalObject* globalObject = ajoa_cast<AJSpriteConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJSprite>(exec, globalObject, impl.get());
			}
		}
		
		exec->clearException();
		//Sprite()
		if (args.size() == 0) {
			RefPtr<Sprite> impl = Sprite::create();
			AJOAGlobalObject* globalObject = ajoa_cast<AJSpriteConstructor*>(constructor)->globalObject();
			return createAJOAWrapper<AJSprite>(exec, globalObject, impl.get());
		}
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Sprite");
		return 0;
	}
	
	ConstructType AJSpriteConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJSpriteConstructor::constructSprite;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Sprite* toSprite(AJValue value)
	{
		return value.inherits(&AJSprite::s_info) ? ajoa_cast<AJSprite*>(asObject(value))->impl() : 0;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSpriteOffsetPositionInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* ajSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return toAJ(exec, ajSprite->globalObject(), ajSprite->impl()->offsetPositionInPixels());
	}
	
	AJ::AJValue ajSpriteTextureRect(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* jsSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return toAJ(exec, jsSprite->globalObject(), jsSprite->impl()->textureRect());
	}
	
	void setAJSpriteTextureRect(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJRect>(value, false)) {
			AJSprite* jsThis = ajoa_cast<AJSprite*>(thisObject);
			jsThis->impl()->setTextureRect(ajoa_cast<AJRect*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "textureRect", "Rect");
	}
	
	AJ::AJValue ajSpriteFlipX(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* ajSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return jsBoolean(ajSprite->impl()->flipX());
	}
	
	void setAJSpriteFlipX(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJSprite* jsThis = ajoa_cast<AJSprite*>(thisObject);
			jsThis->impl()->setFlipX(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "flipX", "bool");
	}
	
	AJ::AJValue ajSpriteFlipY(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* ajSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return jsBoolean(ajSprite->impl()->flipY());
	}
	
	void setAJSpriteFlipY(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJSprite* jsThis = ajoa_cast<AJSprite*>(thisObject);
			jsThis->impl()->setFlipY(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "flipY", "bool");
	}
	
	AJ::AJValue ajSpriteOpacity(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* jsSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return jsNumber(exec, jsSprite->impl()->opacity());
	}
	
	void setAJSpriteOpacity(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJSprite* jsThis = ajoa_cast<AJSprite*>(thisObject);
			jsThis->impl()->setOpacity(value.toInt32(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "opacity", "int");
	}
	
	AJ::AJValue ajSpriteColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* jsSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return toAJ(exec, jsSprite->globalObject(), Color(jsSprite->impl()->rgba()));
	}
	
	void setAJSpriteColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (isColorValue(exec, value)) {
			AJSprite* jsThis = ajoa_cast<AJSprite*>(thisObject);
			jsThis->impl()->setRGBA(toColor(exec, value).rgba());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "color", "color");
	}
	
	AJ::AJValue ajSpriteTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSprite* jsSprite = ajoa_cast<AJSprite*>(asObject(thisValue));
		return toAJ(exec, jsSprite->globalObject(), jsSprite->impl()->texture());
	}
	
	void setAJSpriteTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJTexture2D>(value, true)) {
			AJSprite* jsThis = ajoa_cast<AJSprite*>(thisObject);
			jsThis->impl()->setTexture(OPT_IMPL_FROM_VALUE(Texture2D, value));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "texture", "Texture2D");
	}
	
	AJ::AJValue ajSpriteConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSpriteBindingType, exec, ajoa_cast<AJSprite*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSpriteConstructorDebugDrawOutline(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsBoolean(Sprite::s_debug_draw_outline);
	}
	
	void setAJSpriteConstructorDebugDrawOutline(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		UNUSED_PARAM(thisObject);
		
		if (value.isBoolean()) {
			Sprite::s_debug_draw_outline = value.toBoolean(exec);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "debugDrawOutline", "bool");
	}
	
	AJ::AJValue ajSpriteConstructorDebugDrawTextureOutline(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsBoolean(Sprite::s_debug_draw_texture_outline);
	}
	
	void setAJSpriteConstructorDebugDrawTextureOutline(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		UNUSED_PARAM(thisObject);
		
		if (value.isBoolean()) {
			Sprite::s_debug_draw_texture_outline = value.toBoolean(exec);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "debugDrawTextureOutline", "bool");
	}
	
}
