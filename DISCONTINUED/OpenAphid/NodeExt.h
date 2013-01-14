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

#ifndef OpenAphid_NodeExt_h
#define OpenAphid_NodeExt_h

#include "Node.h"
#include "Color.h"

namespace Aphid {
	class ColorNode : public Node, public RGBAProtocol, public BlendProtocol {
		typedef Node Base;
		typedef ColorNode Self;
	public:
		virtual ~ColorNode();
		
		static ATF::PassRefPtr<Self> create(RGBA color, const Size& size=SizeZero)
		{
			RefPtr<ColorNode> ret = ATF::adoptRef(new ColorNode());
			ret->setRGBA(color);
			ret->setContentSize(size);
			return ret.release();
		}
		
		virtual void setColor(const ccColor3B& c);
		virtual ccColor3B color() const {return m_color;}
		virtual GLubyte opacity() const {return m_opacity;}
		virtual void setOpacity(GLubyte opacity);
		
		virtual void setBlendFunc(ccBlendFunc func);
		virtual ccBlendFunc blendFunc() {return m_blendFunc;}
		
		virtual void draw();
		virtual void setContentSize(const Size &s);
		
		virtual RGBAProtocol* toRGBAProtocol() {return this;}
		
		void setRGBA(RGBA color)
		{
			m_color = toCCColor3B(color);
			m_opacity = Color::alpha(color);
			updateColor();
		}
		
	protected:
		ColorNode();
		
		virtual void updateColor();
		
		GLubyte		m_opacity;
		ccColor3B	m_color;	
		ccVertex2F	m_squareVertices[4];
		ccColor4B	m_squareColors[4];
		
		ccBlendFunc	m_blendFunc;
	};
	
	class GradientNode : public ColorNode { //TODO: GradientNode should not be inheritated from ColorNode
		typedef ColorNode Base;
		typedef GradientNode Self;
	public:
		virtual ~GradientNode();
		
		static const Vector2 defaultVector;
		
		static PassRefPtr<Self> create(RGBA startColor, RGBA endColor, const Vector2& v=defaultVector, const Size& size=SizeZero)
		{
			RefPtr<GradientNode> ret = adoptRef(new GradientNode());
			ret->setGradientColors(startColor, endColor, v);
			ret->setContentSize(size);
			return ret.release();
		}
		
		void setGradientColors(RGBA startColor, RGBA endColor, const Vector2& v=Vector2Make(0, -1))
		{
			m_color = toCCColor3B(startColor);
			m_startOpacity = Color::alpha(startColor);
			m_endColor = toCCColor3B(endColor);
			m_endOpacity = Color::alpha(endColor);
			m_vector = v;
			updateColor();
		}
		
		RGBA startRGBA() const {return toRGBA(m_color, m_startOpacity);}
		void setStartRGBA(RGBA c) 
		{
			m_color = toCCColor3B(c);
			m_startOpacity = Color::alpha(c);
			updateColor();
		}
		
		RGBA endRGBA() const {return toRGBA(m_endColor, m_endOpacity);}
		void setEndRGBA(RGBA c) 
		{
			m_endColor = toCCColor3B(c);
			m_endOpacity = Color::alpha(c);
			updateColor();
		}
		
		Vector2 vector() const {return m_vector;}
		void setVector(const Vector2& v) 
		{
			m_vector = v;
			updateColor();
		}
		
		bool isCompressedInterpolation() const {return m_compressedInterpolation;}
		void setCompressedInterpolation(bool b) 
		{
			m_compressedInterpolation = b;
			updateColor();
		}
		
	protected:
		GradientNode();
		
		virtual void updateColor();
		
		ccColor3B m_endColor;
		
		GLubyte m_startOpacity;
		GLubyte m_endOpacity;
		
		Vector2 m_vector;
		bool m_compressedInterpolation;
	}; 
	
	///TODO:
	class MultiplexNode : public Node {
		
	};
}

#endif
