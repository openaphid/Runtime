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
#include "ActionManager.h"

#include "OAUtil.h"

#include "Scheduler.h"
#include "OAGlobalObject.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	ActionManager::ActionManager()
	: m_markID(0)
	{
		LEAK_DETECT_INC("ActionManager");
		Scheduler::sharedScheduler()->scheduleUpdate(this, 0, false);
		//TODO: add a destroy method to remove it from scheduler
	}
	
	ActionManager::~ActionManager()
	{
		LEAK_DETECT_DEC("ActionManager");
	}
	
	ActionManager* ActionManager::sharedManager()
	{
		ActionManager* manager = OAGlobalObject::sharedInstance()->namespaceG2D()->actionManager();
		ASSERT(manager);
		return manager;
	}
	
	void ActionManager::addAction(PassRefPtr<Aphid::Action> action, PassRefPtr<Aphid::Node> target, bool paused)
	{
		ASSERT(action && target);
		
		RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
		pending->m_target = target;
		pending->m_action = action;
		pending->m_paused = paused;
		pending->m_type = PendingActionSlot_Add;
		
		pending->m_action->becomeManaged();
		
		m_pendings.append(pending.release());
	}
	
	void ActionManager::removeAction(Aphid::Action *action)
	{
		ASSERT(action);
		
		RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
		pending->m_action = action;
		pending->m_type = PendingActionSlot_Remove_Action;
		
		m_pendings.append(pending.release());
	}
	
	void ActionManager::removeActions(Aphid::Node *target)
	{
		ASSERT(target);
		
		RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
		pending->m_target = target;
		pending->m_type = PendingActionSlot_Remove_Actions;
		
		m_pendings.append(pending.release());
	}
	
	void ActionManager::removeAllActions()
	{
		RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
		pending->m_type = PendingActionSlot_Remove_All;
		
		m_pendings.append(pending.release());
	}
	
	void ActionManager::removeActionByTag(int tag, Aphid::Node *target)
	{
		ASSERT(target && tag != ActionTagInvalid);
		
		RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
		pending->m_target = target;
		pending->m_tag = tag;
		pending->m_type = PendingActionSlot_Remove_Tag;
		
		m_pendings.append(pending.release());
	}
	
	Action* ActionManager::getActionByTag(int tag, Aphid::Node *target)
	{
		ASSERT(target);
		
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end()) {
			RefPtr<ActionTargetSlot> slot = it->second;
			for (ActionVector::iterator i = slot->m_actions.begin(); i != slot->m_actions.end(); ++i)
				if (i->get()->tag() == tag)
					return i->get();
		}
		
		return 0;
	}
	
	size_t ActionManager::runningActionsCount(Aphid::Node *target)
	{
		ASSERT(target);
		
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end()) {
			RefPtr<ActionTargetSlot> slot = it->second;
			return slot->m_actions.size();
		}
		
		return 0;
	}
	
	void ActionManager::pause(Aphid::Node *target)
	{
		ASSERT(target);
		
		doPause(target);
		if (!m_pendings.isEmpty()) {
			RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
			pending->m_target = target;
			pending->m_type = PendingActionSlot_Pause;			
			m_pendings.append(pending.release());
		}			
	}
	
	void ActionManager::resume(Aphid::Node *target)
	{
		ASSERT(target);
		
		doResume(target);
		if (!m_pendings.isEmpty()) {
			RefPtr<PendingActionSlot> pending = PendingActionSlot::create();
			pending->m_target = target;
			pending->m_type = PendingActionSlot_Resume;
			m_pendings.append(pending.release());
		}
	}
	
	void ActionManager::update(TimeSec dt)
	{
		{ //process action queue
			size_t size = m_pendings.size();
			for (size_t i = 0; i < size; i++) {
				RefPtr<PendingActionSlot> pending = m_pendings[i];
				switch(pending->m_type) {
					case PendingActionSlot_Add:
						doAddAction(pending->m_action, pending->m_target, pending->m_paused);
						break;
					case PendingActionSlot_Remove_Action:
						doRemoveAction(pending->m_action.get());
						break;
					case PendingActionSlot_Remove_Actions:
						doRemoveActions(pending->m_target.get());
						break;
					case PendingActionSlot_Remove_Tag:
						doRemoveActionByTag(pending->m_tag, pending->m_target.get());
						break;
					case PendingActionSlot_Remove_All:
						doRemoveAllActions();
						break;
					case PendingActionSlot_Pause:
						doPause(pending->m_target.get());
						break;
					case PendingActionSlot_Resume:
						doResume(pending->m_target.get());
						break;
					default:
						oa_error("Unknown action type: %d", pending->m_type);
						break;
				}
			}
			
			if (size)
				m_pendings.clear();
				//m_pendings.remove(0, size);
		}
		
#if OA_DEV
		static bool s_reported = false;
		
		if (m_slots.isEmpty() && !s_reported) {
			ALIVE_REPORT();
			s_reported = true;
		}
#endif
		
		ActionTargetSlot* cursor = m_slotList.head();
		while (cursor) {
			for (ActionVector::iterator i = cursor->m_actions.begin(); i != cursor->m_actions.end(); ++i) {
				Action* action = i->get();				
				
				action->step(dt);
				
				if (action->isDone()) {
					action->stop();
					removeAction(action);
				}
			}
			
			cursor = cursor->next();
		}
	}
	
	void ActionManager::markObjects(AJ::MarkStack& markStack, unsigned markID)
	{
		if (m_markID == markID)
			return;
		
		m_markID = markID;
		
		ActionTargetSlot* cursor = m_slotList.head();
		while (cursor) {
			cursor->m_target->markObjects(markStack, markID);
#if OA_MARK_ACTION_OBJECTS
			for (ActionVector::iterator i = cursor->m_actions.begin(); i != cursor->m_actions.end(); ++i) {
				Action* action = i->get();
				action->markObjects(markStack, markID);
			}
#endif
			
			cursor = cursor->next();
		}
		
		if (!m_pendings.isEmpty()) {
			for (PendingActionQueue::iterator it = m_pendings.begin(); it != m_pendings.end(); ++it) {
				PendingActionSlot* slot = it->get();
				if (slot->m_target)
					slot->m_target->markObjects(markStack, markID);
#if OA_MARK_ACTION_OBJECTS
				if (slot->m_action)
					slot->m_action->markObjects(markStack, markID);
#endif
			}
		}
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	// Protected methods
	
	void ActionManager::doAddAction(PassRefPtr<Action> prpAction, PassRefPtr<Node> prpTarget, bool paused)
	{
		RefPtr<Action> action = prpAction;
		RefPtr<Node> target = prpTarget;
		
		ActionTargetSlotMap::iterator it = m_slots.find(target.get());
		if (it == m_slots.end()) {
			RefPtr<ActionTargetSlot> slot = ActionTargetSlot::create();
			slot->m_target = target;
			slot->m_paused = paused;
			it = m_slots.add(target.get(), slot).first;
			m_slotList.append(slot.get());
		}
		
		RefPtr<ActionTargetSlot> slot = it->second;
		
		ASSERT(slot->m_actions.find(action) == ATF::notFound);
		
		slot->m_actions.append(action);
		
		action->startWithTarget(target.get());
	}
	
	void ActionManager::doRemoveAllActions()
	{
		m_slotList.clear();
		m_slots.clear();		
	}
	
	void ActionManager::doRemoveActions(Node* target)
	{
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end()) {
			m_slotList.remove(it->second.get());
			m_slots.remove(it);
		}
	}
	
	void ActionManager::doRemoveAction(Action* action)
	{
		Node* target = action->originalTarget();
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end()) {
			int index = it->second->m_actions.find(action);
			if (index != ATF::notFound) {
				it->second->m_actions.remove(index);
				if (it->second->m_actions.isEmpty()) {
					m_slotList.remove(it->second.get());
					m_slots.remove(it);
					oa_debug("remove target %p, in doRemoveAction", target);
				}
			}
		}
	}
	
	void ActionManager::doRemoveActionByTag(int tag, Node* target)
	{
		ASSERT(tag != ActionTagInvalid && target);
		
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end()) {
			RefPtr<ActionTargetSlot> slot = it->second;
			size_t index = 0;
			for (ActionVector::iterator i = slot->m_actions.begin(); i != slot->m_actions.end(); ++i) {
				if (i->get()->tag() == tag && i->get()->originalTarget() == target) {
					slot->m_actions.remove(index);
					if (slot->m_actions.isEmpty()) {
						m_slotList.remove(it->second.get());
						m_slots.remove(it);
					}
					break;
				}
				index++;
			}
		}
	}
	
	void ActionManager::doPause(Aphid::Node *target)
	{
		ASSERT(target);
		
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end()) 
			it->second->m_paused = true;
	}
	
	void ActionManager::doResume(Aphid::Node *target)
	{
		ASSERT(target);
		
		ActionTargetSlotMap::iterator it = m_slots.find(target);
		if (it != m_slots.end())
			it->second->m_paused = false;
	}

  ///-------------------------------------------------------------------------------------------------------------------
  /// PendingActionSlot
  PendingActionSlot::PendingActionSlot() 
	: m_type(PendingActionSlot_Undefined)
	, m_target(0)
	, m_action(0)
	, m_paused(false)
	{
	  LEAK_DETECT_INC("PendingActionSlot");
  }

  PendingActionSlot::~PendingActionSlot() 
	{
	  LEAK_DETECT_DEC("PendingActionSlot");
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// ActionTargetSlot
  ActionTargetSlot::ActionTargetSlot()
	: m_target(0)
	, m_paused(0)
	{
	  LEAK_DETECT_INC("ActionTargetSlot");
  }

  ActionTargetSlot::~ActionTargetSlot() 
	{
	  LEAK_DETECT_DEC("ActionTargetSlot");
  }
} //namespace Aphid
