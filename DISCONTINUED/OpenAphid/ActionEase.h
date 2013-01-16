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

#ifndef OpenAphid_ActionEase_h
#define OpenAphid_ActionEase_h

#include "Action.h"

namespace Aphid {
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseAction : public IntervalAction {
		typedef IntervalAction Base;
	public:
		virtual ~EaseAction();
		
		virtual void startWithTarget(Node* target);
		virtual void stop();
		virtual void update(TimeSec t);

		virtual int baseType() const {return ActionTypeEase | Base::baseType();}
		virtual const char* name() const {return "EaseAction";}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		virtual void becomeManaged()
		{
			Base::becomeManaged();
			m_other->becomeManaged();
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID)
		{
			if (isMarked(markID))
				return;
			Base::markObjects(markStack, markID);
			m_other->markObjects(markStack, markID);
		}
		
	protected:
		EaseAction(PassRefPtr<IntervalAction> action);
		
		RefPtr<IntervalAction> m_other;
	}; //class EaseAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseRateAction : public EaseAction {
		typedef EaseAction Base;
	public:
		virtual ~EaseRateAction();
		
		virtual const char* name() const {return "EaseRateAction";}
		
	protected:
		EaseRateAction(PassRefPtr<IntervalAction> action, float rate);
		
		float m_rate;
	}; //class EaseRateAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseInAction : public EaseRateAction {
		typedef EaseRateAction Base;
		typedef EaseInAction Self;
	public:
		virtual ~EaseInAction();
		
		static PassRefPtr<EaseInAction> create(PassRefPtr<IntervalAction> action, float rate)
		{
			return adoptRef(new EaseInAction(action, rate));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseInAction";}
		
	protected:
		EaseInAction(PassRefPtr<IntervalAction> action, float rate);
		
		virtual Self* doCreateClone()
		{
			return new Self(ATF::adoptRef(m_other->newClone()), 1.0f/m_rate);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(ATF::adoptRef(m_other->newReverse()), 1.0f/m_rate);
		}
	}; //class EaseInAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseOutAction : public EaseRateAction {
		typedef EaseRateAction Base;
		typedef EaseOutAction Self;
	public:
		virtual ~EaseOutAction();
		
		static PassRefPtr<EaseOutAction> create(PassRefPtr<IntervalAction> action, float rate)
		{
			return adoptRef(new EaseOutAction(action, rate));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseOutAction";}
		
	protected:
		EaseOutAction(PassRefPtr<IntervalAction> action, float rate);
		
		virtual Self* doCreateClone()
		{
			return new Self(ATF::adoptRef(m_other->newClone()), m_rate);
		}
		
		virtual Self* doCreateReverse()
		{
			return new EaseOutAction(ATF::adoptRef(m_other->newReverse()), 1.0f/m_rate);
		}
	}; //class EaseOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseInOutAction : public EaseRateAction {
		typedef EaseRateAction Base;
		typedef EaseInOutAction Self;
	public:
		virtual ~EaseInOutAction();
		
		static PassRefPtr<EaseInOutAction> create(PassRefPtr<IntervalAction> action, float rate)
		{
			return adoptRef(new EaseInOutAction(action, rate));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseInOutAction";}
		
	protected:
		EaseInOutAction(PassRefPtr<IntervalAction> action, float rate);
		
		virtual Self* doCreateClone()
		{
			return new Self(ATF::adoptRef(m_other->newClone()), m_rate);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(ATF::adoptRef(m_other->newReverse()), m_rate);
		}
	}; //class EaseInOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseExponentialInAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseExponentialInAction Self;
	public:
		virtual ~EaseExponentialInAction();
		
		static PassRefPtr<EaseExponentialInAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseExponentialInAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseExponentialInAction";}
		
	protected:
		EaseExponentialInAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseExponentialInAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseExponentialOutAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseExponentialOutAction Self;
	public:
		virtual ~EaseExponentialOutAction();
		
		static PassRefPtr<EaseExponentialOutAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseExponentialOutAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseExponentialOutAction";}
		
	protected:
		EaseExponentialOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseExponentialOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseExponentialInOutAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseExponentialInOutAction Self;
	public:
		virtual ~EaseExponentialInOutAction();
		
		static PassRefPtr<Self> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new Self(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseExponentialInOutAction";}
		
	protected:
		EaseExponentialInOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseExponentialInOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseSineInAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseSineInAction Self;
	public:
		virtual ~EaseSineInAction();
		
		static PassRefPtr<Self> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new Self(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseSineInAction";}
		
	protected:
		EaseSineInAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseSineInAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseSineOutAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseSineOutAction Self;
	public:
		virtual ~EaseSineOutAction();
		
		static PassRefPtr<Self> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new Self(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseSineOutAction";}
		
	protected:
		EaseSineOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseSinOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseSineInOutAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseSineInOutAction Self;
	public:
		virtual ~EaseSineInOutAction();
		
		static PassRefPtr<EaseSineInOutAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseSineInOutAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseSineInOutAction";}
		
	protected:
		EaseSineInOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseSineInOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
#define DEFAULT_EASE_ELASTIC_PERIOD 0.3f
	class EaseElasticAction : public EaseAction {
		typedef EaseAction Base;
	public:
		virtual ~EaseElasticAction();
		
		virtual const char* name() const {return "EaseElasticAction";}
		
	protected:
		EaseElasticAction(PassRefPtr<IntervalAction> action, float period);
		float m_period;
	}; //class EaseElasticAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseElasticInAction : public EaseElasticAction {
		typedef EaseElasticAction Base;
		typedef EaseElasticInAction Self;
	public:
		virtual ~EaseElasticInAction();
		
		static PassRefPtr<Self> create(PassRefPtr<IntervalAction> action, float period=DEFAULT_EASE_ELASTIC_PERIOD)
		{
			return adoptRef(new Self(action, period));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseElasticInAction";}
		
	protected:
		EaseElasticInAction(PassRefPtr<IntervalAction> action, float period);
		
		virtual Self* doCreateClone()
		{
			return new Self(ATF::adoptRef(m_other->newClone()), m_period);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(ATF::adoptRef(m_other->newReverse()), m_period);
		}
		
		float m_period;
	}; //class EaseElasticInAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseElasticOutAction : public EaseElasticAction {
		typedef EaseElasticAction Base;
		typedef EaseElasticOutAction Self;
	public:
		virtual ~EaseElasticOutAction();
		
		static PassRefPtr<Self> create(PassRefPtr<IntervalAction> action, float period=DEFAULT_EASE_ELASTIC_PERIOD)
		{
			return adoptRef(new Self(action, period));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseElasticInOutAction";}
		
	protected:
		EaseElasticOutAction(PassRefPtr<IntervalAction> action, float period);
		
		virtual Self* doCreateClone()
		{
			return new Self(ATF::adoptRef(m_other->newClone()), m_period);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(ATF::adoptRef(m_other->newReverse()), m_period);
		}
		
		float m_period;
	}; //class EaseElasticOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseElasticInOutAction : public EaseElasticAction {
		typedef EaseElasticAction Base;
		typedef EaseElasticInOutAction Self;
	public:
		virtual ~EaseElasticInOutAction();
		
		static PassRefPtr<EaseElasticInOutAction> create(PassRefPtr<IntervalAction> action, float period=DEFAULT_EASE_ELASTIC_PERIOD)
		{
			return adoptRef(new EaseElasticInOutAction(action, period));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseElasticInOutAction";}
		
	protected:
		EaseElasticInOutAction(PassRefPtr<IntervalAction> action, float period);
		
		virtual Self* doCreateClone()
		{
			return new Self(ATF::adoptRef(m_other->newClone()), m_period);
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()), m_period);
		}
		
		float m_period;
	}; //class EaseElasticInOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBounceAction : public EaseAction {
		typedef EaseAction Base;
	public:
		virtual ~EaseBounceAction();
		
		virtual const char* name() const {return "EaseBounceAction";}
		
	protected:
		EaseBounceAction(PassRefPtr<IntervalAction> action);
		
		TimeSec bounceTime(TimeSec t);
	}; //class EaseBounceAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBounceInAction : public EaseBounceAction {
		typedef EaseBounceAction Base;
		typedef EaseBounceInAction Self;
	public:
		virtual ~EaseBounceInAction();
		
		static PassRefPtr<EaseBounceInAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseBounceInAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseBounceInAction";}
		
	protected:
		EaseBounceInAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseBounceInAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBounceOutAction : public EaseBounceAction {
		typedef EaseBounceAction Base;
		typedef EaseBounceOutAction Self;
	public:
		virtual ~EaseBounceOutAction();
		
		static PassRefPtr<EaseBounceOutAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseBounceOutAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseBounceOutAction";}
		
	protected:
		EaseBounceOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseBounceOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBounceInOutAction : public EaseBounceAction {
		typedef EaseBounceAction Base;
		typedef EaseBounceInOutAction Self;
	public:
		virtual ~EaseBounceInOutAction();
		
		static PassRefPtr<EaseBounceInOutAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseBounceInOutAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseBounceInOutAction";}
		
	protected:
		EaseBounceInOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseBounceInOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBackInAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseBackInAction Self;
	public:
		virtual ~EaseBackInAction();
		
		static PassRefPtr<EaseBackInAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseBackInAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseBackInAction";}
		
	protected:
		EaseBackInAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseBackInAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBackOutAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseBackOutAction Self;
	public:
		virtual ~EaseBackOutAction();
		
		static PassRefPtr<EaseBackOutAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseBackOutAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseBackOutAction";}
		
	protected:
		EaseBackOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseBackOutAction
	
	///-------------------------------------------------------------------------------------------------------------------
	class EaseBackInOutAction : public EaseAction {
		typedef EaseAction Base;
		typedef EaseBackInOutAction Self;
	public:
		virtual ~EaseBackInOutAction();
		
		static PassRefPtr<EaseBackInOutAction> create(PassRefPtr<IntervalAction> action)
		{
			return adoptRef(new EaseBackInOutAction(action));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "EaseBackInOutAction";}
		
	protected:
		EaseBackInOutAction(PassRefPtr<IntervalAction> action);
		
		virtual Self* doCreateClone()
		{
			return new Self(adoptRef(m_other->newClone()));
		}
		
		virtual Self* doCreateReverse()
		{
			return new Self(adoptRef(m_other->newReverse()));
		}
	}; //class EaseBackInOutAction
}

#endif
