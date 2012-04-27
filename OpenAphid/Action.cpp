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
#include "Action.h"

#include "OAUtil.h"

#include "Node.h"

#include "Director.h"

#include "G2DMacros.h"
#include "AJAction.h"
#include "AJFiniteTimeAction.h"
#include "AJIntervalAction.h"
#include "AJInstantAction.h"

#include <algorithm>

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// Action
	Action::Action()
	: m_originalTarget(0)
	, m_target(0)
	, m_tag(ActionTagInvalid)
	, m_timeReversed(false)
	{
		LEAK_DETECT_INC("Action");
	}
	
	Action::~Action()
	{
		LEAK_DETECT_DEC("Action");
	}
	
	const AJ::ClassInfo* Action::wrapperClassInfo() const
	{
		return &AJAction::s_info;
	}
	
	AJ::AJObject* Action::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJAction>(exec, globalObject, this);
	}
	
	void Action::stop()
	{
		m_target = 0;		
	}
	
	Action* Action::newClone()
	{
		Action* action = doCreateClone();
		action->setTag(m_tag);
		action->setTimeReversed(m_timeReversed);
		postSetupClone(action);
		return action;
	}
	
	Action* Action::newReverse()
	{
		Action* action = doCreateReverse();
		action->setTag(m_tag);
		action->setTimeReversed(m_timeReversed);
		postSetupReverse(action);
		return action;
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FiniteTimeAction
  FiniteTimeAction::FiniteTimeAction(TimeSec duration) 
	: m_duration(duration = 0 ? FLT_EPSILON : duration)
	{
	  LEAK_DETECT_INC("FiniteTimeAction");
  }
	
  FiniteTimeAction::~FiniteTimeAction() {
	  LEAK_DETECT_DEC("FiniteTimeAction");
  }
	
	const AJ::ClassInfo* FiniteTimeAction::wrapperClassInfo() const
	{
		return &AJFiniteTimeAction::s_info;
	}
	
	AJ::AJObject* FiniteTimeAction::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJFiniteTimeAction>(exec, globalObject, this);
	}	
	
	FiniteTimeAction* FiniteTimeAction::newClone()
	{
		return static_cast<Self*>(Base::newClone());
	}
	
	FiniteTimeAction* FiniteTimeAction::newReverse()
	{
		return static_cast<Self*>(Base::newReverse());
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// IntervalAction	
	IntervalAction::IntervalAction(TimeSec duration)
	: m_elapsed(0)
	, m_firstTick(true)
	{
		LEAK_DETECT_INC("IntervalAction");
		m_duration = duration;
	}
	
  IntervalAction::~IntervalAction() {
	  LEAK_DETECT_DEC("IntervalAction");
  }
	
	bool IntervalAction::isDone()
	{
		return m_elapsed >= m_duration;
	}
	
	void IntervalAction::step(TimeSec dt)
	{
		if(m_firstTick) {
			m_firstTick = false;
			m_elapsed = 0;
		} else
			m_elapsed += dt;
		
		/*
		if (m_timeReversed)
			update(1 - std::min(1.0f, m_elapsed/m_duration));
		else
			update(std::min(1.0f, m_elapsed/m_duration));
		 */
		handleUpdate(std::min(1.0f, m_elapsed/m_duration));
	}
	
	void IntervalAction::startWithTarget(Node *target)
	{
		Base::startWithTarget(target);
		m_elapsed = 0.0f;
		m_firstTick = true;
	}
	
	const AJ::ClassInfo* IntervalAction::wrapperClassInfo() const
	{
		return &AJIntervalAction::s_info;
	}
	
	AJ::AJObject* IntervalAction::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJIntervalAction>(exec, globalObject, this);
	}
	
	IntervalAction* IntervalAction::newClone()
	{
		return static_cast<Self*>(Base::newClone());
	}
	
	IntervalAction* IntervalAction::newReverse()
	{
		return static_cast<Self*>(Base::newReverse());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// InstantAction
	void InstantAction::becomeManaged()
	{
		Base::becomeManaged();
		m_invoked = true;
	}
	
	const AJ::ClassInfo* InstantAction::wrapperClassInfo() const
	{
		return &AJInstantAction::s_info;
	}
	
	AJ::AJObject* InstantAction::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJInstantAction>(exec, globalObject, this);
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// RepeatForeverAction
  RepeatForeverAction::RepeatForeverAction(PassRefPtr<IntervalAction> action) 
	: m_inner(action)
	{
		ASSERT(m_inner);
	  LEAK_DETECT_INC("RepeatForeverAction");
  }
	
  RepeatForeverAction::~RepeatForeverAction()
	{
	  LEAK_DETECT_DEC("RepeatForeverAction");
  }
	
	void RepeatForeverAction::startWithTarget(Node *target)
	{
		ASSERT(m_inner);
		Base::startWithTarget(target);
		m_inner->startWithTarget(target);
	}
	
	void RepeatForeverAction::step(TimeSec dt)
	{
		ASSERT(m_inner);
		m_inner->step(dt);
		if (m_inner->isDone()) {
			TimeSec diff = dt + m_inner->duration() - m_inner->elapsed();
			m_inner->startWithTarget(m_target);
			// to prevent jerk. issue #390
			m_inner->step(diff);
		}
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// SpeedAction
  SpeedAction::SpeedAction(PassRefPtr<IntervalAction> action, float speed)
	: m_inner(action)
	, m_speed(speed)
	{
		ASSERT(m_inner);
	  LEAK_DETECT_INC("SpeedAction");
  }
	
  SpeedAction::~SpeedAction() 
	{
	  LEAK_DETECT_DEC("SpeedAction");
  }
	
	void SpeedAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_inner->startWithTarget(target);
	}
	
	void SpeedAction::stop()
	{
		m_inner->stop();
		Base::stop();
	}
	
	void SpeedAction::step(TimeSec dt)
	{
		m_inner->step(dt * m_speed);
	}
	
	bool SpeedAction::isDone()
	{
		return m_inner->isDone();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FollowAction
  FollowAction::FollowAction() 
	: m_followedNode(0)
	, m_worldBoundary(RectZero)
	, m_boundarySet(false)
	, m_boundaryFullyCovered(false)
	, m_halfScreenSize(PointZero)
	, m_fullScreenSize(PointZero)
	, m_leftBoundary(0)
	, m_rightBoundary(0)
	, m_topBoundary(0)
	, m_bottomBoundary(0)
	{
	  LEAK_DETECT_INC("FollowAction");
  }
	
  FollowAction::~FollowAction() 
	{
	  LEAK_DETECT_DEC("FollowAction");
  }
	
	PassRefPtr<FollowAction> FollowAction::create(PassRefPtr<Aphid::Node> node)
	{
		FollowAction* action = new FollowAction();
		
		action->m_followedNode = node;
		action->m_boundarySet = false;
		action->m_boundaryFullyCovered = false;
		
		Size s = Director::sharedDirector()->winSize();
		action->m_fullScreenSize = PointMake(s.width, s.height);
		action->m_halfScreenSize = g2d_mult(action->m_fullScreenSize, .5f);
		
		return adoptRef(action);
	}
	
	PassRefPtr<FollowAction> FollowAction::create(PassRefPtr<Aphid::Node> node, Rect rect)
	{
		FollowAction* action = new FollowAction();
		
		action->m_followedNode = node;
		action->m_worldBoundary = rect;
		action->m_boundarySet = true;
		action->m_boundaryFullyCovered = false;
		
		Size s = Director::sharedDirector()->winSize();
		action->m_fullScreenSize = PointMake(s.width, s.height);
		action->m_halfScreenSize = g2d_mult(action->m_fullScreenSize, .5f);
		
		action->m_leftBoundary = -((rect.origin.x+rect.size.width) - action->m_fullScreenSize.x);
		action->m_rightBoundary = -rect.origin.x ;
		action->m_topBoundary = -rect.origin.y;
		action->m_bottomBoundary = -((rect.origin.y+rect.size.height) - action->m_fullScreenSize.y);
		
		if(action->m_rightBoundary < action->m_leftBoundary){
			// screen width is larger than world's boundary width
			//set both in the middle of the world
			action->m_rightBoundary = action->m_leftBoundary = (action->m_leftBoundary + action->m_rightBoundary) / 2;
		}
		
		if(action->m_topBoundary < action->m_bottomBoundary){
			// screen width is larger than world's boundary width
			//set both in the middle of the world
			action->m_topBoundary = action->m_bottomBoundary = (action->m_topBoundary + action->m_bottomBoundary) / 2;
		}
		
		if((action->m_topBoundary == action->m_bottomBoundary) && (action->m_leftBoundary == action->m_rightBoundary))
			action->m_boundaryFullyCovered = true;
		
		return adoptRef(action);
	}
	
	void FollowAction::step(TimeSec dt)
	{
		UNUSED_PARAM(dt);
		
		if(m_boundarySet) {
			// whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
			if(m_boundaryFullyCovered)
				return;
			
			Point tempPos = g2d_sub( m_halfScreenSize, m_followedNode->position());
			m_target->setPosition(PointMake(clampf(tempPos.x, m_leftBoundary, m_rightBoundary), clampf(tempPos.y, m_bottomBoundary, m_topBoundary)));
		}
		else
			m_target->setPosition(g2d_sub(m_halfScreenSize, m_followedNode->position()));
	}
	
	bool FollowAction::isDone()
	{
		return !m_followedNode->isRunning();
	}
	
	void FollowAction::stop()
	{
		m_target = 0;
		Base::stop();
	}
	
	FollowAction* FollowAction::doCreateClone()
	{
		if (m_boundarySet)
			return Self::create(m_followedNode, m_worldBoundary).releaseRef();
		else
			return Self::create(m_followedNode).releaseRef();
	}
	
} //namespace Aphid
