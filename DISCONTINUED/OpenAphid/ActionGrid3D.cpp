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

#include "ActionGrid3D.h"

#include "G2DMacros.h"

#include <algorithm>

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// Waves3DAction
	Waves3DAction::Waves3DAction(int waves, float amplitude, const GridSize& size, TimeSec duration)
	: Base(size, duration)
	, m_waves(waves)
	, m_amplitude(amplitude)
	{
		LEAK_DETECT_INC("Waves3DAction");
	}
	
	Waves3DAction::~Waves3DAction()
	{
		LEAK_DETECT_DEC("Waves3DAction");
	}
	
	void Waves3DAction::update(TimeSec time)
	{
		int i, j;
		
		for( i = 0; i < (m_gridSize.x+1); i++ ) {
			for( j = 0; j < (m_gridSize.y+1); j++ ) {
				ccVertex3F	v = originalVertex(ccg(i,j));
				v.z += (sinf((float)M_PI*time*m_waves*2 + (v.y+v.x) * .01f) * m_amplitude * m_amplitudeRate);
				setVertex(ccg(i,j), v);
			}
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// FlipX3DAction
	FlipX3DAction::FlipX3DAction(TimeSec duration)
	: Base(ccg(1, 1), duration)
	{
		LEAK_DETECT_INC("FlipX3DAction");
	}
	
	FlipX3DAction::~FlipX3DAction()
	{
		LEAK_DETECT_DEC("FlipX3DAction");
	}
	
	void FlipX3DAction::update(TimeSec time)
	{
		float angle = (float)M_PI * time; // 180 degrees
		float mz = sinf( angle );
		angle = angle / 2.0f;     // x calculates degrees from 0 to 90
		float mx = cosf( angle );
		
		ccVertex3F	v0, v1, v, diff;
		
		v0 = originalVertex(ccg(1,1));
		v1 = originalVertex(ccg(0,0));
		
		float	x0 = v0.x;
		float	x1 = v1.x;
		float x;
		GridSize	a, b, c, d;
		
		if ( x0 > x1 ) {
			// Normal Grid
			a = ccg(0,0);
			b = ccg(0,1);
			c = ccg(1,0);
			d = ccg(1,1);
			x = x0;
		} else {
			// Reversed Grid
			c = ccg(0,0);
			d = ccg(0,1);
			a = ccg(1,0);
			b = ccg(1,1);
			x = x1;
		}
		
		diff.x = ( x - x * mx );
		diff.z = fabsf( floorf( (x * mz) / 4.0f ) );
		
		// bottom-left
		v = originalVertex(a);
		v.x = diff.x;
		v.z += diff.z;
		setVertex(a, v);
		
		// upper-left
		v = originalVertex(b);
		v.x = diff.x;
		v.z += diff.z;
		setVertex(b, v);
		
		// bottom-right
		v = originalVertex(c);
		v.x -= diff.x;
		v.z -= diff.z;
		setVertex(c, v);
		
		// upper-right
		v = originalVertex(d);
		v.x -= diff.x;
		v.z -= diff.z;
		setVertex(d, v);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// FlipY3DAction
	FlipY3DAction::FlipY3DAction(TimeSec duration)
	: Base(ccg(1, 1), duration)
	{
		LEAK_DETECT_INC("FlipY3DAction");
	}
	
	FlipY3DAction::~FlipY3DAction()
	{
		LEAK_DETECT_DEC("FlipY3DAction");
	}
	
	void FlipY3DAction::update(TimeSec time)
	{
		float angle = (float)M_PI * time; // 180 degrees
		float mz = sinf( angle );
		angle = angle / 2.0f;     // x calculates degrees from 0 to 90
		float my = cosf( angle );
		
		ccVertex3F	v0, v1, v, diff;
		
		v0 = originalVertex(ccg(1,1));
		v1 = originalVertex(ccg(0,0));
		
		float	y0 = v0.y;
		float	y1 = v1.y;
		float y;
		GridSize	a, b, c, d;
		
		if ( y0 > y1 ) {
			// Normal Grid
			a = ccg(0,0);
			b = ccg(0,1);
			c = ccg(1,0);
			d = ccg(1,1);
			y = y0;
		}	else {
			// Reversed Grid
			b = ccg(0,0);
			a = ccg(0,1);
			d = ccg(1,0);
			c = ccg(1,1);
			y = y1;
		}
		
		diff.y = y - y * my;
		diff.z = fabsf( floorf( (y * mz) / 4.0f ) );
		
		// bottom-left
		v = originalVertex(a);
		v.y = diff.y;
		v.z += diff.z;
		setVertex(a, v);
		
		// upper-left
		v = originalVertex(b);
		v.y -= diff.y;
		v.z -= diff.z;
		setVertex(b, v);
		
		// bottom-right
		v = originalVertex(c);
		v.y = diff.y;
		v.z += diff.z;
		setVertex(c, v);
		
		// upper-right
		v = originalVertex(d);
		v.y -= diff.y;
		v.z -= diff.z;
		setVertex(d, v);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Lens3DAction
	Lens3DAction::Lens3DAction(const Point& pos, float radius, const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	, m_position(PointMake(-1, -1))
	, m_radius(radius)
	, m_lensEffect(0.7f)
	, m_dirty(true)
	{
		LEAK_DETECT_INC("Lens3DAction");
		setPosition(pos);
	}
	
	Lens3DAction::~Lens3DAction()
	{
		LEAK_DETECT_DEC("Lens3DAction");
	}
	
	void Lens3DAction::setPosition(const Point &pos)
	{
		if(!PointEqual(pos, m_position)) {
			m_position = pos;
			m_positionInPixels = toPixels(m_position);
			
			m_dirty = true;
		}
	}
	
	void Lens3DAction::update(TimeSec t)
	{
		UNUSED_PARAM(t);
		
		if (m_dirty) {
			int i, j;
			
			for( i = 0; i < m_gridSize.x + 1; i++ ) {
				for( j = 0; j < m_gridSize.y + 1; j++ )	{
					ccVertex3F	v = originalVertex(ccg(i,j));
					Point vect = g2d_sub(m_positionInPixels, PointMake(v.x, v.y));
					float r = PointLength(vect);
					
					if (r < m_radius) {
						r = m_radius - r;
						float pre_log = r / m_radius;
						if ( pre_log == 0 ) pre_log = 0.001f;
						float l = logf(pre_log) * m_lensEffect;
						float new_r = expf( l ) * m_radius;
						
						if ( PointLength(vect) > 0 ) {
							vect = PointNormalize(vect);
							Point new_vect = g2d_mult(vect, new_r);
							v.z += PointLength(new_vect) * m_lensEffect;
						}
					}
					
					setVertex(ccg(i,j), v);
				}
			}
			
			m_dirty = false;
		}		
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Ripple3DAction
	Ripple3DAction::Ripple3DAction(const Point& pos, float radius, int waves, float amplitude, const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	, m_position(PointMake(-1, -1))
	, m_radius(radius)
	, m_waves(waves)
	, m_amplitude(amplitude)
	{
		LEAK_DETECT_INC("Ripple3DAction");
		setPosition(pos);
	}
	
	Ripple3DAction::~Ripple3DAction()
	{
		LEAK_DETECT_DEC("Ripple3DAction");
	}
	
	void Ripple3DAction::setPosition(const Point &pos)
	{
		m_position = pos;
		m_positionInPixels = toPixels(m_position);
	}
	
	void Ripple3DAction::update(TimeSec time)
	{
		int i, j;
		
		for(i = 0; i < m_gridSize.x + 1; i++) {
			for(j = 0; j < m_gridSize.y + 1; j++) {
				ccVertex3F	v = originalVertex(ccg(i,j));
				Point vect = g2d_sub(m_positionInPixels, PointMake(v.x, v.y));
				float r = PointLength(vect);
				
				if (r < m_radius) {
					r = m_radius - r;
					float rate = powf(r / m_radius, 2);
					v.z += (sinf(time * (float)M_PI*m_waves*2 + r * 0.1f) * m_amplitude * m_amplitudeRate * rate);
				}
				
				setVertex(ccg(i,j), v);
			}
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Shaky3DAction
	Shaky3DAction::Shaky3DAction(int range, bool shakeZ, const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	, m_randRange(range)
	, m_shakeZ(shakeZ)
	{
		LEAK_DETECT_INC("Shaky3DAction");
	}
	
	Shaky3DAction::~Shaky3DAction()
	{
		LEAK_DETECT_DEC("Shaky3DAction");
	}
	
	void Shaky3DAction::update(TimeSec t)
	{
		UNUSED_PARAM(t);
		
		int i, j;
		
		for(i = 0; i < (m_gridSize.x+1); i++ ) {
			for(j = 0; j < (m_gridSize.y+1); j++)	{
				ccVertex3F	v = originalVertex(ccg(i,j));
				v.x += ( rand() % (m_randRange*2) ) - m_randRange;
				v.y += ( rand() % (m_randRange*2) ) - m_randRange;
				if (m_shakeZ)
					v.z += ( rand() % (m_randRange*2) ) - m_randRange;
				
				setVertex(ccg(i,j), v);
			}
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// LiquidAction
	LiquidAction::LiquidAction(int waves, float amplitude, const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	, m_waves(waves)
	, m_amplitude(amplitude)
	{
		LEAK_DETECT_INC("LiquidAction");
	}
	
	LiquidAction::~LiquidAction()
	{
		LEAK_DETECT_DEC("LiquidAction");
	}
	
	void LiquidAction::update(TimeSec time)
	{
		int i, j;
		
		for( i = 1; i < m_gridSize.x; i++ ) {
			for( j = 1; j < m_gridSize.y; j++ )	{
				ccVertex3F	v = originalVertex(ccg(i,j));
				v.x = (v.x + (sinf(time*(float)M_PI*m_waves*2 + v.x * .01f) * m_amplitude * m_amplitudeRate));
				v.y = (v.y + (sinf(time*(float)M_PI*m_waves*2 + v.y * .01f) * m_amplitude * m_amplitudeRate));
				setVertex(ccg(i,j), v);
			}
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// WavesAction
	WavesAction::WavesAction(int waves, float amplitude, bool isHorizontal, bool isVertical, const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	, m_waves(waves)
	, m_amplitude(amplitude)
	, m_horizontal(isHorizontal)
	, m_vertical(isVertical)
	{
		LEAK_DETECT_INC("WavesAction");
	}
	
	WavesAction::~WavesAction()
	{
		LEAK_DETECT_DEC("WavesAction");
	}
	
	void WavesAction::update(TimeSec time)
	{
		int i, j;
		
		for(i = 0; i < m_gridSize.x + 1; i++) {
			for(j = 0; j < m_gridSize.y + 1; j++)	{
				ccVertex3F	v = originalVertex(ccg(i,j));
				
				if (m_vertical)
					v.x = (v.x + (sinf(time*(float)M_PI*m_waves*2 + v.y * .01f) * m_amplitude * m_amplitudeRate));
				
				if (m_horizontal)
					v.y = (v.y + (sinf(time*(float)M_PI*m_waves*2 + v.x * .01f) * m_amplitude * m_amplitudeRate));
				
				setVertex(ccg(i,j), v);
			}
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TwirlAction
	TwirlAction::TwirlAction(const Point& pos, int twirls, float amplitude, const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	, m_twirls(twirls)
	, m_amplitude(amplitude)
	{
		LEAK_DETECT_INC("TwirlAction");
		setPosition(pos);
	}
	
	TwirlAction::~TwirlAction()
	{
		LEAK_DETECT_DEC("TwirlAction");
	}
	
	void TwirlAction::setPosition(const Point &pos)
	{
		m_position = pos;
		m_positionInPixels = toPixels(m_position);
	}
	
	void TwirlAction::update(TimeSec time)
	{
		int i, j;
		Point	c = m_positionInPixels;
		
		for( i = 0; i < m_gridSize.x + 1; i++ ) {
			for( j = 0; j < m_gridSize.y + 1; j++)	{
				ccVertex3F v = originalVertex(ccg(i,j));
				
				Point	avg = PointMake(i-(m_gridSize.x/2.0f), j-(m_gridSize.y/2.0f));
				float r = PointLength( avg );
				
				float amp = 0.1f * m_amplitude * m_amplitudeRate;
				float a = r * cosf((float)M_PI/2.0f + time * (float)M_PI * m_twirls * 2 ) * amp;
				
				float cosA = cosf(a);
				float sinA = sinf(a);
				
				Point	d = {
					sinA * (v.y-c.y) + cosA * (v.x-c.x),
					cosA * (v.y-c.y) - sinA * (v.x-c.x)
				};
				
				v.x = c.x + d.x;
				v.y = c.y + d.y;
				
				setVertex(ccg(i,j), v);
			}
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// PageTurn3DAction
	PageTurn3DAction::PageTurn3DAction(const GridSize& gridSize, TimeSec duration)
	: Base(gridSize, duration)
	{
		LEAK_DETECT_INC("PageTurn3DAction");
	}
	
	PageTurn3DAction::~PageTurn3DAction()
	{
		LEAK_DETECT_DEC("PageTurn3DAction");
	}
	
	void PageTurn3DAction::update(TimeSec time)
	{
		float tt = std::max(0.0f, time - 0.25f );
		float deltaAy = ( tt * tt * 500);
		float ay = -100 - deltaAy;
		
		float deltaTheta = - (float) M_PI_2 * sqrtf( time) ;
		float theta = /*0.01f*/ + (float) M_PI_2 +deltaTheta;
		
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);
		
		for( int i = 0; i <= m_gridSize.x; i++ )
		{
			for( int j = 0; j <= m_gridSize.y; j++ )
			{
				// Get original vertex
				ccVertex3F	p = originalVertex(ccg(i,j));
				
				float R = sqrtf(p.x*p.x + (p.y - ay) * (p.y - ay));
				float r = R * sinTheta;
				float alpha = asinf( p.x / R );
				float beta = alpha / sinTheta;
				float cosBeta = cosf( beta );
				
				// If beta > PI then we've wrapped around the cone
				// Reduce the radius to stop these points interfering with others
				if( beta <= M_PI)
					p.x = ( r * sinf(beta));
				else
				{
					// Force X = 0 to stop wrapped
					// points
					p.x = 0;
				}
				
				p.y = ( R + ay - ( r*(1 - cosBeta)*sinTheta));
				
				// We scale z here to avoid the animation being
				// too much bigger than the screen due to perspectve transform
				p.z = (r * ( 1 - cosBeta ) * cosTheta) / 7; // "100" didn't work for
				
				// Stop z coord from dropping beneath underlying page in a transition
				// issue #751				
				if( p.z<0.5f )
					p.z = 0.5f;
				
				// Set new coords
				setVertex(ccg(i,j), p);
			}
		}
	}
}
