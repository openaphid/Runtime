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

#ifndef OpenAphid_TextureAtlas_h
#define OpenAphid_TextureAtlas_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "Texture2D.h"
#include "ccTypes.h"
#include "G2DConfig.h"

namespace Aphid {
	class JSTextureAtlas;
	
	///-------------------------------------------------------------------------------------------------------------------
	class TextureAtlas : public RefCounted<TextureAtlas>/*, public DirectWrapper<JSTextureAtlas>*/ {
	public:
		virtual ~TextureAtlas();
		
		static PassRefPtr<TextureAtlas> create(ATF::PassRefPtr<Texture2D> texture, size_t capacity)
		{
			return adoptRef(new TextureAtlas(texture, capacity));
		}
		
		void updateQuad(ccV3F_C4B_T2F_Quad* quad, size_t index);
		void insertQuad(ccV3F_C4B_T2F_Quad* quad, size_t index);
		void insertQuad(size_t from, size_t at);
		void removeQuad(size_t index);
		void removeAllQuads();
		bool resize(size_t capacity);
		void drawQuads(size_t n, size_t from=0);
		void drawQuads();
		
		size_t capacity() const {return m_capacity;}
		size_t totalQuads() const {return m_totalQuads;}
		
		Texture2D* texture() const {return m_texture.get();}
		void setTexture(PassRefPtr<Texture2D> texture) {m_texture = texture;}
		
	protected:
		TextureAtlas(PassRefPtr<Texture2D> texture, size_t capacity);
		
	private:
		void initIndices();
		
		size_t m_totalQuads;
		size_t m_capacity;
		ccV3F_C4B_T2F_Quad* m_quads;
		GLushort* m_indices;
		RefPtr<Texture2D> m_texture;
		
#if CC_USES_VBO
		GLuint m_buffersVBO[2];
		bool m_dirty;
#endif		
	}; // class TextureAtlas
	
}// namespace Aphid

#endif
