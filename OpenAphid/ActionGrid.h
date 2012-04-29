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

#ifndef OpenAphid_ActionGrid_h
#define OpenAphid_ActionGrid_h

#include "Action.h"
#include "Grid.h"
#include "ActionInstant.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	class GridBaseAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef GridBaseAction Self;
	public:
		virtual ~GridBaseAction() {}
		
		virtual void startWithTarget(Node* target);		
		
		virtual void setAmplitudeRate(float rate) {m_amplitudeRate = rate;}
		virtual float amplitudeRate() const {return m_amplitudeRate;}
		
		virtual PassRefPtr<GridBase> createGrid() = 0;
		
		virtual int baseType() const {return ActionTypeGrid | Base::baseType();}
		virtual const char* name() const {return "GridBaseAction";}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		GridSize gridSize() const {return m_gridSize;}
		void setGridSize(const GridSize& size) {m_gridSize = size;}
		
		virtual Self* newClone()
		{
			return static_cast<Self*>(Base::newClone());
		}
		
		virtual Self* newReverse()
		{
			return static_cast<Self*>(Base::newReverse());
		}
		
	protected:
		GridBaseAction(const GridSize& gridSize, TimeSec duration)
		: Base(duration)
		, m_gridSize(gridSize)
		, m_amplitudeRate(1.0f)
		{}
		
		virtual void postSetupReverse(Action* reversed)
		{
			reversed->setTimeReversed(!isTimeReversed());
		}
		
		GridSize m_gridSize;
		float m_amplitudeRate;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Grid3DAction : public GridBaseAction {
		typedef GridBaseAction Base;
		typedef Grid3DAction Self;
	public:
		virtual ~Grid3DAction() {}
		
		virtual PassRefPtr<GridBase> createGrid();
		
		virtual int baseType() const {return ActionTypeGrid3D | Base::baseType();}
		virtual const char* name() const {return "Grid3DAction";}
		
		ccVertex3F vertex(const GridSize& pos);
		ccVertex3F originalVertex(const GridSize& pos);
		void setVertex(const GridSize& pos, const ccVertex3F& vertex);
		
	protected:
		Grid3DAction(const GridSize& gridSize, TimeSec duration) : Base(gridSize, duration){}
		
		virtual Self* doCreateClone() = 0;

		virtual Self* doCreateReverse()
		{
			return doCreateClone();
		}
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class TiledGrid3DAction : public GridBaseAction {
		typedef GridBaseAction Base;
		typedef TiledGrid3DAction Self;
	public:
		virtual ~TiledGrid3DAction() {}
		
		virtual PassRefPtr<GridBase> createGrid();
		
		virtual int baseType() const {return ActionTypeTiledGrid3D | Base::baseType();}
		virtual const char* name() const {return "TiledGrid3DAction";}
		
		ccQuad3 tile(const GridSize& pos);
		ccQuad3 originalTile(const GridSize& pos);
		void setTile(const GridSize& pos, const ccQuad3& coords);
		
	protected:
		TiledGrid3DAction(const GridSize& gridSize, TimeSec duration) : Base(gridSize, duration){}
		
		virtual Self* doCreateClone() = 0;

		virtual Self* doCreateReverse()
		{
			return doCreateClone();
		}

		void turnOnTile(const GridSize& pos);
		void turnOffTile(const GridSize& pos);
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class AccelDeccelAmplitudeAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef AccelDeccelAmplitudeAction Self;
	public:
		virtual ~AccelDeccelAmplitudeAction();
		
		static PassRefPtr<Self> create(PassRefPtr<GridBaseAction> action, TimeSec duration)
		{
			return adoptRef(new Self(action, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "AccelDeccelAmplitudeAction";}
		
		float rate() const {return m_rate;}
		void setRate(float rate) {m_rate = rate;}
		
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
		AccelDeccelAmplitudeAction(PassRefPtr<GridBaseAction> action, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			Self* action = new Self(adoptRef(m_other->newClone()), m_duration);
			action->setRate(m_rate); //TODO: different impl from CC
			
			return action;
		}
		
		virtual Self* doCreateReverse()
		{
			Self* action = new Self(adoptRef(m_other->newReverse()), m_duration);
			action->setRate(m_rate); //TODO: different impl from CC
			
			return action;
		}
		
		RefPtr<GridBaseAction> m_other;
		float m_rate;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class AccelAmplitudeAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef AccelAmplitudeAction Self;
	public:
		virtual ~AccelAmplitudeAction();
		
		static PassRefPtr<Self> create(PassRefPtr<GridBaseAction> action, TimeSec duration)
		{
			return adoptRef(new Self(action, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "AccelAmplitudeAction";}
		
		float rate() const {return m_rate;}
		void setRate(float rate) {m_rate = rate;}
		
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
		AccelAmplitudeAction(PassRefPtr<GridBaseAction> action, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			Self* action = new Self(adoptRef(m_other->newClone()), m_duration);
			action->setRate(m_rate); //TODO: different impl from CC
			
			return action;
		}
		
		virtual Self* doCreateReverse()
		{
			Self* action = new Self(adoptRef(m_other->newReverse()), m_duration);
			action->setRate(m_rate); //TODO: different impl from CC
			
			return action;
		}
		
		RefPtr<GridBaseAction> m_other;
		float m_rate;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class DeccelAmplitudeAction : public IntervalAction {
		typedef IntervalAction Base;
		typedef DeccelAmplitudeAction Self;
	public:
		virtual ~DeccelAmplitudeAction();
		
		static PassRefPtr<Self> create(PassRefPtr<GridBaseAction> action, TimeSec duration)
		{
			return adoptRef(new Self(action, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "DeccelAmplitudeAction";}
		
		float rate() const {return m_rate;}
		void setRate(float rate) {m_rate = rate;}
		
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
		DeccelAmplitudeAction(PassRefPtr<GridBaseAction> action, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			Self* action = new DeccelAmplitudeAction(adoptRef(m_other->newClone()), m_duration);
			action->setRate(m_rate); //TODO: different impl from CC
			
			return action;
		}
		
		virtual Self* doCreateReverse()
		{
			Self* action = new DeccelAmplitudeAction(adoptRef(m_other->newReverse()), m_duration);
			action->setRate(m_rate); //TODO: different impl from CC
			
			return action;
		}
		
		RefPtr<GridBaseAction> m_other;
		float m_rate;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class StopGridAction : public InstantAction {
		typedef InstantAction Base;
		typedef StopGridAction Self;
	public:
		virtual ~StopGridAction();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		virtual void startWithTarget(Aphid::Node *target);
		
		virtual const char* name() const {return "StopGridAction";}
		
	protected:
		StopGridAction();
		
		virtual Self* doCreateClone()
		{
			return new Self();
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class ReuseGridAction : public InstantAction {
		typedef InstantAction Base;
		typedef ReuseGridAction Self;
	public:
		virtual ~ReuseGridAction();
		
		static PassRefPtr<Self> create(int times)
		{
			return adoptRef(new Self(times));
		}
		
		virtual void startWithTarget(Aphid::Node *target);
		
		virtual const char* name() const {return "ReuseGridAction";}
		
	protected:
		ReuseGridAction(int times);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_times);
		}
		
		virtual Self* doCreateReverse()
		{
			return Self::doCreateClone();
		}
		
		int m_times;
	};
}

#endif
