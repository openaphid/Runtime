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
#include "Sprite.h"

#include "OAUtil.h"

#include "G2DMacros.h"
#include "TextureAtlas.h"
#include "SpriteBatchNode.h"
#include "AJSprite.h"
#include "AJTexture2D.h"
#include "glu.h"
#include "OpenGL_Internal.h"

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__A__) ( (int)(__A__))
#endif

namespace Aphid {
  ///-------------------------------------------------------------------------------------------------------------------
  Sprite::Sprite()
  : m_opacityModifyRGB(true)
  , m_opacity(255)
  , m_color(ccWHITE)
  , m_colorUnmodified(ccWHITE)
  , m_flipX(false)
  , m_flipY(false)
  , m_offsetPositionInPixels(PointZero)
	, m_unflippedOffsetPositionFromCenter(PointZero)
  , m_honorParentTransform(G2D_HONOR_PARENT_TRANSFORM_ALL)
  , m_hasChildren(false)
  {
	  LEAK_DETECT_INC("Sprite");
		
	  useSelfRender();
		
	  m_blendFunc.src = CC_BLEND_SRC;
	  m_blendFunc.dst = CC_BLEND_DST;
		
	  // update texture (calls updateBlendFunc)
	  setTexture(0);
		
	  // clean the Quad
	  bzero(&m_quad, sizeof(m_quad));
		
	  // default transform anchor: center
	  m_anchor = PointMake(0.5f, 0.5f);
		
	  // Atlas: Color
	  ccColor4B tmpColor = {255, 255, 255, 255};
	  m_quad.bl.colors = tmpColor;
	  m_quad.br.colors = tmpColor;
	  m_quad.tl.colors = tmpColor;
	  m_quad.tr.colors = tmpColor;
		
	  // Atlas: TexCoords
	  setTextureRectInPixels(RectZero, false, SizeZero);
		
	  // updateMethod selector
	  // No need in OA
	  // m_updateMethod = (__typeof__(updateMethod))[self methodForSelector:@selector(updateTransform)];
		
#if OA_NATIVE_BENCHMARK
		if ((arc4random() % 2) == 0)
			_vx = 0.1;
		else
			_vx = -0.1;
		
		if ((arc4random() % 2) == 0)
			_vy = 0.1;
		else
			_vy = -0.1;
#endif
  }
	
  Sprite::~Sprite()
  {
	  LEAK_DETECT_DEC("Sprite");
  }
	
#if OA_NATIVE_BENCHMARK
	//for Native Benchmark
	void Sprite::update(float interval)
	{
		Point p = m_position;
		p.x += interval * 1000 * _vx;
		p.y += interval * 1000 * _vy;
		
		Size size = SizeMake(320, 480);
		
		if (p.x < 0) {
			p.x = 0;
			_vx = -_vx;
		}
		
		if (p.x > size.width) {
			p.x = size.width;
			_vx = -_vx;
		}
		
		if (p.y < 0) {
			p.y = 0;
			_vy = -_vy;
		}
		
		if (p.y > size.height) {
			p.y = size.height;
			_vy = -_vy;
		}
		
		setPosition(p);
	}
	
	void Sprite::handleTouchEvent(Aphid::EventFlag flag, const PlatformTouchVector &touches, Aphid::PlatformTouchEvent *event)
	{
		if (flag == EventFlagTouchMove) {
			if (!touches.isEmpty()) {
				PlatformTouch* touch = touches.at(0).get();
				Point p = touch->clientLocation();
				if (parent())
					setPosition(parent()->convertToNodeSpace(p));
			}
		}
		
		Base::handleTouchEvent(flag, touches, event);
	}	
#endif
	
  PassRefPtr<Sprite> Sprite::create(PassRefPtr<Texture2D> prpTexture)
  {
	  RefPtr<Texture2D> texture = prpTexture;
		if (texture) {
			Rect r = RectZero;
			r.size = texture->contentSize();
			return create(texture.release(), r);
		}else 
			return create();
  }
	
  PassRefPtr<Sprite> Sprite::create(PassRefPtr<Texture2D> texture, const Rect& rect)
  {
	  RefPtr<Sprite> sprite = create();
	  sprite->setTexture(texture);
	  sprite->setTextureRect(rect);
	  return sprite.release();
  }
	
  PassRefPtr<Sprite> Sprite::createWithFrame(PassRefPtr<SpriteFrame> frame)
  {
	  RefPtr<Sprite> sprite = create();
	  sprite->setTexture(frame->texture());
	  sprite->setTextureRect(frame->rect());
	  sprite->setDisplayFrame(frame);
	  return sprite.release();
  }
	
  void Sprite::useSelfRender()
  {
	  m_atlasIndex = G2DSpriteIndexNotInitialized;
	  m_usesBatchNode = false;
	  m_textureAtlas = 0;
	  m_batchNode = 0;
	  m_dirty = false;
	  m_recursiveDirty = false;
		
	  float x1 = 0 + m_offsetPositionInPixels.x;
	  float y1 = 0 + m_offsetPositionInPixels.y;
	  float x2 = x1 + m_rectInPixels.size.width;
	  float y2 = y1 + m_rectInPixels.size.height;
	  m_quad.bl.vertices = (ccVertex3F) {x1, y1, 0};
	  m_quad.br.vertices = (ccVertex3F) {x2, y1, 0};
	  m_quad.tl.vertices = (ccVertex3F) {x1, y2, 0};
	  m_quad.tr.vertices = (ccVertex3F) {x2, y2, 0};
  }
	
  void Sprite::useBatchNode(Aphid::SpriteBatchNode *batchNode)
  {
	  ASSERT(batchNode);
	  m_usesBatchNode = true;
	  m_textureAtlas = batchNode->textureAtlas();
	  m_batchNode = batchNode;
  }
	
  void Sprite::setTextureRect(const Rect& r)
  {
	  Rect inPixels = toPixels(r);
	  setTextureRectInPixels(inPixels, false, inPixels.size);
  }
	
  void Sprite::setTextureRectInPixels(const Rect& rect, bool rotated, const Size& untrimmedSize)
  {
	  m_rectInPixels = rect;
	  m_rect = toPoints(rect);
	  m_rectRotated = rotated;
		
	  
	  updateTextureCoords(m_rectInPixels);
		Base::setContentSizeInPixels(untrimmedSize);
		
		Point relativeOffsetInPixels = m_unflippedOffsetPositionFromCenter;
		
	  // issue #732
	  if (m_flipX)
		  relativeOffsetInPixels.x = -relativeOffsetInPixels.x;
	  if (m_flipY)
		  relativeOffsetInPixels.y = -relativeOffsetInPixels.y;
		
	  m_offsetPositionInPixels.x = relativeOffsetInPixels.x + (m_contentSizeInPixels.width - m_rectInPixels.size.width) / 2;
	  m_offsetPositionInPixels.y = relativeOffsetInPixels.y + (m_contentSizeInPixels.height - m_rectInPixels.size.height) / 2;
		
	  if (m_usesBatchNode) {
		  m_dirty = true;
	  } else {
		  // Atlas: Vertex
		  float x1 = 0 + m_offsetPositionInPixels.x;
		  float y1 = 0 + m_offsetPositionInPixels.y;
		  float x2 = x1 + m_rectInPixels.size.width;
		  float y2 = y1 + m_rectInPixels.size.height;
			
		  // Don't update Z.
		  m_quad.bl.vertices = (ccVertex3F) {x1, y1, 0};
		  m_quad.br.vertices = (ccVertex3F) {x2, y1, 0};
		  m_quad.tl.vertices = (ccVertex3F) {x1, y2, 0};
		  m_quad.tr.vertices = (ccVertex3F) {x2, y2, 0};
	  }
		
  }
	
	void Sprite::setContentSizeInPixels(const Size &s) //different impl from CC here, TODO: is there any side-effect?
	{
		if (!m_texture || !m_texture->name())
			setTextureRectInPixels(RectMake2(PointZero, s), false, s);
		else
			Base::setContentSizeInPixels(s);
	}
	
  void Sprite::updateTextureCoords(const Rect& rect)
  {
	  Texture2D *tex = m_usesBatchNode ? m_textureAtlas->texture() : m_texture.get();
	  if (!tex)
		  return;
		
	  float atlasWidth = (float) tex->pixelWidth();
	  float atlasHeight = (float) tex->pixelHeight();
		
	  float left, right, top, bottom;
		
	  if (m_rectRotated) {
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			left	= (2*rect.origin.x+1)/(2*atlasWidth);
			right	= left+(rect.size.height*2-2)/(2*atlasWidth);
			top		= (2*rect.origin.y+1)/(2*atlasHeight);
			bottom	= top+(rect.size.width*2-2)/(2*atlasHeight);
#else
		  left = rect.origin.x / atlasWidth;
		  right = left + (rect.size.height / atlasWidth);
		  top = rect.origin.y / atlasHeight;
		  bottom = top + (rect.size.width / atlasHeight);
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			
		  if (m_flipX)
				CC_SWAP(top, bottom);
		  if (m_flipY)
				CC_SWAP(left, right);
			
		  m_quad.bl.texCoords.u = left;
		  m_quad.bl.texCoords.v = top;
		  m_quad.br.texCoords.u = left;
		  m_quad.br.texCoords.v = bottom;
		  m_quad.tl.texCoords.u = right;
		  m_quad.tl.texCoords.v = top;
		  m_quad.tr.texCoords.u = right;
		  m_quad.tr.texCoords.v = bottom;
	  } else {
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			left	= (2*rect.origin.x+1)/(2*atlasWidth);
			right	= left + (rect.size.width*2-2)/(2*atlasWidth);
			top		= (2*rect.origin.y+1)/(2*atlasHeight);
			bottom	= top + (rect.size.height*2-2)/(2*atlasHeight);
#else
		  left = rect.origin.x / atlasWidth;
		  right = left + rect.size.width / atlasWidth;
		  top = rect.origin.y / atlasHeight;
		  bottom = top + rect.size.height / atlasHeight;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			
		  if (m_flipX)
				CC_SWAP(left, right);
		  if (m_flipY)
				CC_SWAP(top, bottom);
			
		  m_quad.bl.texCoords.u = left;
		  m_quad.bl.texCoords.v = bottom;
		  m_quad.br.texCoords.u = right;
		  m_quad.br.texCoords.v = bottom;
		  m_quad.tl.texCoords.u = left;
		  m_quad.tl.texCoords.v = top;
		  m_quad.tr.texCoords.u = right;
		  m_quad.tr.texCoords.v = top;
	  }
  }
	
  void Sprite::updateTransform()
  {
	  ASSERT(m_usesBatchNode);
		
	  if (!m_dirty)
		  return;
		
	  AffineTransform matrix;
		
	  // Optimization: if it is not visible, then do nothing
	  if (!m_visible) {
		  m_quad.br.vertices = m_quad.tl.vertices = m_quad.tr.vertices = m_quad.bl.vertices = (ccVertex3F) {0, 0, 0};
		  m_textureAtlas->updateQuad(&m_quad, m_atlasIndex);
		  m_dirty = m_recursiveDirty = false;
		  return;
	  }
		
		
	  // Optimization: If parent is batchnode, or parent is nil
	  // build Affine transform manually
	  if (!m_parent || m_parent == m_batchNode) {
			
		  float radians = -CC_DEGREES_TO_RADIANS(m_rotation);
		  float c = cosf(radians);
		  float s = sinf(radians);
			
		  matrix = AffineTransformMake( c * m_scale.x, s * m_scale.x,
																	 -s * m_scale.y, c * m_scale.y,
																	 m_positionInPixels.x, m_positionInPixels.y);
		  if (m_skew.x || m_skew.y) {
			  AffineTransform skewMatrix = AffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_skew.y)),
																												 tanf(CC_DEGREES_TO_RADIANS(m_skew.x)), 1.0f,
																												 0.0f, 0.0f);
			  matrix = AffineTransformConcat(skewMatrix, matrix);
		  }
		  matrix = AffineTransformTranslate(matrix, -m_anchorInPixels.x, -m_anchorInPixels.y);
			
			
	  } else {   // parent_ != batchNode_
			
		  // else do affine transformation according to the HonorParentTransform
			
		  matrix = AffineTransformIdentity;
		  G2DHonorParentTransform prevHonor = G2D_HONOR_PARENT_TRANSFORM_ALL;
			
		  for (Node *p = this; p && p != m_batchNode; p = p->parent()) {
				
			  // Might happen. Issue #1053
			  ASSERT(p->toSprite());
				
			  struct transformValues_ tv;
			  p->toSprite()->getTransformValues(&tv);
				
			  // If any of the parents are not visible, then don't draw this node
			  if (!tv.visible) {
				  m_quad.br.vertices = m_quad.tl.vertices = m_quad.tr.vertices = m_quad.bl.vertices = (ccVertex3F) {0, 0, 0};
				  m_textureAtlas->updateQuad(&m_quad, m_atlasIndex);
				  m_dirty = m_recursiveDirty = false;
				  return;
			  }
			  AffineTransform newMatrix = AffineTransformIdentity;
				
			  // 2nd: Translate, Skew, Rotate, Scale
			  if (prevHonor & G2D_HONOR_PARENT_TRANSFORM_TRANSLATE)
				  newMatrix = AffineTransformTranslate(newMatrix, tv.pos.x, tv.pos.y);
			  if (prevHonor & G2D_HONOR_PARENT_TRANSFORM_ROTATE)
				  newMatrix = AffineTransformRotate(newMatrix, -CC_DEGREES_TO_RADIANS(tv.rotation));
			  if (prevHonor & G2D_HONOR_PARENT_TRANSFORM_SKEW) {
				  AffineTransform skew = AffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(tv.skew.y)), tanf(CC_DEGREES_TO_RADIANS(tv.skew.x)), 1.0f, 0.0f, 0.0f);
				  // apply the skew to the transform
				  newMatrix = AffineTransformConcat(skew, newMatrix);
			  }
			  if (prevHonor & G2D_HONOR_PARENT_TRANSFORM_SCALE) {
				  newMatrix = AffineTransformScale(newMatrix, tv.scale.x, tv.scale.y);
			  }
				
			  // 3rd: Translate anchor point
			  newMatrix = AffineTransformTranslate(newMatrix, -tv.ap.x, -tv.ap.y);
				
			  // 4th: Matrix multiplication
			  matrix = AffineTransformConcat( matrix, newMatrix);
				
			  prevHonor = p->toSprite()->honorParentTransform();
		  }
	  }
		
		
	  //
	  // calculate the Quad based on the Affine Matrix
	  //
		
	  Size size = m_rectInPixels.size;
		
	  float x1 = m_offsetPositionInPixels.x;
	  float y1 = m_offsetPositionInPixels.y;
		
	  float x2 = x1 + size.width;
	  float y2 = y1 + size.height;
	  float x = matrix.tx;
	  float y = matrix.ty;
		
	  float cr = matrix.a;
	  float sr = matrix.b;
	  float cr2 = matrix.d;
	  float sr2 = -matrix.c;
	  float ax = x1 * cr - y1 * sr2 + x;
	  float ay = x1 * sr + y1 * cr2 + y;
		
	  float bx = x2 * cr - y1 * sr2 + x;
	  float by = x2 * sr + y1 * cr2 + y;
		
	  float cx = x2 * cr - y2 * sr2 + x;
	  float cy = x2 * sr + y2 * cr2 + y;
		
	  float dx = x1 * cr - y2 * sr2 + x;
	  float dy = x1 * sr + y2 * cr2 + y;
		
	  m_quad.bl.vertices = (ccVertex3F) {RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), m_vertexZ};
	  m_quad.br.vertices = (ccVertex3F) {RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), m_vertexZ};
	  m_quad.tl.vertices = (ccVertex3F) {RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), m_vertexZ};
	  m_quad.tr.vertices = (ccVertex3F) {RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), m_vertexZ};
		
	  m_textureAtlas->updateQuad(&m_quad, m_atlasIndex);
	  m_dirty = m_recursiveDirty = false;
  }
	
  void Sprite::getTransformValues(struct transformValues_ *tv)
  {
	  tv->pos = m_positionInPixels;
	  tv->scale = m_scale;
	  tv->rotation = m_rotation;
	  tv->skew = m_skew;
	  tv->ap = m_anchorInPixels;
	  tv->visible = m_visible;
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Draw
	bool Sprite::s_debug_draw_outline = false;
	bool Sprite::s_debug_draw_texture_outline = false;
	
  void Sprite::draw()
  {
	  Base::draw();
	  ASSERT(!m_usesBatchNode);
		
	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Unneeded states: -
		
		if (m_texture) {
			
			bool newBlend = m_blendFunc.src != CC_BLEND_SRC || m_blendFunc.dst != CC_BLEND_DST;
			if (newBlend)
				glBlendFunc(m_blendFunc.src, m_blendFunc.dst);
			
#define kQuadSize sizeof(m_quad.bl)		
			glBindTexture(GL_TEXTURE_2D, m_texture ? m_texture->name() : 0);
			
			long offset = (long) &m_quad;
			
			// vertex
			int diff = offsetof( ccV3F_C4B_T2F, vertices);
			glVertexPointer(3, GL_FLOAT, kQuadSize, (void *) (offset + diff));
			
			// color
			diff = offsetof( ccV3F_C4B_T2F, colors);
			glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (void *) (offset + diff));
			
			// tex coords
			diff = offsetof( ccV3F_C4B_T2F, texCoords);
			glTexCoordPointer(2, GL_FLOAT, kQuadSize, (void *) (offset + diff));
			
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			
			if (newBlend)
				glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		} else {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
			
			long offset = (long) &m_quad;
			
			int diff = offsetof( ccV3F_C4B_T2F, vertices);
			glVertexPointer(3, GL_FLOAT, kQuadSize, (void *) (offset + diff));
			
			// color
			diff = offsetof( ccV3F_C4B_T2F, colors);
			glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (void *) (offset + diff));
			
			
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
		
		if (s_debug_draw_outline) {
			// draw bounding box
			Size s = contentSize();
			Point vertices[4] = {
				PointMake(0,0), PointMake(s.width,0),
				PointMake(s.width,s.height), PointMake(0,s.height)
			};
			ccDrawPoly(vertices, 4, true);
		}
		if (s_debug_draw_texture_outline) {
			// draw texture box
			Size s = textureRect().size;
			Point offsetPix = offsetPositionInPixels();
			Point vertices[4] = {
				PointMake(offsetPix.x,offsetPix.y), PointMake(offsetPix.x+s.width,offsetPix.y),
				PointMake(offsetPix.x+s.width,offsetPix.y+s.height), PointMake(offsetPix.x,offsetPix.y+s.height)
			};
			ccDrawPoly(vertices, 4, true);
		}
		
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Node overrides
  void Sprite::addChild(NonNullPassRefPtr<Aphid::Node> prpChild, int z, int tag)
  {
	  RefPtr<Node> child = prpChild;
	  if (child) {
		  Base::addChild(child, z, tag);
			
		  if (m_usesBatchNode) {
			  ASSERT(child->toSprite());
			  ASSERT(child->toSprite()->texture()->name() == m_textureAtlas->texture()->name());
				
			  size_t index = m_batchNode->atlasIndexForChild(child->toSprite(), z);
			  m_batchNode->insertChildSprite(child->toSprite(), index);
		  }
	  }
		
	  m_hasChildren = true;
  }
	
  void Sprite::reorderChild(PassRefPtr<Node> prpChild, int z)
  {
	  RefPtr<Node> child = prpChild;
		
	  ASSERT( child);
		
	  if (z == child->zOrder())
		  return;
		
	  if (m_usesBatchNode) {
		  // TODO: Instead of removing/adding, it is more efficient to reorder manually
		  removeChild(child.get(), false);
		  Base::addChild(child, z);
	  } else
		  Base::reorderChild(child, z);
  }
	
  void Sprite::removeChild(Node *node, bool cleanup)
  {
	  if (m_usesBatchNode) {
		  ASSERT(node->toSprite());
		  m_batchNode->removeSpriteFromAtlas(node->toSprite());
	  }
		
	  Base::removeChild(node, cleanup);
		
	  m_hasChildren = m_children && m_children->size() > 0;
  }
	
  void Sprite::removeAllChildren(bool cleanup)
  {
	  if (m_usesBatchNode && m_children) {
		  for (NodeVector::const_iterator it = m_children->internalStorage().begin(); it != m_children->internalStorage().end(); ++it) {
			  m_batchNode->removeSpriteFromAtlas(it->get()->toSprite());
		  }
	  }
		
	  Base::removeAllChildren(cleanup);
		
	  m_hasChildren = false;
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  //Getter & Setter
  void Sprite::setDirtyRecursively(bool b)
  {
	  m_dirty = m_recursiveDirty = b;
	  if (m_hasChildren) {
		  for (NodeVector::const_iterator it = m_children->internalStorage().begin(); it != m_children->internalStorage().end(); ++it) {
			  it->get()->toSprite()->setDirtyRecursively(true); //TODO: check null
		  }
	  }
  }
	
	void Sprite::applyDirtyRecursively()
	{
		if( m_usesBatchNode && ! m_recursiveDirty) {
			m_dirty = m_recursiveDirty = true;
			if( m_hasChildren)
				setDirtyRecursively(true);
		}
	}
	
  void Sprite::setPosition(const Point &p)
  {
	  Base::setPosition(p);
	  applyDirtyRecursively();
  }
	
  void Sprite::setPositionInPixels(const Point &p)
  {
	  Base::setPositionInPixels(p);
	  applyDirtyRecursively();
  }
	
  void Sprite::setRotation(float v)
  {
	  Base::setRotation(v);
	  applyDirtyRecursively();
  }
	
  void Sprite::setSkew(const Aphid::Vector2 &v)
  {
	  Base::setSkew(v);
	  applyDirtyRecursively();
  }
	
  void Sprite::setSkew(float x, float y)
  {
	  Base::setSkew(x, y);
	  applyDirtyRecursively();
  }
	
  void Sprite::setSkewX(float v)
  {
	  Base::setSkewX(v);
	  applyDirtyRecursively();
  }
	
  void Sprite::setSkewY(float v)
  {
	  Base::setSkewY(v);
	  applyDirtyRecursively();
  }
	
  void Sprite::setScale(const Aphid::Vector2 &v)
  {
	  Base::setScale(v);
	  applyDirtyRecursively();
  }
	
  void Sprite::setScaleX(float x)
  {
	  Base::setScaleX(x);
	  applyDirtyRecursively();
  }
	
  void Sprite::setScaleY(float y)
  {
	  Base::setScaleY(y);
	  applyDirtyRecursively();
  }
	
  void Sprite::setVertexZ(float v)
  {
	  Base::setVertexZ(v);
	  applyDirtyRecursively();
  }
	
  void Sprite::setAnchor(const Point &p)
  {
	  Base::setAnchor(p);
	  applyDirtyRecursively();
  }
	
  void Sprite::setRelativeAnchor(bool b)
  {
	  Base::setRelativeAnchor(b);
	  applyDirtyRecursively();
  }
	
  void Sprite::setVisible(bool visible)
  {
	  Base::setVisible(visible);
	  applyDirtyRecursively();
  }
	
  void Sprite::setFlipX(bool b)
  {
	  if (m_flipX != b) {
		  m_flipX = b;
		  setTextureRectInPixels(m_rectInPixels, m_rectRotated, m_contentSizeInPixels);
	  }
  }
	
  void Sprite::setFlipY(bool b)
  {
	  if (m_flipY != b) {
		  m_flipY = b;
		  setTextureRectInPixels(m_rectInPixels, m_rectRotated, m_contentSizeInPixels);
	  }
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // RGBAProtocol
  void Sprite::updateColor()
  {
	  ccColor4B color4 = {m_color.r, m_color.g, m_color.b, m_opacity};
		
	  m_quad.bl.colors = color4;
	  m_quad.br.colors = color4;
	  m_quad.tl.colors = color4;
	  m_quad.tr.colors = color4;
		
	  // renders using batchNode
	  if (m_usesBatchNode) {
		  if (m_atlasIndex != G2DSpriteIndexNotInitialized)
			  m_textureAtlas->updateQuad(&m_quad, m_atlasIndex);
		  else
			  // no need to set it recursively
			  // update dirty_, don't update recursiveDirty_
			  m_dirty = true;
	  }
  }
	
  ccColor3B Sprite::color() const
  {
	  if (m_opacityModifyRGB)
		  return m_colorUnmodified;
	  return m_color;
  }
	
  void Sprite::setColor(const ccColor3B& c)
  {
	  m_color = m_colorUnmodified = c;
		
	  if (m_opacityModifyRGB) {
		  m_color.r = c.r * m_opacity / 255;
		  m_color.g = c.g * m_opacity / 255;
		  m_color.b = c.b * m_opacity / 255;
	  }
		
	  updateColor();
  }
	
  void Sprite::setOpacity(GLubyte opacity)
  {
	  m_opacity = opacity;
		
	  // special opacity for premultiplied textures
	  if (m_opacityModifyRGB)
		  setColor(m_colorUnmodified);
		
	  updateColor();
  }
	
  void Sprite::setOpacityModifyRGB(bool modify)
  {
	  ccColor3B oldColor = color();
	  m_opacityModifyRGB = modify;
	  setColor(oldColor);
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Frames & Animations
	
  void Sprite::setDisplayFrame(PassRefPtr<SpriteFrame> prpFrame)
  {
	  ATF::RefPtr<SpriteFrame> frame = prpFrame;
		
	  m_unflippedOffsetPositionFromCenter = frame->offsetInPixels();
		
	  Texture2D *newTexture = frame->texture();
	  // update texture before updating texture rect
	  if (newTexture->name() != m_texture->name())
		  setTexture(newTexture);
		
	  // update rect
	  m_rectRotated = frame->rotated();
	  setTextureRectInPixels(frame->rectInPixels(), frame->rotated(), frame->originalSizeInPixels());
  }
	
  bool Sprite::isFrameDisplayed(SpriteFrame *frame)
  {
	  Rect r = frame->rect();
		Point p = frame->offsetInPixels(); //fix http://code.google.com/p/cocos2d-iphone/issues/detail?id=707
	  return (RectEqual(r, m_rect) &&
						frame->texture()->name() == this->texture()->name() &&
						PointEqual(p, m_offsetPositionInPixels)
						);
  }
	
  PassRefPtr<SpriteFrame> Sprite::displayedFrame()
  {
	  return SpriteFrame::create(m_texture, m_rectInPixels, m_rectRotated, m_unflippedOffsetPositionFromCenter, m_contentSizeInPixels);
  }
	
  //TextureProtocol
  void Sprite::setTexture(PassRefPtr<Texture2D> texture)
  {
	  ASSERT(!m_usesBatchNode);
	  m_texture = texture;
	  updateBlendFunc();
  }
	
  void Sprite::updateBlendFunc()
  {
	  ASSERT(!m_usesBatchNode);
	  if (!m_texture || !m_texture->hasPremultipliedAlpha()) {
		  m_blendFunc.src = GL_SRC_ALPHA;
		  m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		  setOpacityModifyRGB(false);
	  } else {
		  m_blendFunc.src = CC_BLEND_SRC;
		  m_blendFunc.dst = CC_BLEND_DST;
		  setOpacityModifyRGB(true);
	  }
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // InheritanceWrapper
  const AJ::ClassInfo *Sprite::wrapperClassInfo() const
  {
	  return &AJSprite::s_info;
  }
	
  AJ::AJObject *Sprite::createWrapper(AJ::ExecState *exec, AJOAGlobalObject *globalObject)
  {
	  return createAJOAWrapper<AJSprite>(exec, globalObject, this);
  }
	
	void Sprite::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		Base::markObjects(markStack, markID);
		if (m_texture)
			m_texture->markObjects(markStack, markID);
	}
	
} //namespace Aphid
