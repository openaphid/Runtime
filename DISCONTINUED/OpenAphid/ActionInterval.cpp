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

#include "ActionInterval.h"

#include "Node.h"

#include "G2DMacros.h"

#include "Sprite.h"

#include "AJAnimation.h"
#include "AJSpriteFrame.h"

#include <algorithm>

namespace Aphid {	
	///-------------------------------------------------------------------------------------------------------------------
	SequenceAction::SequenceAction(const FiniteTimeActionVector& actions)
	: Base(sumDuration(actions))
	, m_split(0.0f)
	, m_lastSplit(0.0f)
	, m_index(-1)
	{
		LEAK_DETECT_INC("SequenceAction");
		
		m_actions.append(actions);
	}
	
	SequenceAction::~SequenceAction()
	{
		LEAK_DETECT_DEC("SequenceAction");
	}
	
	void SequenceAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_index = 0;
		m_lastSplit = 0.0f;
		m_split = sumDuration(m_actions, m_index + 1) / m_duration;
		m_actions.at(m_index)->startWithTarget(target);
	}
	
	void SequenceAction::stop()
	{
		for (FiniteTimeActionVector::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
			it->get()->stop();
		Base::stop();
	}
	
	TimeSec SequenceAction::sumDuration(const FiniteTimeActionVector &actions, int to)
	{
		int index = std::min(actions.size(), to < 0 ? actions.size() : (size_t)to);
		TimeSec sum = 0;
		for (int i = 0; i < index; i++)
			sum += actions.at(i)->duration();
		return sum;
	}
	
	void SequenceAction::update(TimeSec t)
	{
		while (t >= m_split && m_index < m_actions.size()) {
			dispatchUpdateToAction(m_actions.at(m_index).get(), 1.0f);
			m_actions.at(m_index)->stop();
			m_index++;
			m_lastSplit = m_split;
			if (m_index < m_actions.size()) {
				m_split = sumDuration(m_actions, m_index + 1) / m_duration;
				m_actions.at(m_index)->startWithTarget(m_target);
			}
		}
		
		if (m_index < m_actions.size()) {
			dispatchUpdateToAction(m_actions.at(m_index).get(), (t - m_lastSplit) / (m_split - m_lastSplit));
		}
	}
	
	void SequenceAction::becomeManaged()
	{
		Base::becomeManaged();
		
		for (FiniteTimeActionVector::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
			it->get()->becomeManaged();
	}
	
	void SequenceAction::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		
		Base::markObjects(markStack, markID);
		for (FiniteTimeActionVector::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
			it->get()->markObjects(markStack, markID);
	}
	
	SequenceAction* SequenceAction::doCreateClone()
	{
		FiniteTimeActionVector actions;
		for (FiniteTimeActionVector::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
			actions.append(adoptRef(it->get()->newClone()));
		return new SequenceAction(actions);
	}
	
	SequenceAction* SequenceAction::doCreateReverse()
	{
		FiniteTimeActionVector actions;
		size_t size = m_actions.size();
		for (int i = size - 1; i >= 0; i--)
			actions.append(adoptRef(m_actions.at(i)->newReverse()));
		return new SequenceAction(actions);
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// RepeatAction
  RepeatAction::RepeatAction(PassRefPtr<FiniteTimeAction> action, size_t times)
	: Base(action.get()->duration() * times)
	, m_times(times)
  , m_total(0)
  , m_innerAction(action)
  {
	  LEAK_DETECT_INC("RepeatAction");
  }
	
  RepeatAction::~RepeatAction()
  {
	  LEAK_DETECT_DEC("RepeatAction");
  }
	
	void RepeatAction::startWithTarget(Node *target)
	{
		m_total = 0;
		Base::startWithTarget(target);
		m_innerAction->startWithTarget(target);
	}
	
	void RepeatAction::stop()
	{
		m_innerAction->stop();
		Base::stop();
	}
	
	void RepeatAction::update(TimeSec dt)
	{
		TimeSec t = dt * m_times;
		if(t > m_total + 1) {
			m_innerAction->handleUpdate(1.0f);
			m_total++;
			m_innerAction->stop();
			m_innerAction->startWithTarget(m_target);
			
			// repeat is over ?
			if(m_total == m_times)
				// so, set it in the original position
				m_innerAction->handleUpdate(0);
			else {
				// no ? start next repeat with the right update
				// to prevent jerk (issue #390)
				m_innerAction->handleUpdate(t - m_total);
			}
			
		} else {
			
			float r = fmodf(t, 1.0f);
			
			// fix last repeat position
			// else it could be 0.
			if( dt== 1.0f) {
				r = 1.0f;
				m_total++; // this is the added line
			}
			m_innerAction->handleUpdate(std::min(r,1.0f));
		}
	}
	
	bool RepeatAction::isDone()
	{
		return m_total == m_times;
	}
	
	RepeatAction* RepeatAction::doCreateClone()
	{
		return new RepeatAction(adoptRef(m_innerAction->newClone()), m_times);
	}
	
	RepeatAction* RepeatAction::doCreateReverse()
	{
		return new RepeatAction(adoptRef(m_innerAction->newReverse()), m_times);
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// SpawnAction
  SpawnAction::SpawnAction(PassRefPtr<FiniteTimeAction> one, PassRefPtr<FiniteTimeAction> two)
	: Base(one.get()->duration() + two.get()->duration())
	, m_action0(one)
	, m_action1(two)
  {
	  LEAK_DETECT_INC("SpawnAction");
  }
	
  SpawnAction::~SpawnAction()
  {
	  LEAK_DETECT_DEC("SpawnAction");
  }
	
	PassRefPtr<SpawnAction> SpawnAction::create(const FiniteTimeActionVector& actions) {
		if (actions.size() < 2) {
			oa_debug("create with invalid vector size: %d?", actions.size());
			return 0;
		}
		
		RefPtr<FiniteTimeAction> prev = actions.at(0);
		for(size_t i = 1; i < actions.size(); i++)
			prev = SpawnAction::create(prev, actions.at(i));
		
		return toActionRef<SpawnAction>(prev);
	}
	
	void SpawnAction::setTwo(PassRefPtr<FiniteTimeAction> prpAction0, PassRefPtr<FiniteTimeAction> prpAction1)
	{
		RefPtr<FiniteTimeAction> action0 = prpAction0;
		RefPtr<FiniteTimeAction> action1 = prpAction1;
		
		ASSERT(action0 && action1);
		ASSERT(action0 != m_action0 && action1 != m_action1);
		ASSERT(action1 != m_action0 && action0 != m_action1);
		
		m_action0 = action0;
		m_action1 = action1;
		
		TimeSec d1 = action0->duration();
		TimeSec d2 = action1->duration();
		
		m_duration = std::max(d1, d2);
		if( d1 > d2 ) {
			FiniteTimeActionVector actions;
			actions.append(action1);
			actions.append(DelayTimeAction::create(d1 - d2));
			m_action1 = SequenceAction::create(actions);
		}
		else if( d1 < d2) {
			FiniteTimeActionVector actions;
			actions.append(action0);
			actions.append(DelayTimeAction::create(d2 - d1));
			m_action0 = SequenceAction::create(actions);
		}
		//TODO: support re-init as CC??
	}
	
	void SpawnAction::startWithTarget(Node *target)
	{
		Base::startWithTarget(target);
		m_action0->startWithTarget(target);
		m_action1->startWithTarget(target);
	}
	
	void SpawnAction::stop()
	{
		m_action0->stop();
		m_action1->stop();
		Base::stop();
	}
	
	void SpawnAction::update(TimeSec time)
	{
		m_action0->handleUpdate(time);
		m_action1->handleUpdate(time);
	}
	
	SpawnAction* SpawnAction::doCreateClone()
	{
		return new SpawnAction(adoptRef(m_action0->newClone()), adoptRef(m_action1->newClone()));
	}
	
	SpawnAction* SpawnAction::doCreateReverse()
	{
		return new SpawnAction(adoptRef(m_action0->newReverse()), adoptRef(m_action1->newReverse()));
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// RotateToAction
  RotateToAction::RotateToAction(TimeSec duration, float angle)
	: Base(duration)
	, m_dstAngle(angle)
  {	  
	  LEAK_DETECT_INC("RotateToAction");
  }
	
  RotateToAction::~RotateToAction()
  {
	  LEAK_DETECT_DEC("RotateToAction");
  }
	
	void RotateToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		
		m_startAngle = m_target->rotation();
		if (m_startAngle > 0)
			m_startAngle = fmodf(m_startAngle, 360.0f);
		else
			m_startAngle = fmodf(m_startAngle, -360.0f);
		
		m_diffAngle = m_dstAngle - m_startAngle;
		if (m_diffAngle > 180)
			m_diffAngle -= 360;
		if (m_diffAngle < -180)
			m_diffAngle += 360;
	}
	
	void RotateToAction::update(TimeSec t)
	{
		m_target->setRotation(m_startAngle + m_diffAngle * t);
	}
	
	RotateToAction* RotateToAction::doCreateClone()
	{
		return new RotateToAction(m_duration, m_dstAngle);
	}
	
	RotateToAction* RotateToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// RotateByAction
  RotateByAction::RotateByAction(TimeSec duration, float angle)
	: Base(duration)
	, m_angle(angle)
  {	  
	  LEAK_DETECT_INC("RotateByAction");
  }
	
  RotateByAction::~RotateByAction()
  {
	  LEAK_DETECT_DEC("RotateByAction");
  }
	
	void RotateByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startAngle = m_target->rotation();
	}
	
	void RotateByAction::update(TimeSec dt)
	{
		//TODO: add % 360??
		m_target->setRotation(m_startAngle + m_angle * dt);
	}
	
	RotateByAction* RotateByAction::doCreateClone()
	{
		return new RotateByAction(m_duration, m_angle);
	}
	
	RotateByAction* RotateByAction::doCreateReverse()
	{
		return new RotateByAction(m_duration, -m_angle);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// MoveToAction
	MoveToAction::MoveToAction(TimeSec duration, const Point& p)
	: Base(duration)
	, m_endPosition(p)
	{
		LEAK_DETECT_INC("MoveToAction");
	}
	
	MoveToAction::~MoveToAction()
	{
		LEAK_DETECT_DEC("MoveToAction");
	}
	
	void MoveToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startPosition = m_target->position();
		m_delta = g2d_sub(m_endPosition, m_startPosition);
	}
	
	void MoveToAction::update(TimeSec t)
	{
		m_target->setPosition(PointMake(m_startPosition.x + m_delta.x * t, m_startPosition.y + m_delta.y * t));
	}
	
	MoveToAction* MoveToAction::doCreateClone()
	{
		return new MoveToAction(m_duration, m_endPosition);
	}
	
	MoveToAction* MoveToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// MoveByAction
	MoveByAction::MoveByAction(TimeSec duration, const Point& p)
	: Base(duration)
	, m_delta(p)
	{
		LEAK_DETECT_INC("MoveByAction");
	}
	
	MoveByAction::~MoveByAction()
	{
		LEAK_DETECT_DEC("MoveByAction");
	}
	
	void MoveByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startPosition = m_target->position();
	}
	
	void MoveByAction::update(TimeSec t)
	{
		m_target->setPosition(PointMake(m_startPosition.x + m_delta.x * t, m_startPosition.y + m_delta.y * t));
	}
	
	MoveByAction* MoveByAction::doCreateClone()
	{
		return new MoveByAction(m_duration, m_delta);
	}
	
	MoveByAction* MoveByAction::doCreateReverse()
	{
		return new MoveByAction(m_duration, g2d_sub(PointZero, m_delta));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// SkewToAction
	SkewToAction::SkewToAction(TimeSec duration, float skewX, float skewY)
	: Base(duration)
	{
		LEAK_DETECT_INC("SkewToAction");
		m_endSkew = Vector2Make(skewX, skewY);
	}
	
	SkewToAction::~SkewToAction()
	{
		LEAK_DETECT_DEC("SkewToAction");
	}
	
	void SkewToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		
		m_startSkew = m_target->skew();		
		
		if (m_startSkew.x > 0)
			m_startSkew.x = fmodf(m_startSkew.x, 180.0f);
		else
			m_startSkew.x = fmodf(m_startSkew.x, -180.0f);
		
		m_delta.x = m_endSkew.x - m_startSkew.x;
		
		if ( m_delta.x > 180 )
			m_delta.x -= 360;
		
		if ( m_delta.x < -180 )
			m_delta.x += 360;
		
		if (m_startSkew.y > 0)
			m_startSkew.y = fmodf(m_startSkew.y, 360.0f);
		else
			m_startSkew.y = fmodf(m_startSkew.y, -360.0f);
		
		m_delta.y = m_endSkew.y - m_startSkew.y;
		
		if ( m_delta.y > 180 )
			m_delta.y -= 360;
		
		if ( m_delta.y < -180 )
			m_delta.y += 360;
	}
	
	void SkewToAction::update(TimeSec dt)
	{
		m_target->setSkew(m_startSkew.x + m_delta.x * dt, m_startSkew.y + m_delta.y * dt);
	}
	
	SkewToAction* SkewToAction::doCreateClone()
	{
		return new SkewToAction(m_duration, m_endSkew.x, m_endSkew.y);
	}
	
	SkewToAction* SkewToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// SkewByAction
	SkewByAction::SkewByAction(TimeSec duration, float skewX, float skewY)
	: Base(duration)
	{
		LEAK_DETECT_INC("SkewByAction");
		m_delta = Vector2Make(skewX, skewY);
	}
	
	SkewByAction::~SkewByAction()
	{
		LEAK_DETECT_DEC("SkewByAction");
	}
	
	void SkewByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		
		m_startSkew = m_target->skew();		
		
		if (m_startSkew.x > 0)
			m_startSkew.x = fmodf(m_startSkew.x, 180.0f);
		else
			m_startSkew.x = fmodf(m_startSkew.x, -180.0f);
		
		if (m_startSkew.y > 0)
			m_startSkew.y = fmodf(m_startSkew.y, 360.0f);
		else
			m_startSkew.y = fmodf(m_startSkew.y, -360.0f);
	}
	
	void SkewByAction::update(TimeSec dt)
	{
		m_target->setSkew(m_startSkew.x + m_delta.x * dt, m_startSkew.y + m_delta.y * dt);
	}
	
	SkewByAction* SkewByAction::doCreateClone()
	{
		return new SkewByAction(m_duration, m_delta.x, m_delta.y);
	}
	
	SkewByAction* SkewByAction::doCreateReverse()
	{
		return new SkewByAction(m_duration, -m_delta.x, -m_delta.y);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// JumpByAction
	JumpByAction::JumpByAction(TimeSec duration, const Point& position, TimeSec height, size_t jumps)
	: Base(duration)
	, m_delta(position)
	, m_height(height)
	, m_jumps(jumps)
	{
		LEAK_DETECT_INC("JumpByAction");
	}
	
	JumpByAction::~JumpByAction()
	{
		LEAK_DETECT_DEC("JumpByAction");
	}
	
	void JumpByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startPosition = m_target->position();
	}
	
	void JumpByAction::update(TimeSec t)
	{
		TimeSec frac = fmodf( t * m_jumps, 1.0f );
		TimeSec y = m_height * 4 * frac * (1 - frac);
		y += m_delta.y * t;
		TimeSec x = m_delta.x * t;
		m_target->setPosition(PointMake(m_startPosition.x + x, m_startPosition.y + y));
	}
	
	JumpByAction* JumpByAction::doCreateClone()
	{
		return new JumpByAction(m_duration, m_delta, m_height, m_jumps);
	}
	
	JumpByAction* JumpByAction::doCreateReverse()
	{
		return new JumpByAction(m_duration, g2d_sub(PointZero, m_delta), m_height, m_jumps);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// JumpToAction
	JumpToAction::JumpToAction(TimeSec duration, const Point& position, TimeSec height, size_t jumps)
	: Base(duration)
	, m_endPosition(position)
	, m_height(height)
	, m_jumps(jumps)
	{
		LEAK_DETECT_INC("JumpToAction");
	}
	
	JumpToAction::~JumpToAction()
	{
		LEAK_DETECT_DEC("JumpToAction");
	}
	
	void JumpToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startPosition = m_target->position();
		m_delta = g2d_sub(m_endPosition, m_startPosition);
	}
	
	void JumpToAction::update(TimeSec t)
	{
		TimeSec frac = fmodf( t * m_jumps, 1.0f );
		TimeSec y = m_height * 4 * frac * (1 - frac);
		y += m_delta.y * t;
		TimeSec x = m_delta.x * t;
		m_target->setPosition(PointMake(m_startPosition.x + x, m_startPosition.y + y));
	}
	
	JumpToAction* JumpToAction::doCreateClone()
	{
		return new JumpToAction(m_duration, m_endPosition, m_height, m_jumps);
	}
	
	JumpToAction* JumpToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// BezierByAction
	BezierByAction::BezierByAction(TimeSec duration, const ccBezierConfig& c)
	: Base(duration)
	, m_config(c)
	{
		LEAK_DETECT_INC("BezierByAction");
	}
	
	BezierByAction::~BezierByAction()
	{
		LEAK_DETECT_DEC("BezierByAction");
	}
	
	static inline float bezierat(float a, float b, float c, float d, TimeSec t)
	{
		return (powf(1-t,3) * a + 
						3*t*(powf(1-t,2))*b + 
						3*powf(t,2)*(1-t)*c +
						powf(t,3)*d );
	}
	
	void BezierByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startPosition = m_target->position();
	}
	
	void BezierByAction::update(TimeSec t)
	{
		float xa = 0;
		float xb = m_config.controlPoint_1.x;
		float xc = m_config.controlPoint_2.x;
		float xd = m_config.endPosition.x;
		
		float ya = 0;
		float yb = m_config.controlPoint_1.y;
		float yc = m_config.controlPoint_2.y;
		float yd = m_config.endPosition.y;
		
		float x = bezierat(xa, xb, xc, xd, t);
		float y = bezierat(ya, yb, yc, yd, t);
		m_target->setPosition(g2d_add(m_startPosition, PointMake(x,y)));
	}
	
	BezierByAction* BezierByAction::doCreateClone()
	{
		return new BezierByAction(m_duration, m_config);
	}
	
	BezierByAction* BezierByAction::doCreateReverse()
	{
		ccBezierConfig r;
		
		r.endPosition	 = g2d_neg(m_config.endPosition);
		r.controlPoint_1 = g2d_add(m_config.controlPoint_2, g2d_neg(m_config.endPosition));
		r.controlPoint_2 = g2d_add(m_config.controlPoint_1, g2d_neg(m_config.endPosition));
		return new BezierByAction(m_duration, r);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// BezierToAction
	//TODO the impl of CC looks weird to me
	BezierToAction::BezierToAction(TimeSec duration, const ccBezierConfig& c)
	: Base(duration)
	, m_config(c)
	{
		LEAK_DETECT_INC("BezierToAction");
	}
	
	BezierToAction::~BezierToAction()
	{
		LEAK_DETECT_DEC("BezierToAction");
	}
	
	void BezierToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startPosition = m_target->position();
		
		m_config.controlPoint_1 = g2d_sub(m_config.controlPoint_1, m_startPosition);
		m_config.controlPoint_2 = g2d_sub(m_config.controlPoint_2, m_startPosition);
		m_config.endPosition = g2d_sub(m_config.endPosition, m_startPosition);
	}
	
	void BezierToAction::update(TimeSec t)
	{
		float xa = 0;
		float xb = m_config.controlPoint_1.x;
		float xc = m_config.controlPoint_2.x;
		float xd = m_config.endPosition.x;
		
		float ya = 0;
		float yb = m_config.controlPoint_1.y;
		float yc = m_config.controlPoint_2.y;
		float yd = m_config.endPosition.y;
		
		float x = bezierat(xa, xb, xc, xd, t);
		float y = bezierat(ya, yb, yc, yd, t);
		m_target->setPosition(g2d_add(m_startPosition, PointMake(x,y)));
	}
	
	BezierToAction* BezierToAction::doCreateClone()
	{
		return new BezierToAction(m_duration, m_config);
	}
	
	BezierToAction* BezierToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// ScaleToAction
	ScaleToAction::ScaleToAction(TimeSec duration, const Vector2& scale)
	: Base(duration)
	, m_endScale(scale)
	{
		LEAK_DETECT_INC("ScaleToAction");
	}
	
	ScaleToAction::~ScaleToAction()
	{
		LEAK_DETECT_DEC("ScaleToAction");
	}
	
	void ScaleToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startScale = m_target->scale();
		m_delta = m_endScale - m_startScale;
	}
	
	void ScaleToAction::update(TimeSec t)
	{
		m_target->setScaleX(m_startScale.x + m_delta.x * t);
		m_target->setScaleY(m_startScale.y + m_delta.y * t);
	}
	
	ScaleToAction* ScaleToAction::doCreateClone()
	{
		return new ScaleToAction(m_duration, m_endScale);
	}
	
	ScaleToAction* ScaleToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// ScaleByAction
	ScaleByAction::ScaleByAction(TimeSec duration, const Vector2& scale)
	: Base(duration)
	, m_scaleBy(scale)
	{
		LEAK_DETECT_INC("ScaleByAction");
	}
	
	ScaleByAction::~ScaleByAction()
	{
		LEAK_DETECT_DEC("ScaleByAction");
	}
	
	void ScaleByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_startScale = m_target->scale();
		m_delta.x = m_startScale.x * m_scaleBy.x - m_startScale.x;
		m_delta.y = m_startScale.y * m_scaleBy.y - m_startScale.y;
	}
	
	void ScaleByAction::update(TimeSec t)
	{
		m_target->setScaleX(m_startScale.x + m_delta.x * t);
		m_target->setScaleY(m_startScale.y + m_delta.y * t);
	}
	
	ScaleByAction* ScaleByAction::doCreateClone()
	{
		return new ScaleByAction(m_duration, m_scaleBy);
	}
	
	ScaleByAction* ScaleByAction::doCreateReverse()
	{
		return new ScaleByAction(m_duration, Vector2Make(1.0f / m_scaleBy.x, 1.0f / m_scaleBy.y));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// BlinkAction
	BlinkAction::BlinkAction(TimeSec duration, size_t times)
	: Base(duration)
	, m_times(times)
	{
		LEAK_DETECT_INC("BlinkAction");
	}
	
	BlinkAction::~BlinkAction()
	{
		LEAK_DETECT_DEC("BlinkAction");
	}
	
	void BlinkAction::update(TimeSec t)
	{
		if (!isDone()) {
			TimeSec slice = 1.0f / m_times;
			TimeSec m = fmodf(t, slice);
			m_target->setVisible(m>slice/2);
		}
	}
	
	BlinkAction* BlinkAction::doCreateClone()
	{
		return new BlinkAction(m_duration, m_times);
	}
	
	BlinkAction* BlinkAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeInAction
  FadeInAction::FadeInAction(TimeSec duration)
  : Base(duration)
  {
	  LEAK_DETECT_INC("FadeInAction");
  }
	
  FadeInAction::~FadeInAction()
  {
	  LEAK_DETECT_DEC("FadeInAction");
  }
	
	void FadeInAction::update(TimeSec time)
	{
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			casted->setOpacity(255 * time);
	}
	
	FadeInAction* FadeInAction::doCreateClone()
	{
		return new FadeInAction(m_duration);
	}
	
	IntervalAction* FadeInAction::doCreateReverse()
	{
		return FadeOutAction::create(m_duration).releaseRef();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeOutAction
  FadeOutAction::FadeOutAction(TimeSec duration)
  : Base(duration)
  {
	  LEAK_DETECT_INC("FadeOutAction");
  }
	
  FadeOutAction::~FadeOutAction()
  {
	  LEAK_DETECT_DEC("FadeOutAction");
  }
	
	void FadeOutAction::update(TimeSec t)
	{
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			casted->setOpacity(255 * (1 - t));
	}
	
	FadeOutAction* FadeOutAction::doCreateClone()
	{
		return new FadeOutAction(m_duration);
	}
	
	IntervalAction* FadeOutAction::doCreateReverse()
	{
		return FadeInAction::create(m_duration).releaseRef();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeToAction
  FadeToAction::FadeToAction(TimeSec duration, GLubyte o)
  : Base(duration)
  , m_toOpacity(o)
  {
	  LEAK_DETECT_INC("FadeToAction");
  }
	
  FadeToAction::~FadeToAction()
  {
	  LEAK_DETECT_DEC("FadeToAction");
  }
	
	void FadeToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			m_fromOpacity = casted->opacity();
	}
	
	void FadeToAction::update(TimeSec time)
	{
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			casted->setOpacity(m_fromOpacity + (m_toOpacity - m_fromOpacity) * time);
	}
	
	FadeToAction* FadeToAction::doCreateClone()
	{
		return new FadeToAction(m_duration, m_toOpacity);
	}
	
	FadeToAction* FadeToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// TintToAction
  TintToAction::TintToAction(TimeSec duration, GLubyte r, GLubyte g, GLubyte b)
  : Base(duration)
	, m_to(ccc3(r, g, b))
  {
	  LEAK_DETECT_INC("TintToAction");
  }
	
  TintToAction::~TintToAction()
  {
	  LEAK_DETECT_DEC("TintToAction");
  }
	
	void TintToAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			m_from = casted->color();
	}
	
	void TintToAction::update(TimeSec t)
	{
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			casted->setColor(ccc3(m_from.r + (m_to.r - m_from.r) * t, m_from.g + (m_to.g - m_from.g) * t, m_from.b + (m_to.b - m_from.b) * t));
	}
	
	TintToAction* TintToAction::doCreateClone()
	{
		return new TintToAction(m_duration, m_to.r, m_to.g, m_to.b);
	}
	
	TintToAction* TintToAction::doCreateReverse()
	{
		return doCreateClone();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// TintByAction
  TintByAction::TintByAction(TimeSec duration, GLshort r, GLshort g, GLshort b)
  : Base(duration)
	, m_deltaR(r)
	, m_deltaG(g)
	, m_deltaB(b)
	, m_fromR(255)
	, m_fromG(255)
	, m_fromB(255)
  {
	  LEAK_DETECT_INC("TintByAction");
  }
	
  TintByAction::~TintByAction()
  {
	  LEAK_DETECT_DEC("TintByAction");
  }
	
	void TintByAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted) {
			ccColor3B color = casted->color();
			m_fromR = color.r;
			m_fromG = color.g;
			m_fromB = color.b;
		}
	}
	
	void TintByAction::update(TimeSec t)
	{
		RGBAProtocol* casted = m_target->toRGBAProtocol();
		if (casted)
			casted->setColor(ccc3(m_fromR + m_deltaR * t, m_fromG + m_deltaG * t, m_fromB + m_deltaB * t));
	}
	
	TintByAction* TintByAction::doCreateClone()
	{
		return new TintByAction(m_duration, m_deltaR, m_deltaG, m_deltaB);
	}
	
	TintByAction* TintByAction::doCreateReverse()
	{
		return new TintByAction(m_duration, -m_deltaR, -m_deltaG, -m_deltaB);
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// DelayTimeAction
  DelayTimeAction::DelayTimeAction(TimeSec duration)
  : Base(duration)
  {
	  LEAK_DETECT_INC("DelayTimeAction");
  }
	
  DelayTimeAction::~DelayTimeAction()
  {
	  LEAK_DETECT_DEC("DelayTimeAction");
  }
	
  DelayTimeAction* DelayTimeAction::doCreateClone()
  {
  	return new DelayTimeAction(m_duration);
  }
	
  DelayTimeAction* DelayTimeAction::doCreateReverse()
  {
  	return doCreateClone();
  }
	
	///-------------------------------------------------------------------------------------------------------------------
	/// AnimateAction
	AnimateAction::AnimateAction(TimeSec duration, ATF::PassRefPtr<Animation> animation, bool restoreOriginalFrame)
	: Base(duration)
	, m_restoreOriginalFrame(restoreOriginalFrame)
	, m_animation(animation)
	, m_originalFrame(0)
	{
		LEAK_DETECT_INC("AnimateAction");
	}
	
	AnimateAction::~AnimateAction()
	{
		LEAK_DETECT_DEC("AnimateAction");
	}
	
	ATF::PassRefPtr<AnimateAction> AnimateAction::create(ATF::PassRefPtr<Animation> prpAnimation, bool restoreOriginalFrame) 
	{
		RefPtr<Animation> animation = prpAnimation;
		
		return create(animation->frames().size() * animation->delay(), animation, restoreOriginalFrame);
	}
	
	void AnimateAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		Sprite* sprite = m_target->toSprite();
		if (sprite) {
			m_originalFrame = 0;
			if (m_restoreOriginalFrame)
				m_originalFrame = sprite->displayedFrame();
		} else
			oa_error("m_target is not a Sprite");
	}
	
	void AnimateAction::stop()
	{
		if (m_restoreOriginalFrame) {
			Sprite* sprite = m_target->toSprite();
			if (sprite) 
				sprite->setDisplayFrame(m_originalFrame);
		}
		Base::stop();
	}
	
	void AnimateAction::update(TimeSec t)
	{
		const SpriteFrameVector& frames = m_animation->frames();
		size_t numberOfFrames = frames.size();
		
		size_t idx = t * numberOfFrames;
		
		if( idx >= numberOfFrames )
			idx = numberOfFrames -1;
		
		Sprite* sprite = m_target->toSprite();
		if (sprite) {
			if (!sprite->isFrameDisplayed(frames.at(idx).get()))
				sprite->setDisplayFrame(frames.at(idx));
		}
	}
	
	AnimateAction* AnimateAction::doCreateClone()
	{
		return new AnimateAction(m_duration, m_animation, m_restoreOriginalFrame);
	}
	
	AnimateAction* AnimateAction::doCreateReverse()
	{
		const SpriteFrameVector& frames = m_animation->frames();
		
		RefPtr<Animation> animation = Animation::create();
		animation->setDelay(m_animation->delay());
		
		size_t size = frames.size();
		for (size_t i = 0; i < size; i++) 
			animation->addFrame(frames.at(size - i));
		
		return new AnimateAction(m_duration, animation, m_restoreOriginalFrame);
	}
	
	void AnimateAction::markObjects(AJ::MarkStack &markStack, unsigned markID)
	{
		if (isMarked(markID))
			return;
		
		Base::markObjects(markStack, markID);
		m_animation->markObjects(markStack, markID);
		if (m_originalFrame)
			m_originalFrame->markObjects(markStack, markID);
	}
	
}

