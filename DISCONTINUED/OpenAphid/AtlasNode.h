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

#ifndef OpenAphid_AtlasNode_h
#define OpenAphid_AtlasNode_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "Node.h"
#include "TextureAtlas.h"

namespace Aphid {
	class AtlasNode : public Node, public RGBAProtocol, public TextureProtocol {
		typedef Node Base;
		typedef AtlasNode Self;
	public:
		virtual ~AtlasNode();
		
		static PassRefPtr<Self> create(PassRefPtr<Texture2D> texture, size_t width, size_t height, size_t c)
		{
			return adoptRef(new Self(texture, width, height, c));
		}
		
		virtual void draw();
		
		virtual RGBAProtocol* toRGBAProtocol() {return this;}
		virtual TextureProtocol* toTextureProtocol() {return this;}
		
		//RGBAProtocol
		virtual void setColor(const ccColor3B& c);
		virtual ccColor3B color() const;
		virtual GLubyte opacity() const {return m_opacity;}
		virtual void setOpacity(GLubyte opacity);
		virtual void setOpacityModifyRGB(bool modify);		
		virtual bool doesOpacityModifyRGB() {return m_opacityModifyRGB;}
		
		//TextureProtocol
		virtual Texture2D* texture() const {return m_textureAtlas->texture();}
		virtual void setTexture(PassRefPtr<Texture2D> texture);
		
		//Getter/setter
		void setQuadsToDraw(size_t v) {m_quadsToDraw = v;}

	protected:
		//different from CC, uses a texture instead of a filename
		AtlasNode(PassRefPtr<Texture2D> texture, size_t tileWidth, size_t tileHeight, size_t capacity);
		
		virtual void updateAtlasValues() {}
		
		void calculateMaxItems();
		void updateBlendFunc();
		void updateOpacityModifyRGB();

		RefPtr<TextureAtlas> m_textureAtlas;
		// chars per row
		size_t m_itemsPerRow;
		// chars per column
		size_t m_itemsPerColumn;

		// width of each char
		size_t m_itemWidth;
		// height of each char
		size_t m_itemHeight;

		// quads to draw
		size_t m_quadsToDraw;

		// blend function
		ccBlendFunc m_blendFunc;

		// texture RGBA. 
		GLubyte m_opacity;
		ccColor3B m_color;
		ccColor3B m_colorUnmodified;
		bool m_opacityModifyRGB;
	};
}

#endif
