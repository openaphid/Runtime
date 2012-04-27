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

#ifndef OpenAphid_ActionTiledGrid_h
#define OpenAphid_ActionTiledGrid_h

#include "ActionGrid.h"

namespace Aphid {
  typedef struct
	{
		Point	position;
		Point	startPosition;
		GridSize	delta;
	} Tile;
	///-------------------------------------------------------------------------------------------------------------------
	class ShakyTiles3DAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef ShakyTiles3DAction Self;
	public:
		virtual ~ShakyTiles3DAction();
		
		static PassRefPtr<Self> create(int range, bool shakeZ, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(range, shakeZ, gridSize, duration));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "ShakyTiles3DAction";}
		
	protected:
		ShakyTiles3DAction(int range, bool shakeZ, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_randRange;
		bool m_shakeZ;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class ShatteredTiles3DAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef ShatteredTiles3DAction Self;
	public:
		virtual ~ShatteredTiles3DAction();
		
		static PassRefPtr<Self> create(int range, bool shatterZ, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(range, shatterZ, gridSize, duration));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "ShatteredTiles3DAction";}
		
	protected:
		ShatteredTiles3DAction(int range, bool shatterZ, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_randRange;
		bool m_once;
		bool m_shatterZ;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class ShuffleTilesAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef ShuffleTilesAction Self;
	public:
		virtual ~ShuffleTilesAction();
		
		static PassRefPtr<Self> create(int seed, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(seed, gridSize, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "ShuffleTilesAction";}
		
	protected:
		ShuffleTilesAction(int seed, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_seed;
		size_t m_tilesCount;
		int* m_tilesOrder;
		void* m_tiles;
		
	private:
		GridSize getDelta(const GridSize& pos);
		void placeTile(const GridSize& pos, const Tile& t);
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FadeOutTilesBaseAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
	public:
		virtual ~FadeOutTilesBaseAction() {}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "FadeOutTilesBaseAction";}
		
	protected:
		FadeOutTilesBaseAction(const GridSize& gridSize, TimeSec duration) : Base(gridSize, duration) {}
		
		virtual float testFunc(const GridSize& pos, TimeSec time);
		virtual void transformTile(const GridSize& pos, float distance);
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FadeOutTRTilesAction : public FadeOutTilesBaseAction {
		typedef FadeOutTilesBaseAction Base;
		typedef FadeOutTRTilesAction Self;
	public:
		virtual ~FadeOutTRTilesAction();
		
		static PassRefPtr<Self> create(const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(gridSize, duration));
		}
		
		virtual const char* name() const {return "FadeOutTRTilesAction";}
		
	protected:
		FadeOutTRTilesAction(const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FadeOutBLTilesAction : public FadeOutTilesBaseAction {
		typedef FadeOutTilesBaseAction Base;
		typedef FadeOutBLTilesAction Self;
	public:
		virtual ~FadeOutBLTilesAction();
		
		static PassRefPtr<Self> create(const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(gridSize, duration));
		}
		
		virtual const char* name() const {return "FadeOutBLTilesAction";}
		
	protected:
		FadeOutBLTilesAction(const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		virtual float testFunc(const GridSize& pos, TimeSec time);
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FadeOutUpTilesAction : public FadeOutTilesBaseAction {
		typedef FadeOutTilesBaseAction Base;
		typedef FadeOutUpTilesAction Self;
	public:
		virtual ~FadeOutUpTilesAction();
		
		static PassRefPtr<Self> create(const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(gridSize, duration));
		}
		
		virtual const char* name() const {return "FadeOutUpTilesAction";}
		
	protected:
		FadeOutUpTilesAction(const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		virtual float testFunc(const GridSize& pos, TimeSec time);
		virtual void transformTile(const GridSize& pos, float distance);
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FadeOutDownTilesAction : public FadeOutTilesBaseAction {
		typedef FadeOutTilesBaseAction Base;
		typedef FadeOutDownTilesAction Self;
	public:
		virtual ~FadeOutDownTilesAction();
		
		static PassRefPtr<Self> create(const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(gridSize, duration));
		}
		
		virtual const char* name() const {return "FadeOutDownTilesAction";}
		
	protected:
		FadeOutDownTilesAction(const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		virtual float testFunc(const GridSize& pos, TimeSec time);
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class TurnOffTilesAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef TurnOffTilesAction Self;
	public:
		virtual ~TurnOffTilesAction();
		
		static PassRefPtr<Self> create(int seed, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(seed, gridSize, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "TurnOffTilesAction";}
		
	protected:
		TurnOffTilesAction(int seed, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_seed;
		size_t m_tilesCount;
		int* m_tilesOrder;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class WavesTiles3DAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef WavesTiles3DAction Self;
	public:
		virtual ~WavesTiles3DAction();
		
		static PassRefPtr<Self> create(int waves, float amplitude, GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(waves, amplitude, gridSize, duration));
		}
		
		virtual void update(TimeSec t);
		
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float amplitude) {m_amplitude = amplitude;}
		
		virtual const char* name() const {return "WavesTiles3DAction";}
		
	protected:
		WavesTiles3DAction(int waves, float amplitude, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_waves;
		float m_amplitude;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class JumpTiles3DAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef JumpTiles3DAction Self;
	public:
		virtual ~JumpTiles3DAction();
		
		static PassRefPtr<Self> create(int jumps, float amplitude, GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(jumps, amplitude, gridSize, duration));
		}
		
		virtual void update(TimeSec t);
		
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float amplitude) {m_amplitude = amplitude;}
		
		virtual const char* name() const {return "JumpTiles3DAction";}
		
	protected:
		JumpTiles3DAction(int jumps, float amplitude, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_jumps;
		float m_amplitude;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class SplitRowsAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef SplitRowsAction Self;
	public:
		virtual ~SplitRowsAction();
		
		static PassRefPtr<Self> create(int rows, TimeSec duration)
		{
			return adoptRef(new Self(rows, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "SplitRowsAction";}
		
	protected:
		SplitRowsAction(int rows, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_rows;
		Size m_winSize;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class SplitColumnsAction : public TiledGrid3DAction {
		typedef TiledGrid3DAction Base;
		typedef SplitColumnsAction Self;
	public:
		virtual ~SplitColumnsAction();
		
		static PassRefPtr<Self> create(int cols, TimeSec duration)
		{
			return adoptRef(new Self(cols, duration));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "SplitColumnsAction";}
		
	protected:
		SplitColumnsAction(int cols, TimeSec duration);
		
		virtual Self* doCreateClone();
		
		int m_cols;
		Size m_winSize;
	};
}

#endif
