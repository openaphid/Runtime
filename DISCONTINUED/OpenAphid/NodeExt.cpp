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
#include "NodeExt.h"
#include "G2DMacros.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// ColorNode
	ColorNode::ColorNode()
	{
		LEAK_DETECT_INC("ColorNode");
		
		m_blendFunc = (ccBlendFunc) { CC_BLEND_SRC, CC_BLEND_DST };
		
		m_color.r = 0;
		m_color.g = 0;
		m_color.b = 0;
		m_opacity = 255;
		
		for (size_t i = 0; i < sizeof(m_squareVertices) / sizeof(m_squareVertices[0]); i++ ) {
			m_squareVertices[i].x = 0.0f;
			m_squareVertices[i].y = 0.0f;
		}
	}
	
	ColorNode::~ColorNode()
	{
		LEAK_DETECT_DEC("ColorNode");
	}
	
	void ColorNode::setContentSize(const Size &size)
	{
		m_squareVertices[1].x = size.width * G2D_CONTENT_SCALE_FACTOR();
		m_squareVertices[2].y = size.height * G2D_CONTENT_SCALE_FACTOR();
		m_squareVertices[3].x = size.width * G2D_CONTENT_SCALE_FACTOR();
		m_squareVertices[3].y = size.height * G2D_CONTENT_SCALE_FACTOR();
		
		Base::setContentSize(size);
	}
	
	void ColorNode::setColor(const ccColor3B& c)
	{
		m_color = c;
		updateColor();
	}
	
	void ColorNode::setOpacity(GLubyte opacity)
	{
		m_opacity = opacity;
		updateColor();
	}
	
	void ColorNode::setBlendFunc(ccBlendFunc func)
	{
		m_blendFunc = func;
	}
	
	void ColorNode::updateColor()
	{
		for( size_t i = 0; i < 4; i++) {
			m_squareColors[i].r = m_color.r;
			m_squareColors[i].g = m_color.g;
			m_squareColors[i].b = m_color.b;
			m_squareColors[i].a = m_opacity;
		}
	}
	
	void ColorNode::draw()
	{
		Base::draw();
		
		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		
		glVertexPointer(2, GL_FLOAT, 0, m_squareVertices);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_squareColors);
		
		
		bool newBlend = m_blendFunc.src != CC_BLEND_SRC || m_blendFunc.dst != CC_BLEND_DST;
		if( newBlend )
			glBlendFunc(m_blendFunc.src, m_blendFunc.dst );
		
		else if( m_opacity != 255 ) {
			newBlend = true;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		if( newBlend )
			glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		
		// restore default GL state
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// GradientNode
	const Vector2 GradientNode::defaultVector = Vector2Make(0, -1);
	
	GradientNode::GradientNode()
	{
		LEAK_DETECT_INC("GradientNode");
		m_endColor.r = 255;
		m_endColor.g = 255;
		m_endColor.b = 255;
		m_endOpacity = 255;
		m_startOpacity = 255;
		m_compressedInterpolation = true;
		m_vector = (Vector2){0, -1};
	}
	
	GradientNode::~GradientNode()
	{
		LEAK_DETECT_DEC("GradientNode");
	}
	
	void GradientNode::updateColor()
	{
		Base::updateColor();
		float h = m_vector.length();
    if (h == 0)
			return;
		
		double c = sqrt(2);
    Point u = PointMake(m_vector.x / h, m_vector.y / h);
		
		// Compressed Interpolation mode
		if(m_compressedInterpolation ) {
			float h2 = 1 / ( fabsf(u.x) + fabsf(u.y) );
			u = g2d_mult(u, h2 * (float)c);
		}
		
		float opacityf = (float)m_opacity/255.0f;
		
    ccColor4B S = {
			m_color.r,
			m_color.g,
			m_color.b,
			m_startOpacity * opacityf
		};
		
    ccColor4B E = {
			m_endColor.r,
			m_endColor.g,
			m_endColor.b,
			m_endOpacity * opacityf
		};
		
		
    // (-1, -1)
		m_squareColors[0].r = E.r + (S.r - E.r) * ((c + u.x + u.y) / (2.0f * c));
		m_squareColors[0].g = E.g + (S.g - E.g) * ((c + u.x + u.y) / (2.0f * c));
		m_squareColors[0].b = E.b + (S.b - E.b) * ((c + u.x + u.y) / (2.0f * c));
		m_squareColors[0].a = E.a + (S.a - E.a) * ((c + u.x + u.y) / (2.0f * c));
    // (1, -1)
		m_squareColors[1].r = E.r + (S.r - E.r) * ((c - u.x + u.y) / (2.0f * c));
		m_squareColors[1].g = E.g + (S.g - E.g) * ((c - u.x + u.y) / (2.0f * c));
		m_squareColors[1].b = E.b + (S.b - E.b) * ((c - u.x + u.y) / (2.0f * c));
		m_squareColors[1].a = E.a + (S.a - E.a) * ((c - u.x + u.y) / (2.0f * c));
		// (-1, 1)
		m_squareColors[2].r = E.r + (S.r - E.r) * ((c + u.x - u.y) / (2.0f * c));
		m_squareColors[2].g = E.g + (S.g - E.g) * ((c + u.x - u.y) / (2.0f * c));
		m_squareColors[2].b = E.b + (S.b - E.b) * ((c + u.x - u.y) / (2.0f * c));
		m_squareColors[2].a = E.a + (S.a - E.a) * ((c + u.x - u.y) / (2.0f * c));
		// (1, 1)
		m_squareColors[3].r = E.r + (S.r - E.r) * ((c - u.x - u.y) / (2.0f * c));
		m_squareColors[3].g = E.g + (S.g - E.g) * ((c - u.x - u.y) / (2.0f * c));
		m_squareColors[3].b = E.b + (S.b - E.b) * ((c - u.x - u.y) / (2.0f * c));
		m_squareColors[3].a = E.a + (S.a - E.a) * ((c - u.x - u.y) / (2.0f * c));
	}
}