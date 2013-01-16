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

#include "AtlasNode.h"

#include "G2DMacros.h"

namespace Aphid {
  AtlasNode::AtlasNode(PassRefPtr<Texture2D> texture, size_t w, size_t h, size_t capacity)
  {
	  LEAK_DETECT_INC("AtlasNode");

	  m_itemWidth = w * G2D_CONTENT_SCALE_FACTOR();
	  m_itemHeight = h * G2D_CONTENT_SCALE_FACTOR();

	  m_opacity = 255;
	  m_color = m_colorUnmodified = ccWHITE;
	  m_opacityModifyRGB = true;

	  m_blendFunc.src = CC_BLEND_SRC;
	  m_blendFunc.dst = CC_BLEND_DST;

	  m_textureAtlas = TextureAtlas::create(texture, capacity);

	  updateBlendFunc();
	  updateOpacityModifyRGB();

	  calculateMaxItems();

	  m_quadsToDraw = capacity;
  }

  AtlasNode::~AtlasNode()
  {
	  LEAK_DETECT_DEC("AtlasNode");
  }

  void AtlasNode::calculateMaxItems()
  {
	  Size s = m_textureAtlas->texture()->contentSizeInPixels();
	  m_itemsPerColumn = s.height / m_itemHeight;
	  m_itemsPerRow = s.width / m_itemWidth;
  }

  void AtlasNode::updateBlendFunc()
  {
	  if (!m_textureAtlas->texture()->hasPremultipliedAlpha()) {
		  m_blendFunc.src = GL_SRC_ALPHA;
		  m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	  }
  }

  void AtlasNode::draw()
  {
	  Base::draw();

	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Unneeded states: GL_COLOR_ARRAY
	  glDisableClientState(GL_COLOR_ARRAY);

	  glColor4ub(m_color.r, m_color.g, m_color.b, m_opacity);

	  bool newBlend = m_blendFunc.src != CC_BLEND_SRC || m_blendFunc.dst != CC_BLEND_DST;
	  if (newBlend)
		  glBlendFunc(m_blendFunc.src, m_blendFunc.dst);

	  m_textureAtlas->drawQuads(m_quadsToDraw, 0);

	  if (newBlend)
		  glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	  // is this chepear than saving/restoring color state ?
	  // TODO: There is no need to restore the color to (255,255,255,255). Objects should use the color
	  // TODO: that they need
	  //	glColor4ub( 255, 255, 255, 255);

	  // restore default GL state
	  glEnableClientState(GL_COLOR_ARRAY);
  }

  //--------------------------------------------------------------------------------------------------------------------
  // RGBAProtocol
  ccColor3B AtlasNode::color() const
  {
	  if (m_opacityModifyRGB)
		  return m_colorUnmodified;
	  return m_color;
  }

  void AtlasNode::setColor(const ccColor3B &color3)
  {
	  m_color = m_colorUnmodified = color3;

	  if (m_opacityModifyRGB) {
		  m_color.r = color3.r * m_opacity / 255;
		  m_color.g = color3.g * m_opacity / 255;
		  m_color.b = color3.b * m_opacity / 255;
	  }
  }

  void AtlasNode::setOpacity(GLubyte opacity)
  {
	  m_opacity = opacity;

	  // special opacity for premultiplied textures
	  if (m_opacityModifyRGB)
		  setColor(m_colorUnmodified);
  }

  void AtlasNode::setOpacityModifyRGB(bool modify)
  {
	  ccColor3B oldColor = color();
	  m_opacityModifyRGB = modify;
	  setColor(oldColor);
  }

  void AtlasNode::updateOpacityModifyRGB()
  {
	  m_opacityModifyRGB = m_textureAtlas->texture()->hasPremultipliedAlpha();
  }

  //--------------------------------------------------------------------------------------------------------------------
  // TextureProtocol
  void AtlasNode::setTexture(PassRefPtr<Aphid::Texture2D> texture)
  {
	  m_textureAtlas->setTexture(texture);
	  updateBlendFunc();
	  updateOpacityModifyRGB();
  }
}
