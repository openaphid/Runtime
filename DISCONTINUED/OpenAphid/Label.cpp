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

#include "Label.h"

#include "G2DMacros.h"

namespace Aphid {
	
	///-------------------------------------------------------------------------------------------------------------------
	/// LabelTTF
	LabelTTF::LabelTTF(const AJ::UString& string, const Size& dimensions, PassRefPtr<Font> font, TextAlignment alignment, LineBreakMode lineBreakMode)
	: m_font(font)
	, m_alignment(alignment)
	, m_lineBreakMode(lineBreakMode)
	{
		LEAK_DETECT_INC("LabelTTF");
		
		m_dimensions = g2d_mult(dimensions, G2D_CONTENT_SCALE_FACTOR());
		//m_fontSize = fontSize * G2D_CONTENT_SCALE_FACTOR(); //OA: disabled, scaled in toUIFont
		
		setLabelString(string);
	}
	
	LabelTTF::~LabelTTF()
	{
		LEAK_DETECT_DEC("LabelTTF");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// LabelAtlas
	LabelAtlas::LabelAtlas(const AJ::UString& string, PassRefPtr<Texture2D> texture, size_t w, size_t h, unsigned char mapStartChar)
	: Base(texture, w, h, string.size())
	, m_mapStartChar(mapStartChar)
	{
		LEAK_DETECT_INC("LabelAtlas");
		setLabelString(string.UTF8String().data()); //TODO: optimize to avoid a string copy
	}
	
	LabelAtlas::~LabelAtlas()
	{
		LEAK_DETECT_DEC("LabelAtlas");
	}
	
	void LabelAtlas::updateAtlasValues()
	{
		size_t n = m_string.size();
		
		ccV3F_C4B_T2F_Quad quad;
		
		const unsigned char *s = (unsigned char*) m_string.UTF8String().data();
		
		Texture2D *texture = m_textureAtlas->texture();
		float textureWide = texture->pixelWidth();
		float textureHigh = texture->pixelHeight();
		
		for( size_t i=0; i<n; i++) {
			unsigned char a = s[i] - m_mapStartChar;
			float row = (a % m_itemsPerRow);
			float col = (a / m_itemsPerRow);
			
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			// Issue #938. Don't use texStepX & texStepY
			float left		= (2*row*itemWidth_+1)/(2*textureWide);
			float right		= left+(itemWidth_*2-2)/(2*textureWide);
			float top		= (2*col*itemHeight_+1)/(2*textureHigh);
			float bottom	= top+(itemHeight_*2-2)/(2*textureHigh);
#else
			float left		= row*m_itemWidth/textureWide;
			float right		= left+m_itemWidth/textureWide;
			float top		= col*m_itemHeight/textureHigh;
			float bottom	= top+m_itemHeight/textureHigh;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			
			quad.tl.texCoords.u = left;
			quad.tl.texCoords.v = top;
			quad.tr.texCoords.u = right;
			quad.tr.texCoords.v = top;
			quad.bl.texCoords.u = left;
			quad.bl.texCoords.v = bottom;
			quad.br.texCoords.u = right;
			quad.br.texCoords.v = bottom;
			
			quad.bl.vertices.x = (int) (i * m_itemWidth);
			quad.bl.vertices.y = 0;
			quad.bl.vertices.z = 0.0f;
			quad.br.vertices.x = (int)(i * m_itemWidth + m_itemWidth);
			quad.br.vertices.y = 0;
			quad.br.vertices.z = 0.0f;
			quad.tl.vertices.x = (int)(i * m_itemWidth);
			quad.tl.vertices.y = (int)(m_itemHeight);
			quad.tl.vertices.z = 0.0f;
			quad.tr.vertices.x = (int)(i * m_itemWidth + m_itemWidth);
			quad.tr.vertices.y = (int)(m_itemHeight);
			quad.tr.vertices.z = 0.0f;
			
			m_textureAtlas->updateQuad(&quad, i);
		}
		
	}
	
	void LabelAtlas::setLabelString(const AJ::UString& string)
	{
		m_string = string;
		
		size_t len = m_string.size();
		if( len > m_textureAtlas->capacity())
			m_textureAtlas->resize(len);
		
		updateAtlasValues();
		
		Size s;
		s.width = len * m_itemWidth;
		s.height = m_itemHeight;
		setContentSizeInPixels(s);
		
		setQuadsToDraw(len);
	}
}
