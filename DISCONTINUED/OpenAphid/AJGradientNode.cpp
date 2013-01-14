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

#include "AJGradientNode.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "AJColor.h"
//referred
#include "AJSize.h"
#include "AJVector2.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJGradientNode_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJGradientNode::AJGradientNode(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<GradientNode> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJGradientNode");
	}
	
	AJGradientNode::~AJGradientNode()
	{
		//oa_debug("AJGradientNode destructed");
		AJ_LEAK_DETECT_DEC("AJGradientNode");
	}
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJGradientNodeConstructor::constructGradientNode(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//GradientNode(color startColor, color endColor, [Vector2 vector], [Size size])
		if (args.size() >= 2) {
			if (
					ensureColorArg(exec, args, 0, "startColor")
					&& ensureColorArg(exec, args, 1, "endColor")
					) {
				Color startColor = toColor(exec, args.at(0));
				Color endColor = toColor(exec, args.at(1));
				AJVector2* jsVector = optObjectArg<AJVector2>(exec, args, 2, 0);
				Vector2 vector = jsVector ? jsVector->data() : GradientNode::defaultVector;
				AJSize* jsSize = optObjectArg<AJSize>(exec, args, 3, 0);
				Size size = jsSize ? jsSize->data() : SizeZero;
				
				RefPtr<GradientNode> impl = GradientNode::create(startColor.rgba(), endColor.rgba(), vector, size);
				AJOAGlobalObject* globalObject = ajoa_cast<AJGradientNodeConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJGradientNode>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "GradientNode");
		return 0;
	}
	
	ConstructType AJGradientNodeConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJGradientNodeConstructor::constructGradientNode;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	GradientNode* toGradientNode(AJValue value)
	{
		return value.inherits(&AJGradientNode::s_info) ? ajoa_cast<AJGradientNode*>(asObject(value))->impl() : 0;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajGradientNodeStartColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJGradientNode* jsGradientNode = ajoa_cast<AJGradientNode*>(asObject(thisValue));
		return toAJ(exec, jsGradientNode->globalObject(), Color(jsGradientNode->impl()->startRGBA()));
	}
	
	void setAJGradientNodeStartColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		 if (isColorValue(exec, value)) {
		 AJGradientNode* jsThis = ajoa_cast<AJGradientNode*>(thisObject);
		 jsThis->impl()->setStartRGBA(toColor(exec, value).rgba());
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "startColor", "color");
	}
	
	AJ::AJValue ajGradientNodeEndColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJGradientNode* jsGradientNode = ajoa_cast<AJGradientNode*>(asObject(thisValue));
		return toAJ(exec, jsGradientNode->globalObject(), Color(jsGradientNode->impl()->endRGBA()));
	}
	
	void setAJGradientNodeEndColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		 if (isColorValue(exec, value)) {
		 AJGradientNode* jsThis = ajoa_cast<AJGradientNode*>(thisObject);
		 jsThis->impl()->setEndRGBA(toColor(exec, value).rgba());
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "endColor", "color");
	}
	
	AJ::AJValue ajGradientNodeVector(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJGradientNode* jsGradientNode = ajoa_cast<AJGradientNode*>(asObject(thisValue));
		return toAJ(exec, jsGradientNode->globalObject(), jsGradientNode->impl()->vector());
	}
	
	void setAJGradientNodeVector(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		 if (valueIsOAObject<AJVector2>(value, true)) {
		 AJGradientNode* jsThis = ajoa_cast<AJGradientNode*>(thisObject);
		 jsThis->impl()->setVector(ajoa_cast<AJVector2*>(asObject(value))->data());
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "vector", "Vector2");
	}
	
	AJ::AJValue ajGradientNodeCompressedInterpolation(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJGradientNode* jsGradientNode = ajoa_cast<AJGradientNode*>(asObject(thisValue));
		return jsBoolean(jsGradientNode->impl()->isCompressedInterpolation());
	}
	
	void setAJGradientNodeCompressedInterpolation(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		 if (value.isBoolean()) {
		 AJGradientNode* jsThis = ajoa_cast<AJGradientNode*>(thisObject);
		 jsThis->impl()->setCompressedInterpolation(value.toBoolean(exec));
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "compressInterpolation", "bool");
	}
	
	AJ::AJValue ajGradientNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJGradientNodeBindingType, exec, ajoa_cast<AJGradientNode*>(asObject(thisValue))->globalObject());
	}
	
}
