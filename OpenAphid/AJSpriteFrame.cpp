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

#include "AJSpriteFrame.h"

#include "SpriteFrame.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJPoint.h"
#include "AJRect.h"
#include "AJSize.h"
#include "AJTexture2D.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJSpriteFrame_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJSpriteFrame::AJSpriteFrame(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<SpriteFrame> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJSpriteFrame");
	}
	
	AJSpriteFrame::~AJSpriteFrame()
	{
		//oa_debug("AJSpriteFrame destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJSpriteFrame");
	}
	
	void AJSpriteFrame::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJObject* AJSpriteFrameConstructor::constructSpriteFrame(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//SpriteFrame(Texture2D texture, [Rect rectInPixels])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "texture", Texture2D)
					) {
				Texture2D* texture = IMPL_FROM_VALUE(Texture2D, args.at(0));
				AJRect* rectInPixels = optObjectArg<AJRect>(exec, args, 1, 0);
				
				RefPtr<SpriteFrame> impl = SpriteFrame::create(texture, rectInPixels ? rectInPixels->data() : RectMake2(PointZero, texture->contentSize()));
				AJOAGlobalObject* globalObject = ajoa_cast<AJSpriteFrameConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJSpriteFrame>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "SpriteFrame");
		return 0;
	}
	
	ConstructType AJSpriteFrameConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJSpriteFrameConstructor::constructSpriteFrame;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	SpriteFrame* toSpriteFrame(AJValue value)
	{
		return value.inherits(&AJSpriteFrame::s_info) ? ajoa_cast<AJSpriteFrame*>(asObject(value))->impl() : 0;
	}
	
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, SpriteFrame* impl)
	{
		if (!impl)
			return jsNull();
		
		AJSpriteFrame* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		
		return createAJOAWrapper<AJSpriteFrame>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSpriteFrameRect(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSpriteFrame* ajSpriteFrame = ajoa_cast<AJSpriteFrame*>(asObject(thisValue));
		return toAJ(exec, ajSpriteFrame->globalObject(), ajSpriteFrame->impl()->rect());
	}
	
	void setAJSpriteFrameRect(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJRect>(value, false)) {
			AJSpriteFrame* jsThis = ajoa_cast<AJSpriteFrame*>(thisObject);
			jsThis->impl()->setRect(ajoa_cast<AJRect*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "rect", "Rect");		
	}
	
	AJ::AJValue ajSpriteFrameRectInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSpriteFrame* ajSpriteFrame = ajoa_cast<AJSpriteFrame*>(asObject(thisValue));
		return toAJ(exec, ajSpriteFrame->globalObject(), ajSpriteFrame->impl()->rectInPixels());
	}
	
	void setAJSpriteFrameRectInPixels(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJRect>(value, false)) {
			AJSpriteFrame* jsThis = ajoa_cast<AJSpriteFrame*>(thisObject);
			jsThis->impl()->setRectInPixels(ajoa_cast<AJRect*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "rectInPixels", "Rect");
	}
	
	AJ::AJValue ajSpriteFrameRotated(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSpriteFrame* ajSpriteFrame = ajoa_cast<AJSpriteFrame*>(asObject(thisValue));
		return jsBoolean(ajSpriteFrame->impl()->rotated());
	}
	
	void setAJSpriteFrameRotated(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJSpriteFrame* jsThis = ajoa_cast<AJSpriteFrame*>(thisObject);
			jsThis->impl()->setRotated(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "rotated", "bool");
	}
	
	AJ::AJValue ajSpriteFrameTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSpriteFrame* ajSpriteFrame = ajoa_cast<AJSpriteFrame*>(asObject(thisValue));
		return toAJ(exec, ajSpriteFrame->globalObject(), ajSpriteFrame->impl()->texture());
	}
	
	void setAJSpriteFrameTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJTexture2D>(value, false)) {
			AJSpriteFrame* jsThis = ajoa_cast<AJSpriteFrame*>(thisObject);
			jsThis->impl()->setTexture(IMPL_FROM_VALUE(Texture2D, value));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "texture", "Texture2D");
	}
	
	AJ::AJValue ajSpriteFrameConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSpriteFrameBindingType, exec, ajoa_cast<AJSpriteFrame*>(asObject(thisValue))->globalObject());
	}
	
}
