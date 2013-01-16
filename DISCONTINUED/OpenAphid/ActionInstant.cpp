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

#include "ActionInstant.h"
#include "Node.h"
#include "Sprite.h"
#include "AJOABinding.h"
#include "OAGlobalObject.h"
#include "AJNode.h"

namespace Aphid {
	using namespace AJ;
  
	///-------------------------------------------------------------------------------------------------------------------
	/// ShowAction
	ShowAction::ShowAction()
	{
		LEAK_DETECT_INC("ShowAction");
	}
	
	ShowAction::~ShowAction()
	{
		LEAK_DETECT_DEC("ShowAction");
	}
	
	void ShowAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_target->setVisible(true);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// HideAction
	HideAction::HideAction()
	{
		LEAK_DETECT_INC("HideAction");
	}
	
	HideAction::~HideAction()
	{
		LEAK_DETECT_DEC("HideAction");
	}
	
	void HideAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_target->setVisible(false);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// ToggleVisibleAction
	ToggleVisibleAction::ToggleVisibleAction()
	{
		LEAK_DETECT_INC("ToggleVisibleAction");
	}
	
	ToggleVisibleAction::~ToggleVisibleAction()
	{
		LEAK_DETECT_DEC("ToggleVisibleAction");
	}
	
	void ToggleVisibleAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_target->setVisible(!m_target->visible());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// FlipXAction
	FlipXAction::FlipXAction(bool x)
	: m_flipX(x)
	{
		LEAK_DETECT_INC("FlipXAction");
	}
	
	FlipXAction::~FlipXAction()
	{
		LEAK_DETECT_DEC("FlipXAction");
	}
	
	void FlipXAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		Sprite* sprite = m_target->toSprite();
		if (sprite)
			sprite->setFlipX(m_flipX);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// FlipYAction
	FlipYAction::FlipYAction(bool y)
	: m_flipY(y)
	{
		LEAK_DETECT_INC("FlipYAction");
	}
	
	FlipYAction::~FlipYAction()
	{
		LEAK_DETECT_DEC("FlipYAction");
	}
	
	void FlipYAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		Sprite* sprite = m_target->toSprite();
		if (sprite)
			sprite->setFlipY(m_flipY);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// PlaceAction
	PlaceAction::PlaceAction(const Point& p)
	: m_position(p)
	{
		LEAK_DETECT_INC("PlaceAction");
	}
	
	PlaceAction::~PlaceAction()
	{
		LEAK_DETECT_DEC("PlaceAction");
	}
	
	void PlaceAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_target->setPosition(m_position);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// CallAJFunctionAction
	CallAJFunctionAction::CallAJFunctionAction(AJObject* callback)
	{
		LEAK_DETECT_INC("CallAJFunctionAction");

		m_function = AJObjectManager::sharedManager()->putMapped(callback);
	}
	
	CallAJFunctionAction::~CallAJFunctionAction()
	{
		LEAK_DETECT_DEC("CallAJFunctionAction");
		if (!this->isDone())
			AJObjectManager::sharedManager()->dismissCallAJFunctionAction(this);
	}
	
	void CallAJFunctionAction::becomeManaged()
	{
		AJObjectManager::sharedManager()->putCallAJFunctionAction(this);
	}
	
	void CallAJFunctionAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		if (m_function) {			
			AJOAGlobalObject* globalObject = OAGlobalObject::sharedInstance()->wrapper();
			ASSERT(globalObject);
			
			ExecState* exec = globalObject->globalExec();
			
			AJValue jsTarget = toAJ(exec, globalObject, target);
			AJObject* callback = m_function->optObject();
			ASSERT(callback);
			
			CallData callData;
			CallType callType = callback->getCallData(callData);
			
			if (callType != CallTypeNone) {
				ref();
				
				MarkedArgumentBuffer args;
				args.append(jsTarget);
				args.append(wrapper());
				
				AJ::call(exec, callback, callType, callData, jsTarget, args);
				
				if (exec->hadException())
					reportAJException(exec, exec->exception());
				
				deref();
			} else 
				oa_error("Invalid call type for invokeUpdate: %d", callType);
		}
		
		AJObjectManager::sharedManager()->dismissCallAJFunctionAction(this);//TODO: is this enough to release this object from GC and ref counting?
	}
	
	void CallAJFunctionAction::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		Base::markObjects(markStack, markID);
		if (m_function)
			m_function->markObject(markStack, markID);
	}
}
