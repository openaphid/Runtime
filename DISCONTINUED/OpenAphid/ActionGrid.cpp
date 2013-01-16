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

#include "ActionGrid.h"
#include "Node.h"
#include "AJGridBaseAction.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	void GridBaseAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		
		RefPtr<GridBase> newGrid = createGrid();
		
		GridBase* targetGrid = target->grid();
		
		if (targetGrid && targetGrid->reuseGrid() > 0) {
			if (targetGrid->active() && targetGrid->gridSize().x == m_gridSize.x && targetGrid->gridSize().y == m_gridSize.y /* && [targetGrid isKindOfClass:[newgrid class]]*/)//TODO:
				targetGrid->reuse();
			else {
				oa_error("Can't reuse grid");
				CRASH(); //TODO: throw an exception
			}
		}	else {
			if (targetGrid && targetGrid->active())
				targetGrid->setActive(false);
			
			target->setGrid(newGrid);
			target->grid()->setActive(true);
		}
	}
	
	const AJ::ClassInfo* GridBaseAction::wrapperClassInfo() const
	{
		return &AJGridBaseAction::s_info;
	}
	
	AJ::AJObject* GridBaseAction::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJGridBaseAction>(exec, globalObject, this);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Grid3DAction
	PassRefPtr<GridBase> Grid3DAction::createGrid()
	{
		return Grid3D::create(m_gridSize);
	}
	
	ccVertex3F Grid3DAction::vertex(const GridSize& pos)
	{
		Grid3D* grid = static_cast<Grid3D*>(m_target->grid());
		return grid->vertex(pos);
	}
	
	ccVertex3F Grid3DAction::originalVertex(const GridSize& pos)
	{
		Grid3D* grid = static_cast<Grid3D*>(m_target->grid());
		return grid->originalVertex(pos);
	}
	
	void Grid3DAction::setVertex(const GridSize& pos, const ccVertex3F& vertex)
	{
		Grid3D* grid = static_cast<Grid3D*>(m_target->grid());
		grid->setVertex(pos, vertex);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TiledGrid3DAction	
  PassRefPtr<GridBase> TiledGrid3DAction::createGrid()
	{
		return TiledGrid3D::create(m_gridSize);
	}
	
	ccQuad3 TiledGrid3DAction::tile(const GridSize& pos)
	{
		TiledGrid3D* grid = static_cast<TiledGrid3D*>(m_target->grid());
		return grid->tile(pos);
	}
	
	ccQuad3 TiledGrid3DAction::originalTile(const GridSize& pos)
	{
		TiledGrid3D* grid = static_cast<TiledGrid3D*>(m_target->grid());
		return grid->originalTile(pos);
	}
	
	void TiledGrid3DAction::setTile(const GridSize& pos, const ccQuad3& coords)
	{
		TiledGrid3D* grid = static_cast<TiledGrid3D*>(m_target->grid());
		grid->setTile(pos, coords);
	}

	void TiledGrid3DAction::turnOnTile(const GridSize& pos)
  {
	  setTile(pos, originalTile(pos));
  }

  void TiledGrid3DAction::turnOffTile(const GridSize& pos)
  {
	  ccQuad3 coords;
	  bzero(&coords, sizeof(ccQuad3));
	  setTile(pos, coords);
  }
	
	///-------------------------------------------------------------------------------------------------------------------
	/// AccelDeccelAmplitudeAction
	AccelDeccelAmplitudeAction::AccelDeccelAmplitudeAction(PassRefPtr<GridBaseAction> action, TimeSec duration)
	: Base(duration)
	, m_other(action)
	, m_rate(1.0f)
	{
		LEAK_DETECT_INC("AccelDeccelAmplitudeAction");
	}
	
	AccelDeccelAmplitudeAction::~AccelDeccelAmplitudeAction()
	{
		LEAK_DETECT_DEC("AccelDeccelAmplitudeAction");
	}
	
	void AccelDeccelAmplitudeAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_other->Action::startWithTarget(target);
	}
	
	void AccelDeccelAmplitudeAction::update(TimeSec time)
	{
		float f = time * 2;
		
		if (f > 1) {
			f -= 1;
			f = 1 - f;
		}
		
		m_other->setAmplitudeRate(powf(f, m_rate));
		m_other->handleUpdate(time);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// AccelAmplitudeAction
	AccelAmplitudeAction::AccelAmplitudeAction(PassRefPtr<GridBaseAction> action, TimeSec duration)
	: Base(duration)
	, m_other(action)
	, m_rate(1.0f)
	{
		LEAK_DETECT_INC("AccelAmplitudeAction");
	}
	
	AccelAmplitudeAction::~AccelAmplitudeAction()
	{
		LEAK_DETECT_DEC("AccelAmplitudeAction");
	}
	
	void AccelAmplitudeAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_other->Action::startWithTarget(target);
	}
	
	void AccelAmplitudeAction::update(TimeSec time)
	{		
		m_other->setAmplitudeRate(powf(time, m_rate));
		m_other->handleUpdate(time);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// DeccelAmplitudeAction
	DeccelAmplitudeAction::DeccelAmplitudeAction(PassRefPtr<GridBaseAction> action, TimeSec duration)
	: Base(duration)
	, m_other(action)
	, m_rate(1.0f)
	{
		LEAK_DETECT_INC("DeccelAmplitudeAction");
	}
	
	DeccelAmplitudeAction::~DeccelAmplitudeAction()
	{
		LEAK_DETECT_DEC("DeccelAmplitudeAction");
	}
	
	void DeccelAmplitudeAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		m_other->Action::startWithTarget(target);
	}
	
	void DeccelAmplitudeAction::update(TimeSec time)
	{		
		m_other->setAmplitudeRate(powf(1 - time, m_rate));
		m_other->handleUpdate(time);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// StopGridAction
	StopGridAction::StopGridAction()
	{
		LEAK_DETECT_INC("StopGridAction");
	}
	
	StopGridAction::~StopGridAction()
	{
		LEAK_DETECT_DEC("StopGridAction");
	}
	
	void StopGridAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		if (this->target() && this->target()->grid() && this->target()->grid()->active()) //TODO: can use target directly?
			this->target()->grid()->setActive(false);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// ReuseGridAction
	ReuseGridAction::ReuseGridAction(int times)
	: m_times(times)
	{
		LEAK_DETECT_INC("ReuseGridAction");
	}
	
	ReuseGridAction::~ReuseGridAction()
	{
		LEAK_DETECT_DEC("ReuseGridAction");
	}
	
	void ReuseGridAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		if (this->target() && this->target()->grid() && this->target()->grid()->active())
			this->target()->grid()->setReuseGrid(this->target()->grid()->reuseGrid());
	}
	
}