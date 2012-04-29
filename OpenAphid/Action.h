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

#ifndef OpenAphid_Action_h
#define OpenAphid_Action_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "InheritanceWrapper.h"

#include "OAData.h"

namespace Aphid {
  class AJAction;
  class Node;
	
  enum {
	  ActionTagInvalid = -1,
  };
	
	typedef enum {
		ActionTypeBase = 1,
		ActionTypeFiniteTime = 1 << 1,
		ActionTypeInterval = 1 << 2,
		ActionTypeInstant = 1 << 3,
		ActionTypeEase = 1 << 4,
		ActionTypeGrid = 1 << 5,
		ActionTypeGrid3D = 1 << 6,
		ActionTypeTiledGrid3D = 1 << 7,
		ActionTypeCamera = 1 << 8,
		ActionTypeCallFunction = 1 << 9,
	} ActionBaseTypeFlag;
	
  ///-------------------------------------------------------------------------------------------------------------------
  class Action : public RefCounted<Action>, public InheritanceWrapper {
	public:		
	  virtual ~Action();
		
	  //TODO: rename to start
		virtual void becomeManaged() {}
	  virtual void startWithTarget(Node *target) 
		{
			ASSERT(target);
			m_originalTarget = m_target = target;
		}		
	  virtual void stop();
	  virtual void step(TimeSec dt)
		{
			UNUSED_PARAM(dt);
		}
	  virtual void update(TimeSec t)
		{
			UNUSED_PARAM(t);
		}
	  virtual bool isDone() {return true;}
		
		Node* originalTarget() const {return m_originalTarget;}
		Node* target() const {return m_target;}
		
		int tag() const {return m_tag;}
		void setTag(int tag) {m_tag = tag;}
		
		bool isTimeReversed() const {return m_timeReversed;}
		void setTimeReversed(bool b) {m_timeReversed = b;}
		
		virtual int baseType() const {return ActionTypeBase;}
		virtual const char* name() const {return "Action";}
		
		virtual Action* newClone();
		
		virtual Action* newReverse();
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
	protected:		
	  Action();
		
		void dispatchUpdateToAction(Action* action, TimeSec t)
		{
			if (action) {
				if (action->supportsReversedTime() && isTimeReversed())
					action->handleUpdate(1 - t);
				else
					action->handleUpdate(t);
			}
		}
		
		virtual Action* doCreateClone() = 0;
		virtual Action* doCreateReverse() = 0;
		virtual void postSetupClone(Action* cloned)
		{
			UNUSED_PARAM(cloned);
		}
		virtual void postSetupReverse(Action* reversed)
		{
			UNUSED_PARAM(reversed);
		}
		
		virtual bool supportsReversedTime() {return true;}
		
		void handleUpdate(TimeSec t) //TODO: not 100% correct ?
		{
			if (isTimeReversed() && supportsReversedTime())
				update(1 - t);
			else
				update(t);
		}
		
	  Node* m_originalTarget;
	  Node* m_target;
	  int m_tag;
		
		bool m_timeReversed;
  }; // class Action
	
	template<class T>
	OA_RELEASE_INLINE PassRefPtr<T> toActionRef(PassRefPtr<Action> a)
	{
#if OA_DEV
		T* b = dynamic_cast<T*>(a.releaseRef());
		if (!b)
			CRASH();
		return adoptRef(b);
#else
		return adoptRef(static_cast<T*>(a.releaseRef()));
#endif
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  class FiniteTimeAction : public Action {
		typedef Action Base;
		typedef FiniteTimeAction Self;
	public:		
	  virtual ~FiniteTimeAction();
		
	  TimeSec duration() const {return m_duration;}		
	  void setDuration(TimeSec duration) {m_duration = duration;}
		
		virtual int baseType() const {return ActionTypeFiniteTime | Base::baseType();}
		virtual const char* name() const {return "FiniteTimeAction";}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		virtual Self* newClone();
		
		virtual Self* newReverse();
		
	protected:
	  FiniteTimeAction(TimeSec duration=0);
		
		virtual FiniteTimeAction* doCreateClone() = 0;
		virtual FiniteTimeAction* doCreateReverse() = 0;
		
	  TimeSec m_duration;
  }; // class FiniteTimeAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class IntervalAction : public FiniteTimeAction {
		typedef FiniteTimeAction Base;
		typedef IntervalAction Self;
	public:		
	  virtual ~IntervalAction();
		
		virtual bool isDone();
		virtual void step(TimeSec dt);
		virtual void startWithTarget(Node* target);
		
		TimeSec elapsed() const {return m_elapsed;}
		
		virtual int baseType() const {return ActionTypeInterval | Base::baseType();}
		virtual const char* name() const {return "IntervalAction";}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		virtual Self* newClone();
		
		virtual Self* newReverse();
		
	protected:		
		IntervalAction(TimeSec duration);
		
		virtual IntervalAction* doCreateClone() = 0;
		virtual IntervalAction* doCreateReverse()
		{
			return doCreateClone();
		}
		
	  TimeSec m_elapsed;
	  bool m_firstTick;
  }; //IntervalAction
	
	///-------------------------------------------------------------------------------------------------------------------
  class InstantAction : public FiniteTimeAction {
		typedef FiniteTimeAction Base;
	public:
		virtual ~InstantAction() {}
		
		virtual void becomeManaged();
		virtual bool isDone() {return m_invoked;}
		virtual void step(TimeSec t)
		{
			UNUSED_PARAM(t);
			handleUpdate(1);
		}
		virtual void update(TimeSec t)
		{
			UNUSED_PARAM(t);
		}
		
		virtual int baseType() const {return ActionTypeInstant | Base::baseType();}
		virtual const char* name() const {return "InstantAction";}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		virtual void startWithTarget(Node *target)
		{
			Base::startWithTarget(target);
			m_invoked = true;
		}
		
	protected:
		InstantAction()
		: Base(0)
		, m_invoked(false)
		{
		}
		
		bool m_invoked;
	};
	
  ///-------------------------------------------------------------------------------------------------------------------
  class RepeatForeverAction : public Action {
		typedef Action Base;
		typedef RepeatForeverAction Self;
	public:		
	  virtual ~RepeatForeverAction();
		
		static PassRefPtr<RepeatForeverAction> create(PassRefPtr<IntervalAction> action) {
			return adoptRef(new RepeatForeverAction(action));
		}
		
		virtual void startWithTarget(Node *target);
		virtual void step(TimeSec dt);
		virtual bool isDone() {return false;}
		
		virtual const char* name() const {return "RepeatForeverAction";}
		
		virtual void becomeManaged()
		{
			Base::becomeManaged();
			m_inner->becomeManaged();
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID)
		{
			if (isMarked(markID))
				return;
			
			Base::markObjects(markStack, markID);
			m_inner->markObjects(markStack, markID);
		}
		
		IntervalAction* innerAction() const {return m_inner.get();}
		void setInnerAction(PassRefPtr<IntervalAction> action) {m_inner = action;}
		
	protected:		
	  RepeatForeverAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_inner->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{		
			return new Self(adoptRef(m_inner->newReverse()));
		}
		
	  RefPtr<IntervalAction> m_inner;
  }; //class RepeatForEver
	
  ///-------------------------------------------------------------------------------------------------------------------
  class SpeedAction : public Action {
		typedef Action Base;
		typedef SpeedAction Self;
	public:		
	  virtual ~SpeedAction();
		
		static PassRefPtr<SpeedAction> create(PassRefPtr<IntervalAction> action, float speed)
		{
			return adoptRef(new SpeedAction(action, speed));
		}
		
		virtual void startWithTarget(Node *target);
		virtual void stop();
		virtual void step(TimeSec dt);
		virtual bool isDone();
		
		virtual const char* name() const {return "SpeedAction";}
		
		virtual void becomeManaged()
		{
			Base::becomeManaged();
			m_inner->becomeManaged();
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID)
		{
			if (isMarked(markID))
				return;
			
			Base::markObjects(markStack, markID);
			m_inner->markObjects(markStack, markID);
		}
		
		IntervalAction* innerAction() const {return m_inner.get();}
		void setInnerAction(PassRefPtr<IntervalAction> action) {m_inner = action;}
		
		float speed() const {return m_speed;}
		void setSpeed(float speed) {m_speed = speed;}
		
	protected:		
	  SpeedAction(PassRefPtr<IntervalAction> action, float speed);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_inner->newClone()), m_speed);
		}
		
		virtual Self* doCreateReverse()
		{
			return new SpeedAction(adoptRef(m_inner->newReverse()), m_speed);
		}
		
		RefPtr<IntervalAction> m_inner;
		float m_speed;		
  }; //class SpeedAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class FollowAction : public Action {
		typedef Action Base;
		typedef FollowAction Self;
	public:		
	  virtual ~FollowAction();
		
		static PassRefPtr<FollowAction> create(PassRefPtr<Node> node);
		static PassRefPtr<FollowAction> create(PassRefPtr<Node> node, Rect worldBoundary);
		
		virtual void step(TimeSec dt);
		virtual void stop();
		virtual bool isDone();
		
		virtual const char* name() const {return "FollowAction";}
		
	protected:		
	  FollowAction();
		
		virtual Self* doCreateClone();
		
		virtual Self* doCreateReverse()
		{
			return doCreateClone();
		}
		
		RefPtr<Node> m_followedNode;
		Rect m_worldBoundary;
		bool m_boundarySet;
		bool m_boundaryFullyCovered;
		Point m_halfScreenSize;
		Point m_fullScreenSize;
		
		float m_leftBoundary;
		float m_rightBoundary;
		float m_topBoundary;
		float m_bottomBoundary;		
  }; //class FollowAction
	
}// namespace Aphid

#endif
