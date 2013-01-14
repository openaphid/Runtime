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

#include "AJAnimation.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJSpriteFrame.h"
//end

namespace Aphid {
	using namespace AJ;

#include "AJAnimation_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJAnimation::AJAnimation(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Animation> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJAnimation");
	}
	
	AJAnimation::~AJAnimation()
	{
		//oa_debug("AJAnimation destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJAnimation");
	}
	
	void AJAnimation::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJAnimationConstructor::constructAnimation(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Animation()
		if (args.size() == 0) {
			RefPtr<Animation> impl = Animation::create();
			AJOAGlobalObject* globalObject = ajoa_cast<AJAnimationConstructor*>(constructor)->globalObject();
			return createAJOAWrapper<AJAnimation>(exec, globalObject, impl.get());
		}
		
		//Animation(Array frames)
		if (args.size() >= 1) {
			if (
					ensureArrayArg(exec, args, 0, "frames", false)
					) {
				AJArray* frames = ajoa_cast<AJArray*>(asObject(args.at(0)));
				float delay = optFloatArg(exec, args, 1, 1.0f/60);
				
				RefPtr<Animation> impl = Animation::create();
				
				impl->setDelay(delay);
				
				size_t size  = frames->length();
				for (size_t i = 0; i < size; i++) {
					AJObject* o = asObject(frames->getIndex(i));
					if (o->inherits(&AJSpriteFrame::s_info))
						impl->addFrame(ajoa_cast<AJSpriteFrame*>(o)->impl());
					else {
						OA_ERROR_INVALID_ARGUMENT(exec, "frames", "Array of SpriteFrames");
						break;
					}
				}
				
				if (!exec->hadException()) {
					AJOAGlobalObject* globalObject = ajoa_cast<AJAnimationConstructor*>(constructor)->globalObject();
					return createAJOAWrapper<AJAnimation>(exec, globalObject, impl.get());
				}
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Animation");
		return 0;
	}
	
	ConstructType AJAnimationConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJAnimationConstructor::constructAnimation;
		return ConstructTypeHost;
	}

	///-------------------------------------------------------------------------------------------------------------------
	Animation* toAnimation(AJValue value)
	{
		return value.inherits(&AJAnimation::s_info) ? ajoa_cast<AJAnimation*>(asObject(value))->impl() : 0;
	}

	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Animation* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJAnimation>(exec, globalObject, impl);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajAnimationDelay(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAnimation* ajAnimation = ajoa_cast<AJAnimation*>(asObject(thisValue));
		return jsNumber(exec, ajAnimation->impl()->delay());
	}

	void setAJAnimationDelay(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAnimation* jsThis = ajoa_cast<AJAnimation*>(thisObject);
			jsThis->impl()->setDelay(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "delay", "float");
	}

	AJ::AJValue ajAnimationConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJAnimationBindingType, exec, ajoa_cast<AJAnimation*>(asObject(thisValue))->globalObject());
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajAnimationPrototypeFunctionAddFrame(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJAnimation);

		AJAnimation* thisObject = ajoa_cast<AJAnimation*>(asObject(thisValue));
		
		// addFrame(SpriteFrame frame)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "frame", SpriteFrame)
					) {
				SpriteFrame* frame = ajoa_cast<AJSpriteFrame*>(asObject(args.at(0)))->impl();
				thisObject->impl()->addFrame(frame);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "addFrame");
		return jsUndefined();
	}

}
