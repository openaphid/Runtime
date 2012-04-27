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

#include "AJNode.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "AJEventListener.h"
#include "OAGlobalObject.h"
//referred
#include "AJAction.h"
#include "AJCamera.h"
//#include "JSGridBase.h"
#include "AJNodeList.h"
#include "AJAffineTransformation.h"
#include "AJPoint.h"
#include "AJRect.h"
#include "AJSize.h"
#include "AJTouch.h"
#include "AJVector2.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJNode_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNode::AJNode(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Node> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNode");
	}
	
	AJNode::~AJNode()
	{
		//oa_debug("AJNode destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNode");
	}
	
	void AJNode::markChildren(AJ::MarkStack& markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJNodeConstructor::constructNode(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		UNUSED_PARAM(args);
		RefPtr<Node> impl = Node::create();
		AJOAGlobalObject* globalObject = ajoa_cast<AJNodeConstructor*>(constructor)->globalObject();
		return createAJOAWrapper<AJNode>(exec, globalObject, impl.get());
	}
	
	ConstructType AJNodeConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJNodeConstructor::constructNode;
		return ConstructTypeHost;
	}
	///-------------------------------------------------------------------------------------------------------------------
	Node* toNode(AJValue value)
	{
		return value.inherits(&AJNode::s_info) ? ajoa_cast<AJNode*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Node* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(impl->isInheritanceWrapper());
		return impl->createWrapper(exec, globalObject);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajNodeZOrder(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsNumber(exec, thisObject->impl()->zOrder());
	}
	
	AJ::AJValue ajNodeSkew(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->skew());
	}
	
	void setAJNodeSkew(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber())
			ajoa_cast<AJNode*>(thisObject)->impl()->setSkew(value.toFloat(exec), value.toFloat(exec));
		else if (value.inherits(&AJVector2::s_info)) {
			AJVector2* v = ajoa_cast<AJVector2*>(asObject(value));
			ajoa_cast<AJNode*>(thisObject)->impl()->setSkew(v->data());
		}
		else
			OA_ERROR_INVALID_PROPERTY(exec, "skew", "Vector2/float");
	}
		
	AJ::AJValue ajNodeRotation(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsNumber(exec, thisObject->impl()->rotation());
	}
	
	void setAJNodeRotation(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber())
			ajoa_cast<AJNode*>(thisObject)->impl()->setRotation(value.toFloat(exec));
		else
			OA_ERROR_INVALID_PROPERTY(exec, "rotation", "float");
	}
	
	AJ::AJValue ajNodeScale(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->scale());
	}
	
	void setAJNodeScale(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			ajoa_cast<AJNode*>(thisObject)->impl()->setScale(Vector2Make(value.toFloat(exec), value.toFloat(exec)));
		} else if (value.inherits(&AJVector2::s_info)) {
			AJVector2* v = ajoa_cast<AJVector2*>(asObject(value));
			ajoa_cast<AJNode*>(thisObject)->impl()->setScale(v->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "scale", "Vector2/float");
	}
		
	AJ::AJValue ajNodePosition(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->position());
	}
	
	void setAJNodePosition(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJPoint::s_info)) {
			AJPoint* p = ajoa_cast<AJPoint*>(asObject(value));
			ajoa_cast<AJNode*>(thisObject)->impl()->setPosition(p->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "position", "Point");
	}
	
	AJ::AJValue ajNodePositionInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->positionInPixels());
	}
	
	void setAJNodePositionInPixels(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJPoint::s_info)) {
			AJPoint* p = ajoa_cast<AJPoint*>(asObject(value));
			ajoa_cast<AJNode*>(thisObject)->impl()->setPositionInPixels(p->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "positionInPixels", "Point");
	}
	
	AJ::AJValue ajNodeCamera(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->camera());
	}
	
	AJ::AJValue ajNodeChildren(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->children());
	}
	
	AJ::AJValue ajNodeVisible(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(exec);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsBoolean(thisObject->impl()->visible());
	}
	
	void setAJNodeVisible(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setVisible(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "visible", "bool");
	}
	
	AJ::AJValue ajNodeAnchor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->anchor());
	}
	
	void setAJNodeAnchor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJPoint::s_info)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setAnchor(ajoa_cast<AJPoint*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "anchor", "Point");
	}
	
	AJ::AJValue ajNodeAnchorInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->anchorInPixels());
	}
	
	AJ::AJValue ajNodeContentSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->contentSize());
	}
	
	void setAJNodeContentSize(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJSize::s_info)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setContentSize(ajoa_cast<AJSize*>(asObject(value))->data());
		} else		
			OA_ERROR_INVALID_PROPERTY(exec, "contentSize", "Size");
	}
	
	AJ::AJValue ajNodeContentSizeInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->contentSizeInPixels());
	}
	
	void setAJNodeContentSizeInPixels(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJSize::s_info)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setContentSizeInPixels(ajoa_cast<AJSize*>(asObject(value))->data());
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "contentSizeInPixels", "Size");
	}
	
	AJ::AJValue ajNodeIsRunning(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(exec);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsBoolean(thisObject->impl()->isRunning());
	}
	
	AJ::AJValue ajNodeParent(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->parent());
	}
	
	AJ::AJValue ajNodeIsRelativeAnchor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(exec);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsBoolean(thisObject->impl()->isRelativeAnchor());
	}
	
	void setAJNodeIsRelativeAnchor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setRelativeAnchor(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "isRelativeAnchor", "bool");
	}
	
	AJ::AJValue ajNodeTag(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsNumber(exec, thisObject->impl()->tag());
	}
	
	void setAJNodeTag(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setTag(value.toInt32(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "tag", "int");
	}
	
	AJ::AJValue ajNodeBoundingBox(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->boundingBox());
	}
	
	AJ::AJValue ajNodeBoundingBoxInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->boundingBoxInPixels());
	}
	
	AJ::AJValue ajNodeOnframeupdate(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
		return ajNode->impl()->jsUpdateCallback();
	}
	
	void setAJNodeOnframeupdate(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setAJUpdateCallback(exec, valueAsObject(value));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onUpdate", "function");		
	}
	
	AJ::AJValue ajNodeOntouchstart(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajNode->impl()->findEventListener(EventFlagTouchStart));
	}
	
	void setAJNodeOntouchstart(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			AJEventListener::setAJListener(EventFlagTouchStart, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "ontouchstart", "function");
	}
	
	AJ::AJValue ajNodeOntouchmove(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajNode->impl()->findEventListener(EventFlagTouchMove));
	}
	
	void setAJNodeOntouchmove(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			AJEventListener::setAJListener(EventFlagTouchMove, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "ontouchmove", "function");
	}
	
	AJ::AJValue ajNodeOntouchend(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajNode->impl()->findEventListener(EventFlagTouchEnd));
	}
	
	void setAJNodeOntouchend(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			AJEventListener::setAJListener(EventFlagTouchEnd, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "ontouchend", "function");
	}
	
	AJ::AJValue ajNodeOntouchcancel(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajNode->impl()->findEventListener(EventFlagTouchCancel));
	}
	
	void setAJNodeOntouchcancel(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			AJEventListener::setAJListener(EventFlagTouchCancel, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "ontouchcancel", "function");
	}
	
	AJ::AJValue ajNodeTouchEnabled(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
		return jsBoolean(ajNode->impl()->isTouchEnabled());
	}
	
	void setAJNodeTouchEnabled(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isBoolean()) {
			AJNode* jsThis = ajoa_cast<AJNode*>(thisObject);
			jsThis->impl()->setTouchEnabled(value.toBoolean(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "touchEnabled", "bool");
	}
	
	AJ::AJValue ajNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJNodeBindingType, exec, ajoa_cast<AJNode*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionAddChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		
		//addChild(Node child, [int z], [int tag])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "child", Node)
					) {
				Node* child = ajoa_cast<AJNode*>(asObject(args.at(0)))->impl();
				int z = optIntArg(exec, args, 1, child->zOrder());
				int tag = optIntArg(exec, args, 2, child->tag());
				thisObject->impl()->addChild(child, z, tag);
				return thisValue;
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "addChild");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveFromParent(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		bool cleanup = optBoolArg(exec, args, 0, true);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		thisObject->impl()->removeFromParentAndCleanup(cleanup);
		
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//removeChild(Node child, [bool cleanup])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "child", Node)
					) {
				Node* child = ajoa_cast<AJNode*>(asObject(args.at(0)))->impl();
				bool cleanup = optBoolArg(exec, args, 1, true);
				thisObject->impl()->removeChild(child, cleanup);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "removeChild");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveChildByTag(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//removeChildByTag(int tag, [bool cleanup])
		if (args.size() >= 1) {
			if (
					ensureIntArg(exec, args, 0, "tag")
					) {
				int tag = args.at(0).toInt32(exec);
				bool cleanup = optBoolArg(exec, args, 1, true);
				thisObject->impl()->removeChildByTag(tag, cleanup);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "removeChildByTag");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveAllChildren(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//removeAllChildren([bool cleanup])
		bool cleanup = optBoolArg(exec, args, 0, true);
		thisObject->impl()->removeAllChildren(cleanup);
		
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionGetChildByTag(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//getChildByTag(int tag)
		if (args.size() >= 1) {
			if (
					ensureIntArg(exec, args, 0, "tag")
					) {
				int tag = args.at(0).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->getChildByTag(tag));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "getChildByTag");
		return jsUndefined();	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionReorderChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		//TODO: AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		if (args.size() >= 2) {
			if (!args.at(0).inherits(&AJNode::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "child", "Node");
			else if (!args.at(1).isNumber())
				OA_ERROR_INVALID_ARGUMENT(exec, "z", "int");
			else {
				//TODO: 
				AJNode* ajNode = ajoa_cast<AJNode*>(asObject(thisValue));
				AJNode* jsChild = ajoa_cast<AJNode*>(asObject(args.at(0)));
				ajNode->impl()->reorderChild(jsChild->impl(), args.at(1).toInt32(exec));
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 2, args.size());
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionCleanup(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		thisObject->impl()->cleanup();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRunAction(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		
		if (args.size() >= 1) {
			if (!args.at(0).inherits(&AJAction::s_info))
				OA_ERROR_INVALID_ARGUMENT(exec, "action", "Action");
			else {				
				Action* action = ajoa_cast<AJAction*>(asObject(args.at(0)))->impl();
				thisObject->impl()->runAction(action);
			}
		} else
			OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, 1, args.size());
		
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionStopAllActions(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		
		thisObject->impl()->stopAllActions();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionScheduleTimer(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//scheduleTimer(function callback, float interval, [int repeatTimes], [float delay])
		if (args.size() >= 2) {
			if (
					ensureFunctionArg(exec, args, 0, "callback", false)
					&& ensureFloatArg(exec, args, 1, "interval")
					) {
				AJObject* callback = asObject(args.at(0));
				float interval = args.at(1).toFloat(exec);
				int repeatTimes = optIntArg(exec, args, 2, -1);
				float delay = optFloatArg(exec, args, 3, 0);
				thisObject->impl()->schedule(callback, interval, repeatTimes, delay);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "scheduleTimer");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionUnscheduleTimer(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		
		//unscheduleTimer(function callback)
		if (args.size() >= 1) {
			if (
					ensureFunctionArg(exec, args, 0, "callback", false)
					) {
				AJObject* callback = asObject(args.at(0));
				thisObject->impl()->unschedule(callback);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "unscheduleTimer");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionUnscheduleAllTimers(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		thisObject->impl()->unscheduleAllCallbacks();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionScheduleUpdate(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		thisObject->impl()->scheduleUpdate();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionUnscheduleUpdate(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		thisObject->impl()->unscheduleUpdate();
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToNodeSpace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//convertToNodeSpace(Point p)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "p", Point)
					) {
				Point p = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->convertToNodeSpace(p));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "convertToNodeSpace");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertParentToNodeSpace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args)
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//convertParentToNodeSpace(Point p)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "p", Point)
					) {
				Point p = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->convertParentToNodeSpace(p));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "convertToNodeSpace");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToWorldSpace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		
		//convertToWorldSpace(Point p)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "p", Point)
					) {
				Point p = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->convertToWorldSpace(p));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "convertToWorldSpace");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToNodeSpaceAR(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//convertToNodeSpaceAR(Point p)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "p", Point)
					) {
				Point p = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->convertToNodeSpaceAR(p));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "convertToNodeSpaceAR");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToWorldSpaceAR(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//convertToWorldSpaceAR(Point p)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "p", Point)
					) {
				Point p = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				return toAJ(exec, thisObject->globalObject(), thisObject->impl()->convertToWorldSpaceAR(p));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "convertToWorldSpaceAR");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionLocationOfTouch(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//locationOfTouch(Touch touch)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "touch", Touch)
					) {
				Touch* touch = IMPL_FROM_VALUE(Touch, args.at(0));
				Point p = thisObject->impl()->convertToNodeSpace(touch->clientLocation());
				return toAJ(exec, thisObject->globalObject(), p);
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "touchLocation");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionLocationAROfTouch(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJNode);
		
		AJNode* thisObject = ajoa_cast<AJNode*>(asObject(thisValue));
		//locationOfTouch(Touch touch)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "touch", Touch)
					) {
				Touch* touch = IMPL_FROM_VALUE(Touch, args.at(0));
				Point p = thisObject->impl()->convertToNodeSpaceAR(touch->clientLocation());
				return toAJ(exec, thisObject->globalObject(), p);
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "touchLocation");
		return jsUndefined();
	}
	
	AJ::AJValue ajNodeConstructorINVALID_TAG(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, NodeTagInvalid);
	}
	
}
