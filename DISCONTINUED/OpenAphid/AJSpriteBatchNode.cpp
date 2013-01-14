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

#include "AJSpriteBatchNode.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJSprite.h"
#include "AJTexture2D.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJSpriteBatchNode_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJSpriteBatchNode::AJSpriteBatchNode(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<SpriteBatchNode> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJSpriteBatchNode");
	}
	
	AJSpriteBatchNode::~AJSpriteBatchNode()
	{
		//oa_debug("AJSpriteBatchNode destructed");
		AJ_LEAK_DETECT_DEC("AJSpriteBatchNode");
	}
	void AJSpriteBatchNode::markChildren(MarkStack& markStack)
	{
		Base::markChildren(markStack);
		impl()->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJSpriteBatchNodeConstructor::constructSpriteBatchNode(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//SpriteBatchNode(Texture2D texture, [int capacity])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "texture", Texture2D)
					) {
				Texture2D* texture = IMPL_FROM_VALUE(Texture2D, args.at(0));
				int capacity = optIntArg(exec, args, 1, 29);
				
				RefPtr<SpriteBatchNode> impl = capacity <= 0 ? SpriteBatchNode::create(texture) : SpriteBatchNode::create(texture, capacity);
				AJOAGlobalObject* globalObject = ajoa_cast<AJSpriteBatchNodeConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJSpriteBatchNode>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "SpriteBatchNode");
		return 0;
	}
	
	ConstructType AJSpriteBatchNodeConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJSpriteBatchNodeConstructor::constructSpriteBatchNode;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	SpriteBatchNode* toSpriteBatchNode(AJValue value)
	{
		return value.inherits(&AJSpriteBatchNode::s_info) ? ajoa_cast<AJSpriteBatchNode*>(asObject(value))->impl() : 0;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajSpriteBatchNodeTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJSpriteBatchNode* jsSpriteBatchNode = ajoa_cast<AJSpriteBatchNode*>(asObject(thisValue));
		return toAJ(exec, jsSpriteBatchNode->globalObject(), jsSpriteBatchNode->impl()->texture());
	}
	
	void setAJSpriteBatchNodeTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsOAObject<AJTexture2D>(value, false)) {
			AJSpriteBatchNode* jsThis = ajoa_cast<AJSpriteBatchNode*>(thisObject);
			jsThis->impl()->setTexture(IMPL_FROM_VALUE(Texture2D, value));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "texture", "Texture2D");
	}
	
	AJ::AJValue ajSpriteBatchNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJSpriteBatchNodeBindingType, exec, ajoa_cast<AJSpriteBatchNode*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajSpriteBatchNodePrototypeFunctionAddChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJSpriteBatchNode);
		AJSpriteBatchNode* thisObject = ajoa_cast<AJSpriteBatchNode*>(asObject(thisValue));
		//addChild(Sprite child, [int z], [int tag])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "child", Sprite)
					) {
				Sprite* child = IMPL_FROM_VALUE(Sprite, args.at(0));
				if (child->texture() == thisObject->impl()->texture()) {
					int z = optIntArg(exec, args, 1, child->zOrder());
					int tag = optIntArg(exec, args, 2, child->tag());
					thisObject->impl()->addChild(child, z, tag);
					return thisValue;
				} else {
					OA_ERROR_INVALID_ARGUMENT(exec, "child", "Sprite with the same texture as SpriteBatchNode has");
					return jsUndefined();
				}				
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "addChild");
		return jsUndefined();
	}
	
}
