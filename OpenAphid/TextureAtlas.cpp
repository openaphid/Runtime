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
#include "TextureAtlas.h"

#include "OAUtil.h"

#include "ccTypes.h"

namespace Aphid {
  ///-------------------------------------------------------------------------------------------------------------------
  TextureAtlas::TextureAtlas(PassRefPtr<Texture2D> texture, size_t capacity)
  : m_capacity(capacity)
  , m_totalQuads(0)
  , m_texture(texture)
  {
	  LEAK_DETECT_INC("TextureAtlas");

	  m_quads = (ccV3F_C4B_T2F_Quad *) calloc(sizeof(ccV3F_C4B_T2F_Quad) * m_capacity, 1);
	  m_indices = (GLushort *) calloc(sizeof(GLushort) * m_capacity * 6, 1);

	  if (!m_quads || !m_indices) {
		  oa_error("Failed to create quads and indices for TextureAtlas: %d", capacity);
		  CRASH();
	  }

#if CC_USES_VBO
		// initial binding
		glGenBuffers(2, &m_buffersVBO[0]);	
		m_dirty = true;
#endif

	  initIndices();
  }

  TextureAtlas::~TextureAtlas()
  {
	  LEAK_DETECT_DEC("TextureAtlas");
	  OA_FREE(m_quads);
	  OA_FREE(m_indices);

#if CC_USES_VBO
		glDeleteBuffers(2, m_buffersVBO);
#endif
  }

  void TextureAtlas::initIndices()
  {
	  for (size_t i = 0; i < m_capacity; i++) {
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
			m_indices[i*6+0] = i*4+0;
			m_indices[i*6+1] = i*4+0;
			m_indices[i*6+2] = i*4+2;		
			m_indices[i*6+3] = i*4+1;
			m_indices[i*6+4] = i*4+3;
			m_indices[i*6+5] = i*4+3;
#else
		  m_indices[i * 6 + 0] = i * 4 + 0;
		  m_indices[i * 6 + 1] = i * 4 + 1;
		  m_indices[i * 6 + 2] = i * 4 + 2;

		  // inverted index. issue #179
		  m_indices[i * 6 + 3] = i * 4 + 3;
		  m_indices[i * 6 + 4] = i * 4 + 2;
		  m_indices[i * 6 + 5] = i * 4 + 1;
		  //		m_indices[i*6+3] = i*4+2;
		  //		m_indices[i*6+4] = i*4+3;
		  //		m_indices[i*6+5] = i*4+1;
#endif
	  }

#if CC_USES_VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_quads[0]) * m_capacity, m_quads, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffersVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_capacity * 6, m_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif // CC_USES_VBO
  }

  void TextureAtlas::updateQuad(ccV3F_C4B_T2F_Quad *quad, size_t index)
  {
	  ASSERT(index < m_capacity);

	  m_totalQuads = std::max(index + 1, m_totalQuads);

	  m_quads[index] = *quad;

#if CC_USES_VBO
		m_dirty = true;
#endif
  }

  void TextureAtlas::insertQuad(ccV3F_C4B_T2F_Quad *quad, size_t index)
  {
	  ASSERT(index < m_capacity);

	  m_totalQuads++;
	  ASSERT(m_totalQuads <= m_capacity);

	  int remaining = (m_totalQuads - 1) - index;

	  // last object doesn't need to be moved
	  if (remaining > 0)
		  // tex coordinates
		  memmove(&m_quads[index + 1], &m_quads[index], sizeof(m_quads[0]) * remaining);

	  m_quads[index] = *quad;

#if CC_USES_VBO
		m_dirty = true;
#endif
  }

  void TextureAtlas::insertQuad(size_t oldIndex, size_t newIndex)
  {
	  ASSERT(oldIndex < m_totalQuads && newIndex < m_totalQuads);

	  if (oldIndex == newIndex)
		  return;

	  size_t howMany = labs(oldIndex - newIndex);
	  size_t dst = oldIndex;
	  size_t src = oldIndex + 1;
	  if (oldIndex > newIndex) {
		  dst = newIndex + 1;
		  src = newIndex;
	  }

	  // tex coordinates
	  ccV3F_C4B_T2F_Quad quadsBackup = m_quads[oldIndex];
	  memmove(&m_quads[dst], &m_quads[src], sizeof(m_quads[0]) * howMany);
	  m_quads[newIndex] = quadsBackup;

#if CC_USES_VBO
		m_dirty = true;
#endif
  }

  void TextureAtlas::removeQuad(size_t index)
  {
	  ASSERT(index < m_totalQuads);

	  size_t remaining = (m_totalQuads - 1) - index;


	  // last object doesn't need to be moved
	  if (remaining)
		  // tex coordinates
		  memmove(&m_quads[index], &m_quads[index + 1], sizeof(m_quads[0]) * remaining);

	  m_totalQuads--;

#if CC_USES_VBO
		m_dirty = true;
#endif
  }

  void TextureAtlas::removeAllQuads()
  {
	  m_totalQuads = 0;
  }

  bool TextureAtlas::resize(size_t newCapacity)
  {
	  if (newCapacity == m_capacity)
		  return true;

	  // update capacity and totolQuads
	  m_totalQuads = std::min(m_totalQuads, newCapacity);
	  m_capacity = newCapacity;

	  void *tmpQuads = realloc(m_quads, sizeof(m_quads[0]) * m_capacity);
	  void *tmpIndices = realloc(m_indices, sizeof(m_indices[0]) * m_capacity * 6);

	  if (!(tmpQuads && tmpIndices)) {
		  oa_warn("TextureAtlas: not enough memory");
		  OA_FREE(tmpQuads);
		  OA_FREE(m_quads);
		  OA_FREE(tmpIndices);
		  OA_FREE(m_indices);

		  m_capacity = m_totalQuads = 0;
		  return false;
	  }

	  m_quads = (ccV3F_C4B_T2F_Quad *) tmpQuads;
	  m_indices = (GLushort *) tmpIndices;

	  initIndices();

#if CC_USES_VBO
		m_dirty = true;
#endif
	  return true;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // drawing
  void TextureAtlas::drawQuads()
  {
	  drawQuads(m_totalQuads, 0);
  }

  void TextureAtlas::drawQuads(size_t n, size_t start)
  {
	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Unneeded states: -

	  glBindTexture(GL_TEXTURE_2D, m_texture->name());
#define kQuadSize sizeof(m_quads[0].bl)
#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
		
		// TODO: update is done in draw... perhaps it should be done in a timer
		if (m_dirty) {
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_quads[0])*start, sizeof(m_quads[0]) * n , &m_quads[start] );
			m_dirty = false;
		}
		
		// vertices
		glVertexPointer(3, GL_FLOAT, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, vertices));
		
		// colors
		glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, colors));
		
		// tex coords
		glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, texCoords));
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffersVBO[1]);
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_indices[0])) );
#else
		glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_indices[0])) );
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#else // ! CC_USES_VBO

	  size_t offset = (size_t) m_quads;
	  // vertex
	  size_t diff = offsetof( ccV3F_C4B_T2F, vertices);
	  glVertexPointer(3, GL_FLOAT, kQuadSize, (GLvoid *) (offset + diff));
	  // color
	  diff = offsetof( ccV3F_C4B_T2F, colors);
	  glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (GLvoid *) (offset + diff));

	  // tex coords
	  diff = offsetof( ccV3F_C4B_T2F, texCoords);
	  glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid *) (offset + diff));

#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
		glDrawElements(GL_TRIANGLE_STRIP, n*6, GL_UNSIGNED_SHORT, m_indices + start * 6 );
#else
	  glDrawElements(GL_TRIANGLES, n * 6, GL_UNSIGNED_SHORT, m_indices + start * 6);
#endif

#endif // CC_USES_VBO
  }

} //namespace Aphid
