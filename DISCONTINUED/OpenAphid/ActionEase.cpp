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

#include "ActionEase.h"
#include "AJOABinding.h"

#include "AJEaseAction.h"

namespace Aphid {
  ///-------------------------------------------------------------------------------------------------------------------
  /// EaseAction
  EaseAction::EaseAction(PassRefPtr<IntervalAction> action)
  : m_other(action)
  , Base(action.get()->duration())
  {
	  LEAK_DETECT_INC("EaseAction");
  }
	
  EaseAction::~EaseAction()
  {
	  LEAK_DETECT_DEC("EaseAction");
  }
	
	void EaseAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_other->startWithTarget(target);
	}
	
	void EaseAction::stop()
	{
		m_other->stop();
		Base::stop();
	}
	
	void EaseAction::update(TimeSec t)
	{
		m_other->handleUpdate(t);
	}
	
	const AJ::ClassInfo* EaseAction::wrapperClassInfo() const
	{
		return &AJEaseAction::s_info;
	}
	
	AJ::AJObject* EaseAction::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJEaseAction>(exec, globalObject, this);
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// EaseRateAction
  EaseRateAction::EaseRateAction(PassRefPtr<IntervalAction> action, float rate)
	: Base(action)
	, m_rate(rate)
  {
	  LEAK_DETECT_INC("EaseRateAction");
  }
	
  EaseRateAction::~EaseRateAction()
  {
	  LEAK_DETECT_DEC("EaseRateAction");
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// EaseInAction
  EaseInAction::EaseInAction(PassRefPtr<IntervalAction> action, float rate)
  : Base(action, rate)
  {
	  LEAK_DETECT_INC("EaseInAction");
  }
	
  EaseInAction::~EaseInAction()
  {
	  LEAK_DETECT_DEC("EaseInAction");
  }
	
	void EaseInAction::update(TimeSec t)
	{
		m_other->handleUpdate(powf(t, m_rate));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseOutAction
  EaseOutAction::EaseOutAction(PassRefPtr<IntervalAction> action, float rate)
  : Base(action, rate)
  {
	  LEAK_DETECT_INC("EaseOutAction");
  }
	
  EaseOutAction::~EaseOutAction()
  {
	  LEAK_DETECT_DEC("EaseOutAction");
  }
	
	void EaseOutAction::update(TimeSec t)
	{
		m_other->handleUpdate(powf(t, 1.0f/m_rate));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseInOutAction
  EaseInOutAction::EaseInOutAction(PassRefPtr<IntervalAction> action, float rate)
  : Base(action, rate)
  {
	  LEAK_DETECT_INC("EaseInOutAction");
  }
	
  EaseInOutAction::~EaseInOutAction()
  {
	  LEAK_DETECT_DEC("EaseInOutAction");
  }
	
	void EaseInOutAction::update(TimeSec t)
	{
		int sign =1;
		int r = (int) m_rate;
		if (r % 2 == 0)
			sign = -1;
		t *= 2;
		if (t < 1) 
			m_other->handleUpdate(0.5f * powf (t, m_rate));
		else
			m_other->handleUpdate(sign * 0.5f * (powf(t - 2, m_rate) + sign*2));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseExponentialInAction
  EaseExponentialInAction::EaseExponentialInAction(PassRefPtr<IntervalAction> action)
  : Base(action)
  {
	  LEAK_DETECT_INC("EaseExponentialInAction");
  }
	
  EaseExponentialInAction::~EaseExponentialInAction()
  {
	  LEAK_DETECT_DEC("EaseExponentialInAction");
  }
	
	void EaseExponentialInAction::update(TimeSec t)
	{
		m_other->handleUpdate((t==0) ? 0 : powf(2, 10 * (t/1 - 1)) - 1 * 0.001f);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseExponentialOutAction
  EaseExponentialOutAction::EaseExponentialOutAction(PassRefPtr<IntervalAction> action)
  : Base(action)
  {
	  LEAK_DETECT_INC("EaseExponentialOutAction");
  }
	
  EaseExponentialOutAction::~EaseExponentialOutAction()
  {
	  LEAK_DETECT_DEC("EaseExponentialOutAction");
  }
	
	void EaseExponentialOutAction::update(TimeSec t)
	{
		m_other->handleUpdate((t==1) ? 1 : (-powf(2, -10 * t/1) + 1));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseExponentialInOutAction
  EaseExponentialInOutAction::EaseExponentialInOutAction(PassRefPtr<IntervalAction> action)
  : Base(action)
  {
	  LEAK_DETECT_INC("EaseExponentialInOutAction");
  }
	
  EaseExponentialInOutAction::~EaseExponentialInOutAction()
  {
	  LEAK_DETECT_DEC("EaseExponentialInOutAction");
  }
	
	void EaseExponentialInOutAction::update(TimeSec t)
	{
		t /= 0.5f;
		if (t < 1)
			t = 0.5f * powf(2, 10 * (t - 1));
		else
			t = 0.5f * (-powf(2, -10 * (t -1) ) + 2);
		
		m_other->handleUpdate(t);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseSineInAction
  EaseSineInAction::EaseSineInAction(PassRefPtr<IntervalAction> action)
  : Base(action)
  {
	  LEAK_DETECT_INC("EaseSineInAction");
  }
	
  EaseSineInAction::~EaseSineInAction()
  {
	  LEAK_DETECT_DEC("EaseSineInAction");
  }
	
	void EaseSineInAction::update(TimeSec t)
	{
		m_other->handleUpdate(-1 * cosf(t * (float)M_PI_2) + 1);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseSineOutAction
  EaseSineOutAction::EaseSineOutAction(PassRefPtr<IntervalAction> action)
  : Base(action)
  {
	  LEAK_DETECT_INC("EaseSineOutAction");
  }
	
  EaseSineOutAction::~EaseSineOutAction()
  {
	  LEAK_DETECT_DEC("EaseSineOutAction");
  }
	
	void EaseSineOutAction::update(TimeSec t)
	{
		m_other->handleUpdate(sinf(t * (float)M_PI_2));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
  /// EaseSineInOutAction
  EaseSineInOutAction::EaseSineInOutAction(PassRefPtr<IntervalAction> action)
  : Base(action)
  {
	  LEAK_DETECT_INC("EaseSineInOutAction");
  }
	
  EaseSineInOutAction::~EaseSineInOutAction()
  {
	  LEAK_DETECT_DEC("EaseSineInOutAction");
  }
	
	void EaseSineInOutAction::update(TimeSec t)
	{
		m_other->handleUpdate(-0.5f * (cosf((float)M_PI * t) - 1));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseElasticAction
	EaseElasticAction::EaseElasticAction(PassRefPtr<IntervalAction> action, float period)
	: Base(action)
	, m_period(period)
	{
		LEAK_DETECT_INC("EaseElasticAction");
	}
	
	EaseElasticAction::~EaseElasticAction()
	{
		LEAK_DETECT_DEC("EaseElasticAction");
	}
		
#ifndef M_PI_X_2
#define M_PI_X_2 (float)M_PI * 2.0f
#endif
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseElasticInAction
	EaseElasticInAction::EaseElasticInAction(PassRefPtr<IntervalAction> action, float period)
	: Base(action, period)
	{
		LEAK_DETECT_INC("EaseElasticInAction");
	}
	
	EaseElasticInAction::~EaseElasticInAction()
	{
		LEAK_DETECT_DEC("EaseElasticInAction");
	}
	
	void EaseElasticInAction::update(TimeSec t)
	{
		TimeSec newT = 0;
		if (t == 0 || t == 1)
			newT = t;
		
		else {
			float s = m_period / 4;
			t = t - 1;
			newT = -powf(2, 10 * t) * sinf((t - s) * M_PI_X_2 / m_period);
		}
		m_other->handleUpdate(newT);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseElasticOutAction
	EaseElasticOutAction::EaseElasticOutAction(PassRefPtr<IntervalAction> action, float period)
	: Base(action, period)
	{
		LEAK_DETECT_INC("EaseElasticOutAction");
	}
	
	EaseElasticOutAction::~EaseElasticOutAction()
	{
		LEAK_DETECT_DEC("EaseElasticOutAction");
	}
	
	void EaseElasticOutAction::update(TimeSec t)
	{
		TimeSec newT = 0;
		if (t == 0 || t == 1) {
			newT = t;
			
		} else {
			float s = m_period / 4;
			newT = powf(2, -10 * t) * sinf((t - s) * M_PI_X_2 / m_period) + 1;
		}
		m_other->handleUpdate(newT);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseElasticInOutAction
	EaseElasticInOutAction::EaseElasticInOutAction(PassRefPtr<IntervalAction> action, float period)
	: Base(action, period)
	{
		LEAK_DETECT_INC("EaseElasticInOutAction");
	}
	
	EaseElasticInOutAction::~EaseElasticInOutAction()
	{
		LEAK_DETECT_DEC("EaseElasticInOutAction");
	}
	
	void EaseElasticInOutAction::update(TimeSec t)
	{
		TimeSec newT = 0;
		
		if( t == 0 || t == 1 )
			newT = t;
		else {
			t = t * 2;
			if(!m_period ) //TODO: why check this?
				m_period = 0.3f * 1.5f;
			TimeSec s = m_period / 4;
			
			t = t -1;
			if( t < 0 )
				newT = -0.5f * powf(2, 10 * t) * sinf((t - s) * M_PI_X_2 / m_period);
			else
				newT = powf(2, -10 * t) * sinf((t - s) * M_PI_X_2 / m_period) * 0.5f + 1;
		}
		
		m_other->handleUpdate(newT);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBounceAction
	EaseBounceAction::EaseBounceAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBounceAction");
	}
	
	EaseBounceAction::~EaseBounceAction()
	{
		LEAK_DETECT_DEC("EaseBounceAction");
	}
	
	TimeSec EaseBounceAction::bounceTime(TimeSec t)
	{
		if (t < 1 / 2.75) 
			return 7.5625f * t * t;
		else if (t < 2 / 2.75) {
			t -= 1.5f / 2.75f;
			return 7.5625f * t * t + 0.75f;
		}
		else if (t < 2.5 / 2.75) {
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		}
		
		t -= 2.625f / 2.75f;
		return 7.5625f * t * t + 0.984375f;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBounceInAction
	EaseBounceInAction::EaseBounceInAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBounceInAction");
	}
	
	EaseBounceInAction::~EaseBounceInAction()
	{
		LEAK_DETECT_DEC("EaseBounceInAction");
	}
	
	void EaseBounceInAction::update(TimeSec t)
	{
		TimeSec newT = 1 - bounceTime(1-t);
		m_other->handleUpdate(newT);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBounceOutAction
	EaseBounceOutAction::EaseBounceOutAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBounceOutAction");
	}
	
	EaseBounceOutAction::~EaseBounceOutAction()
	{
		LEAK_DETECT_DEC("EaseBounceOutAction");
	}
	
	void EaseBounceOutAction::update(TimeSec t)
	{
		TimeSec newT = 1 - bounceTime(t);
		m_other->handleUpdate(newT);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBounceInOutAction
	EaseBounceInOutAction::EaseBounceInOutAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBounceInOutAction");
	}
	
	EaseBounceInOutAction::~EaseBounceInOutAction()
	{
		LEAK_DETECT_DEC("EaseBounceInOutAction");
	}
	
	void EaseBounceInOutAction::update(TimeSec t)
	{
		TimeSec newT = 0;
		if (t < 0.5) {
			t = t * 2;
			newT = (1 - bounceTime(1-t)) * 0.5f;
		} else
			newT = bounceTime(t * 2 - 1) * 0.5f + 0.5f;
		
		m_other->handleUpdate(newT);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBackInAction
	EaseBackInAction::EaseBackInAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBackInAction");
	}
	
	EaseBackInAction::~EaseBackInAction()
	{
		LEAK_DETECT_DEC("EaseBackInAction");
	}
	
	void EaseBackInAction::update(TimeSec t)
	{
		TimeSec overshoot = 1.70158f;
		m_other->handleUpdate(t * t * ((overshoot + 1) * t - overshoot));
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBackOutAction
	EaseBackOutAction::EaseBackOutAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBackOutAction");
	}
	
	EaseBackOutAction::~EaseBackOutAction()
	{
		LEAK_DETECT_DEC("EaseBackOutAction");
	}
	
	void EaseBackOutAction::update(TimeSec t)
	{
		TimeSec overshoot = 1.70158f;
		
		t = t - 1;
		m_other->handleUpdate(t * t * ((overshoot + 1) * t + overshoot) + 1);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EaseBackInOutAction
	EaseBackInOutAction::EaseBackInOutAction(ATF::PassRefPtr<IntervalAction> action)
	: Base(action)
	{
		LEAK_DETECT_INC("EaseBackInOutAction");
	}
	
	EaseBackInOutAction::~EaseBackInOutAction()
	{
		LEAK_DETECT_DEC("EaseBackInOutAction");
	}
	
	void EaseBackInOutAction::update(TimeSec t)
	{
		TimeSec overshoot = 1.70158f * 1.525f;
		
		t = t * 2;
		if (t < 1)
			m_other->handleUpdate((t * t * ((overshoot + 1) * t - overshoot)) / 2);
		else {
			t = t - 2;
			m_other->handleUpdate((t * t * ((overshoot + 1) * t + overshoot)) / 2 + 1);
		}
	}
	
}