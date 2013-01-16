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

#include "AJColorNode.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "AJColor.h"
#include "Director.h"
//referred
#include "AJSize.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJColorNode_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJColorNode::AJColorNode(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<ColorNode> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJColorNode");
	}
	
	AJColorNode::~AJColorNode()
	{
		//oa_debug("AJColorNode destructed");
		AJ_LEAK_DETECT_DEC("AJColorNode");
	}
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJColorNodeConstructor::constructColorNode(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//ColorNode(color color, [Size size])
		if (args.size() >= 1) {
			if (
					ensureColorArg(exec, args, 0, "color")
					) {
				Color color = toColor(exec, args.at(0));
				AJSize* jsSize = optObjectArg<AJSize>(exec, args, 1, 0);
				Size size = jsSize ? jsSize->data() : Director::sharedDirector()->winSize();
				
				RefPtr<ColorNode> impl = ColorNode::create(color.rgba(), size);
				AJOAGlobalObject* globalObject = ajoa_cast<AJColorNodeConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJColorNode>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ColorNode");
		
		return 0;
	}
	
	ConstructType AJColorNodeConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJColorNodeConstructor::constructColorNode;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	ColorNode* toColorNode(AJValue value)
	{
		return value.inherits(&AJColorNode::s_info) ? ajoa_cast<AJColorNode*>(asObject(value))->impl() : 0;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajColorNodeColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJColorNode* jsColorNode = ajoa_cast<AJColorNode*>(asObject(thisValue));
		return toAJ(exec, jsColorNode->globalObject(), Color(jsColorNode->impl()->rgba()));
	}
	
	void setAJColorNodeColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (isColorValue(exec, value)) {
			AJColorNode* jsThis = ajoa_cast<AJColorNode*>(thisObject);
			jsThis->impl()->setRGBA(toColor(exec, value).rgba());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "color", "color");
	}
	
	AJ::AJValue ajColorNodeOpacity(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJColorNode* jsColorNode = ajoa_cast<AJColorNode*>(asObject(thisValue));
		return jsNumber(exec, jsColorNode->impl()->opacity());
	}
	
	void setAJColorNodeOpacity(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		 if (value.isNumber()) {
		 AJColorNode* jsThis = ajoa_cast<AJColorNode*>(thisObject);
		 jsThis->impl()->setOpacity(value.toInt32(exec));
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "opacity", "int");
	}
	
	AJ::AJValue ajColorNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJColorNodeBindingType, exec, ajoa_cast<AJColorNode*>(asObject(thisValue))->globalObject());
	}
	
}
