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

#ifndef OpenAphid_ActionInterval_h
#define OpenAphid_ActionInterval_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "Action.h"
#include "ccTypes.h"
#include "Animation.h"

namespace Aphid {
  typedef Vector<RefPtr<FiniteTimeAction> > FiniteTimeActionVector;
	
	///-------------------------------------------------------------------------------------------------------------------
	class SequenceAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef SequenceAction Self;
	public:		
	  virtual ~SequenceAction();
		
	  static PassRefPtr<Self> create(const FiniteTimeActionVector& actions)
		{
			return adoptRef(new Self(actions));
		}
		
		virtual void startWithTarget(Node* target);
	  virtual void stop();
	  virtual void update(TimeSec t);
		
		virtual const char* name() const {return "SequenceAction";}
		
		virtual void becomeManaged();
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:		
	  SequenceAction(const FiniteTimeActionVector& actions);
		
		static TimeSec sumDuration(const FiniteTimeActionVector& actions, int to=-1);
		
	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		virtual bool supportsReversedTime() {return false;}
		
		FiniteTimeActionVector m_actions;
		TimeSec m_split, m_lastSplit;
		int m_index;
  }; //class SequenceAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class RepeatAction : public IntervalAction {
	  typedef IntervalAction Base;
	  typedef RepeatAction Self;
	public:
	  virtual ~RepeatAction();

	  static PassRefPtr<RepeatAction> create(PassRefPtr<FiniteTimeAction> action, size_t times)
		{
			return ATF::adoptRef(new RepeatAction(action, times));
		}

	  virtual void startWithTarget(Node* target);
	  virtual void stop();
	  virtual void update(TimeSec t);
	  virtual bool isDone();
	  
		virtual const char* name() const {return "RepeatAction";}
		
		virtual void becomeManaged()
		{
			Base::becomeManaged();
			m_innerAction->becomeManaged();
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID)
		{
			if (isMarked(markID))
				return;
			
			Base::markObjects(markStack, markID);
			m_innerAction->markObjects(markStack, markID);
		}
		
	protected:
	  RepeatAction(PassRefPtr<FiniteTimeAction> action, size_t times);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  size_t m_times;
	  size_t m_total;
	  RefPtr<FiniteTimeAction> m_innerAction;
  }; //class RepeatAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class SpawnAction : public IntervalAction {
	  typedef IntervalAction Base;
	  typedef SpawnAction Self;
	public:
	  virtual ~SpawnAction();

	  static PassRefPtr<SpawnAction> create(const FiniteTimeActionVector& actions);
	  static PassRefPtr<SpawnAction> create(PassRefPtr<FiniteTimeAction> one, PassRefPtr<FiniteTimeAction> two)
		{
			return ATF::adoptRef(new SpawnAction(one, two));
		}

	  void setTwo(PassRefPtr<FiniteTimeAction> action0, PassRefPtr<FiniteTimeAction> action1);
		
		virtual void startWithTarget(Node* target);
	  virtual void stop();
	  virtual void update(TimeSec t);
		
		virtual const char* name() const {return "SpawnAction";}
		
		virtual void becomeManaged()
		{
			Base::becomeManaged();
			m_action0->becomeManaged();
			m_action1->becomeManaged();
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID)
		{
			if (isMarked(markID))
				return;
			
			Base::markObjects(markStack, markID);
			m_action0->markObjects(markStack, markID);
			m_action1->markObjects(markStack, markID);
		}
		
	protected:
	  SpawnAction(PassRefPtr<FiniteTimeAction> one, PassRefPtr<FiniteTimeAction> two);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  RefPtr<FiniteTimeAction> m_action0;
	  RefPtr<FiniteTimeAction> m_action1;
  }; //class SpawnAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class RotateToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef RotateToAction Self;
	public:
	  virtual ~RotateToAction();
		
		static PassRefPtr<RotateToAction> create(TimeSec duration, float angle)
		{
			return adoptRef(new RotateToAction(duration, angle));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "RotateToAction";}
		
	protected:
	  RotateToAction(TimeSec duration, float angle);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  float m_dstAngle;
	  float m_startAngle;
	  float m_diffAngle;
  }; //class RotateToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class RotateByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef RotateByAction Self;
	public:
	  virtual ~RotateByAction();
		
		static PassRefPtr<RotateByAction> create(TimeSec duration, float deltaAngle)
		{
			return ATF::adoptRef(new RotateByAction(duration, deltaAngle));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "RotateByAction";}
		
	protected:
	  RotateByAction(TimeSec duration, float deltaAngle);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  float m_angle;
	  float m_startAngle;
  }; //class RotateByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class MoveToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef MoveToAction Self;
	public:
	  virtual ~MoveToAction();
		
		static PassRefPtr<MoveToAction> create(TimeSec duration, const Point& p)
		{
			return ATF::adoptRef(new MoveToAction(duration, p));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "MoveToAction";}
		
	protected:
	  MoveToAction(TimeSec duration, const Point& p);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  Point m_endPosition;
	  Point m_startPosition;
	  Point m_delta;
  }; //class MoveToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class MoveByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef MoveByAction Self;
	public:
	  virtual ~MoveByAction();
		
		static PassRefPtr<MoveByAction> create(TimeSec duration, const Point& delta)
		{
			return ATF::adoptRef(new MoveByAction(duration, delta));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "MoveByAction";}
		
	protected:
	  MoveByAction(TimeSec duration, const Point& delta);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		Point m_startPosition;
	  Point m_delta;
  }; //class MoveByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class SkewToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef SkewToAction Self;
	public:
	  virtual ~SkewToAction();
		
		static PassRefPtr<SkewToAction> create(TimeSec duration, float skewX, float skewY)
		{
			return ATF::adoptRef(new SkewToAction(duration, skewX, skewY));
		}
		
		static PassRefPtr<SkewToAction> create(TimeSec duration, const Vector2& skew)
		{
			return ATF::adoptRef(new SkewToAction(duration, skew.x, skew.y));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "SkewToAction";}

	protected:
	  SkewToAction(TimeSec duration, float skewX, float skewY);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
	  Vector2 m_startSkew;
	  Vector2 m_endSkew;
	  Vector2 m_delta;
  }; //class SkewToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class SkewByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef SkewByAction Self;
	public:
	  virtual ~SkewByAction();
		
		static PassRefPtr<SkewByAction> create(TimeSec duration, const Vector2& deltaSkew)
		{
			return ATF::adoptRef(new SkewByAction(duration, deltaSkew.x, deltaSkew.y));
		}

		static PassRefPtr<SkewByAction> create(TimeSec duration, float deltaX, float deltaY)
		{
			return ATF::adoptRef(new SkewByAction(duration, deltaX, deltaY));
		}

		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "SkewByAction";}

	protected:
	  SkewByAction(TimeSec duration, float deltaX, float deltaY);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  Vector2 m_startSkew;
	  Vector2 m_delta;
  }; //class SkewByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class JumpByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef JumpByAction Self;
	public:
	  virtual ~JumpByAction();
		
		static PassRefPtr<JumpByAction> create(TimeSec duration, const Point& position, TimeSec height, size_t jumps)
		{
			return ATF::adoptRef(new JumpByAction(duration, position, height, jumps));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "JumpByAction";}
		
	protected:
	  JumpByAction(TimeSec duration, const Point& position, TimeSec height, size_t jumps);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  Point m_startPosition;
	  Point m_delta;
	  TimeSec m_height;
	  size_t m_jumps;
  }; //class JumpByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class JumpToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef JumpToAction Self;
	public:
	  virtual ~JumpToAction();
		
		static PassRefPtr<JumpToAction> create(TimeSec duration, const Point& position, TimeSec height, size_t jumps)
		{
			return ATF::adoptRef(new JumpToAction(duration, position, height, jumps));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "JumpToAction";}
		
	protected:
	  JumpToAction(TimeSec duration, const Point& position, TimeSec height, size_t jumps);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		Point m_startPosition;
		Point m_endPosition;
	  Point m_delta;
	  TimeSec m_height;
	  size_t m_jumps;
  }; //class JumpToAction
	
	typedef struct _ccBezierConfig {
		//! end position of the bezier
		Point endPosition;
		//! Bezier control point 1
		Point controlPoint_1;
		//! Bezier control point 2
		Point controlPoint_2;
	} ccBezierConfig;
	
  ///-------------------------------------------------------------------------------------------------------------------
  class BezierByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef BezierByAction Self;
	public:		
	  virtual ~BezierByAction();
		
		static ATF::PassRefPtr<BezierByAction> create(TimeSec duration, const ccBezierConfig& c)
		{
			return ATF::adoptRef(new BezierByAction(duration, c));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "BezierByAction";}
		
	protected:
	  BezierByAction(TimeSec duration, const ccBezierConfig& c);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		ccBezierConfig m_config;
		Point m_startPosition;
  }; //class BezierByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class BezierToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef BezierToAction Self;
	public:		
	  virtual ~BezierToAction();
		
		static ATF::PassRefPtr<BezierToAction> create(TimeSec duration, const ccBezierConfig& c)
		{
			return ATF::adoptRef(new BezierToAction(duration, c));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "BezierToAction";}
		
	protected:		
	  BezierToAction(TimeSec duration, const ccBezierConfig& c);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		ccBezierConfig m_config;
		Point m_startPosition;		
  }; //class BezierToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class ScaleToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef ScaleToAction Self;
	public:
	  virtual ~ScaleToAction();
		
		static ATF::PassRefPtr<ScaleToAction> create(TimeSec duration, const Vector2& scale)
		{
			return ATF::adoptRef(new ScaleToAction(duration, scale));
		}
		
		static ATF::PassRefPtr<ScaleToAction> create(TimeSec duration, float scaleX, float scaleY)
		{
			Vector2 scale = {scaleX, scaleY};
			return ATF::adoptRef(new ScaleToAction(duration, scale));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "ScaleToAction";}

	protected:
	  ScaleToAction(TimeSec duration, const Vector2& scale);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		Vector2 m_startScale;
		Vector2 m_endScale;
		Vector2 m_delta;
  }; //class ScaleToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class ScaleByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef ScaleByAction Self;
	public:		
	  virtual ~ScaleByAction();
		
		static ATF::PassRefPtr<ScaleByAction> create(TimeSec duration, const Vector2& scale)
		{
			return ATF::adoptRef(new ScaleByAction(duration, scale));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "ScaleByAction";}

	protected:		
	  ScaleByAction(TimeSec duration, const Vector2& scale);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		Vector2 m_startScale;
		Vector2 m_scaleBy;
		Vector2 m_delta;		
  }; //class ScaleByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class BlinkAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef BlinkAction Self;
	public:
	  virtual ~BlinkAction();
		
		static ATF::PassRefPtr<BlinkAction> create(TimeSec duration, size_t times)
		{
			return ATF::adoptRef(new BlinkAction(duration, times));
		}
		
		virtual void update(TimeSec t);

		virtual const char* name() const {return "BlinkAction";}
		
	protected:
	  BlinkAction(TimeSec duration, size_t times);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  size_t m_times;
  }; //class BlinkAction
	
  ///-------------------------------------------------------------------------------------------------------------------
	class FadeOutAction;
	
  class FadeInAction : public IntervalAction {
	  typedef IntervalAction Base;
	  typedef FadeInAction Self;
	public:
	  virtual ~FadeInAction();
		
		static ATF::PassRefPtr<FadeInAction> create(TimeSec duration)
		{
			return ATF::adoptRef(new FadeInAction(duration));
		}
		
		virtual void update(TimeSec t);

		virtual const char* name() const {return "FadeInAction";}
		
	protected:
	  FadeInAction(TimeSec duration);
		
		virtual Self* doCreateClone();
		virtual IntervalAction* doCreateReverse();
  }; //class FaceInAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class FadeOutAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef FadeOutAction Self;
	public:
	  virtual ~FadeOutAction();
		
		static ATF::PassRefPtr<FadeOutAction> create(TimeSec duration)
		{
			return ATF::adoptRef(new FadeOutAction(duration));
		}
		
		virtual void update(TimeSec t);

		virtual const char* name() const {return "FadeOutAction";}

	protected:
	  FadeOutAction(TimeSec duration);

	  virtual Self* doCreateClone();
		virtual IntervalAction* doCreateReverse();
  }; //class FadeOutAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class FadeToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef FadeToAction Self;
	public:
	  virtual ~FadeToAction();
		
		static ATF::PassRefPtr<FadeToAction> create(TimeSec duration, GLubyte o)
		{
			return ATF::adoptRef(new FadeToAction(duration, o));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "FadeToAction";}
		
	protected:
	  FadeToAction(TimeSec duration, GLubyte o);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  GLubyte m_toOpacity;
	  GLubyte m_fromOpacity;
  }; //class FadeToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class TintToAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef TintToAction Self;
	public:
	  virtual ~TintToAction();
		
		static ATF::PassRefPtr<TintToAction> create(TimeSec duration, GLubyte r, GLubyte g, GLubyte b)
		{
			return ATF::adoptRef(new TintToAction(duration, r, g, b));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "TintToAction";}
		
	protected:
	  TintToAction(TimeSec duration, GLubyte r, GLubyte g, GLubyte b);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  ccColor3B m_to;
	  ccColor3B m_from;
  }; //class TintToAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class TintByAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef TintByAction Self;
	public:
	  virtual ~TintByAction();
		
		static ATF::PassRefPtr<TintByAction> create(TimeSec duration, GLshort r, GLshort g, GLshort b)
		{
			return ATF::adoptRef(new TintByAction(duration, r, g, b));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);

		virtual const char* name() const {return "TintByAction";}
		
	protected:
	  TintByAction(TimeSec duration, GLshort r, GLshort g, GLshort b);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();

	  GLshort m_deltaR, m_deltaG, m_deltaB;
	  GLshort m_fromR, m_fromG, m_fromB; //why use shorts here?
  }; //class TintByAction
	
  ///-------------------------------------------------------------------------------------------------------------------
  class DelayTimeAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef DelayTimeAction Self;
	public:
	  virtual ~DelayTimeAction();

	  static PassRefPtr<DelayTimeAction> create(TimeSec duration)
		{
			return ATF::adoptRef(new DelayTimeAction(duration));
		}
		
		virtual void update(TimeSec t)
		{
			UNUSED_PARAM(t);
		}

		virtual const char* name() const {return "DelayTimeAction";}
		
	protected:		
	  DelayTimeAction(TimeSec duration);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
  }; //class DelayTimeAction

  ///-------------------------------------------------------------------------------------------------------------------
  class AnimateAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef AnimateAction Self;
	public:
	  virtual ~AnimateAction();
		
		static PassRefPtr<Self> create(ATF::PassRefPtr<Animation> animation, bool restoreOriginalFrame=true);
		
		static PassRefPtr<Self> create(TimeSec duration, ATF::PassRefPtr<Animation> animation, bool restoreOriginalFrame=true)
		{
			return ATF::adoptRef(new Self(duration, animation, restoreOriginalFrame));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void stop();
		virtual void update(TimeSec t);

		virtual const char* name() const {return "AnimateAction";}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
	  AnimateAction(TimeSec duration, ATF::PassRefPtr<Animation> animation, bool restoreOriginalFrame=true);

	  virtual Self* doCreateClone();
		virtual Self* doCreateReverse();
		
		RefPtr<Animation> m_animation;
		RefPtr<SpriteFrame> m_originalFrame;
		bool m_restoreOriginalFrame;
		
  }; //class AnimateAction
}


#endif
