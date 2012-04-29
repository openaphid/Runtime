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

#ifndef OpenAphid_ActionManager_h
#define OpenAphid_ActionManager_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "Action.h"
#include "ActionInterval.h"
#include "Node.h"
#include "Scheduler.h"

#include "LinkedList.h"
#include "TimerTarget.h"

namespace Aphid {
	class AJActionManager;
	class PendingActionSlot;
	class ActionTargetSlot;
	
	typedef enum {
		PendingActionSlot_Undefined,
		PendingActionSlot_Add,
		PendingActionSlot_Remove_Action,
		PendingActionSlot_Remove_Actions,
		PendingActionSlot_Remove_Tag,
		PendingActionSlot_Remove_All,
		PendingActionSlot_Pause,
		PendingActionSlot_Resume,
	} PendingActionSlotType;
	
	typedef Vector<RefPtr<Action> > ActionVector;
	typedef _LinkedList<ActionTargetSlot*> ActionTargetSlotList;
	
	//TODO: OA impls an async APIS for add/remove, check if this can cause any problems later
	///-------------------------------------------------------------------------------------------------------------------
	class ActionManager : public RefCounted<ActionManager>, public TimerTarget {
		typedef HashMap<Node*, RefPtr<ActionTargetSlot> > ActionTargetSlotMap;
		typedef Vector<RefPtr<PendingActionSlot> > PendingActionQueue;
	public:
		virtual ~ActionManager();
		
		static PassRefPtr<ActionManager> create()
		{
			return adoptRef(new ActionManager());
		}
		
		static ActionManager* sharedManager();
		
		virtual void update(TimeSec dt);
		
		void addAction(PassRefPtr<Action> action, PassRefPtr<Node> target, bool paused);
		void removeAllActions();
		void removeActions(Node* target);
		void removeAction(Action* action);
		void removeActionByTag(int tag, Node* target);
		Action* getActionByTag(int tag, Node* target);
		size_t runningActionsCount(Node* target);
		void pause(Node * target);
		void resume(Node* target);
		
		void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		using RefCounted<ActionManager>::ref;
		using RefCounted<ActionManager>::deref;
	protected:
		ActionManager();
		
		virtual void refTimerTarget() {ref();}
		virtual void derefTimerTarget() {deref();}
		
		void doAddAction(PassRefPtr<Action> action, PassRefPtr<Node> target, bool paused);
		void doRemoveAllActions();
		void doRemoveActions(Node* target);
		void doRemoveAction(Action* action);
		void doRemoveActionByTag(int tag, Node* target);
		void doPause(Node * target);
		void doResume(Node* target);
		
	private:
		ActionTargetSlotMap m_slots;
		ActionTargetSlotList m_slotList; //weak container for fast iteration
		PendingActionQueue m_pendings;
		unsigned m_markID;
	}; // class ActionManager
	
	///-------------------------------------------------------------------------------------------------------------------
	class PendingActionSlot : public RefCounted<PendingActionSlot> {
		friend class ActionManager;
	public:
		virtual ~PendingActionSlot();
		
		static PassRefPtr<PendingActionSlot> create()
		{
			return adoptRef(new PendingActionSlot());
		}
		
	protected:
		PendingActionSlot();
		
		PendingActionSlotType m_type;
		
		RefPtr<Node> m_target;
		RefPtr<Action> m_action;
		bool m_paused;
		int m_tag;
	}; //class PendingActionSlot
	
	///-------------------------------------------------------------------------------------------------------------------
	class ActionTargetSlot : public RefCounted<ActionTargetSlot>, public _LinkElement<ActionTargetSlot> {
		friend class ActionManager;
		
	public:
		virtual ~ActionTargetSlot();
		
		static PassRefPtr<ActionTargetSlot> create()
		{
			return adoptRef(new ActionTargetSlot());
		}
		
	protected:
		ActionTargetSlot();
		
		RefPtr<Node> m_target;
		bool m_paused;
		
		ActionVector m_actions;
	}; //class ActionTargetSlot
	
}// namespace Aphid

#endif
