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

#ifndef OpenAphid_Label_h
#define OpenAphid_Label_h

#include "Sprite.h"
#include "AtlasNode.h"
#include "Font.h"

namespace Aphid {
	//TODO: LabelBMFont
	
	class LabelTTF : public Sprite, public LabelProtocol {
		typedef Sprite Base;
	public:
		virtual ~LabelTTF();
		
		static ATF::PassRefPtr<LabelTTF> create(const AJ::UString& string, const Size& dimensions, PassRefPtr<Font> font, TextAlignment alignment, LineBreakMode lineBreakMode)
		{
			return ATF::adoptRef(new LabelTTF(string, dimensions, font, alignment, lineBreakMode));
		}
		
		virtual void setLabelString(const AJ::UString& string);
		virtual AJ::UString labelString() const {return m_string;}
		
	protected:
		LabelTTF(const AJ::UString& string, const Size& dimensions, PassRefPtr<Font> font, TextAlignment alignment, LineBreakMode lineBreakMode);
		
		AJ::UString m_string;
		Size m_dimensions;
		RefPtr<Font> m_font;
		TextAlignment m_alignment;
		LineBreakMode m_lineBreakMode;
	};
	
	class LabelAtlas : public AtlasNode, public LabelProtocol {
		typedef AtlasNode Base;
	public:
		virtual ~LabelAtlas();
		
		virtual void setLabelString(const AJ::UString& string);
		virtual AJ::UString labelString() const {return m_string;}
		
		virtual void updateAtlasValues();
		
	protected:
		LabelAtlas(const AJ::UString& string, PassRefPtr<Texture2D> texture, size_t w, size_t h, unsigned char mapStartChar);
		
		AJ::UString m_string;
		unsigned char m_mapStartChar;
	};
	
}

#endif
