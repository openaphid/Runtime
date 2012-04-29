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

#ifndef OpenAphid_ActionGrid3D_h
#define OpenAphid_ActionGrid3D_h

#include "ActionGrid.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	class Waves3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef Waves3DAction Self;
	public:
		virtual ~Waves3DAction();
		
		static PassRefPtr<Self> create(int waves, float amplitude, const GridSize& size, TimeSec duration)
		{
			return adoptRef(new Self(waves, amplitude, size, duration));
		}
		
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float v) {m_amplitude = v;}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "Waves3DAction";}
		
	protected:
		Waves3DAction(int waves, float amplitude, const GridSize& size, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_waves, m_amplitude, m_gridSize, m_duration);
		}
		
		int m_waves;
		float m_amplitude;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FlipX3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef FlipX3DAction Self;
	public:
		virtual ~FlipX3DAction();
		
		static PassRefPtr<Self> create(TimeSec duration)
		{
			return adoptRef(new Self(duration));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "FlipX3DAction";}
		
	protected:
		FlipX3DAction(TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_duration);
		}		
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FlipY3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef FlipY3DAction Self;
	public:
		virtual ~FlipY3DAction();
		
		static PassRefPtr<Self> create(TimeSec duration)
		{
			return adoptRef(new Self(duration));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "FlipY3DAction";}
		
	protected:
		FlipY3DAction(TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_duration);
		}		
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Lens3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef Lens3DAction Self;
	public:
		virtual ~Lens3DAction();
		
		static PassRefPtr<Self> create(const Point& pos, float radius, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(pos, radius, gridSize, duration));
		}
		
		float lensEffect() const {return m_lensEffect;}
		void setLensEffect(float v) {m_lensEffect = v;}
		Point position() const {return m_position;}
		void setPosition(const Point& p);
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "Lens3DAction";}
		
	protected:
		Lens3DAction(const Point& pos, float radius, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_position, m_radius, m_gridSize, m_duration);
		}
		
		
		Point	m_position;
		Point	m_positionInPixels;
		float	m_radius;
		float	m_lensEffect;
		bool	m_dirty;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Ripple3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef Ripple3DAction Self;
	public:
		virtual ~Ripple3DAction();
		
		static PassRefPtr<Self> create(const Point& pos, float radius, int waves, float amplitude, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(pos, radius, waves, amplitude, gridSize, duration));
		}
		
		Point position() const {return m_position;}
		void setPosition(const Point& p);
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float v) {m_amplitude = v;}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "Ripple3DAction";}
		
	protected:
		Ripple3DAction(const Point& pos, float radius, int waves, float amplitude, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_position, m_radius, m_waves, m_amplitude, m_gridSize, m_duration);
		}
		
		
		Point	m_position;
		Point	m_positionInPixels;
		float	m_radius;
		int m_waves;
		float	m_amplitude;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Shaky3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef Shaky3DAction Self;
	public:
		virtual ~Shaky3DAction();
		
		static PassRefPtr<Self> create(int range, bool shakeZ, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(range, shakeZ, gridSize, duration));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "Shaky3DAction";}
		
	protected:
		Shaky3DAction(int range, bool shakeZ, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_randRange, m_shakeZ, m_gridSize, m_duration);
		}
		
		
		int m_randRange;
		bool m_shakeZ;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class LiquidAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef LiquidAction Self;
	public:
		virtual ~LiquidAction();
		
		static PassRefPtr<Self> create(int waves, float amplitude, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(waves, amplitude, gridSize, duration));
		}
		
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float v) {m_amplitude = v;}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "LiquidAction";}
		
	protected:
		LiquidAction(int waves, float amplitude, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_waves, m_amplitude, m_gridSize, m_duration);
		}
		
		
		int m_waves;
		float	m_amplitude;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class WavesAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef WavesAction Self;
	public:
		virtual ~WavesAction();
		
		static PassRefPtr<Self> create(int waves, float amplitude, bool isHorizontal, bool isVertical, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(waves, amplitude, isHorizontal, isVertical, gridSize, duration));
		}
		
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float v) {m_amplitude = v;}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "WavesAction";}
		
	protected:
		WavesAction(int waves, float amplitude, bool isHorizontal, bool isVertical, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_waves, m_amplitude, m_horizontal, m_vertical, m_gridSize, m_duration);
		}
		
		
		int m_waves;
		float	m_amplitude;
		bool m_vertical;
		bool m_horizontal;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class TwirlAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef TwirlAction Self;
	public:
		virtual ~TwirlAction();
		
		static PassRefPtr<Self> create(const Point& pos, int twirls, float amplitude, const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(pos, twirls, amplitude, gridSize, duration));
		}
		
		float amplitude() const {return m_amplitude;}
		void setAmplitude(float v) {m_amplitude = v;}
		Point position() const {return m_position;}
		void setPosition(const Point& p);
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "TwirlAction";}
		
	protected:
		TwirlAction(const Point& pos, int twirls, float amplitude, const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_position, m_twirls, m_amplitude, m_gridSize, m_duration);
		}
		
		
		Point m_position;
		Point m_positionInPixels;
		int m_twirls;
		float	m_amplitude;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class PageTurn3DAction : public Grid3DAction {
		typedef Grid3DAction Base;
		typedef PageTurn3DAction Self;
	public:
		virtual ~PageTurn3DAction();
		
		static PassRefPtr<Self> create(const GridSize& gridSize, TimeSec duration)
		{
			return adoptRef(new Self(gridSize, duration));
		}
		
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "PageTurn3DAction";}
		
	protected:
		PageTurn3DAction(const GridSize& gridSize, TimeSec duration);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_gridSize, m_duration);
		}
		
	};

}

#endif
