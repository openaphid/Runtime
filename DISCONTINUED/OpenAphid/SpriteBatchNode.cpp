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

#include "SpriteBatchNode.h"

#include "G2DMacros.h"

#include "Sprite.h"

#include "glu.h"

namespace Aphid {
  SpriteBatchNode::SpriteBatchNode(PassRefPtr<Texture2D> texture, size_t capacity)
  {
	  lazyCreateChildren();

	  m_blendFunc.src = CC_BLEND_SRC;
	  m_blendFunc.dst = CC_BLEND_DST;

	  m_textureAtlas = TextureAtlas::create(texture, capacity);

	  updateBlendFunc();

	  LEAK_DETECT_INC("SpriteBatchNode");
  }

  SpriteBatchNode::~SpriteBatchNode()
  {
	  LEAK_DETECT_DEC("SpriteBatchNode");
  }

  void SpriteBatchNode::visit()
  {
	  if (!m_visible)
		  return;

	  glPushMatrix();

	  if (m_grid && m_grid->active()) {
		  m_grid->beforeDraw();
		  transformAncestors();
	  }

	  transform();

	  draw();

	  if (m_grid && m_grid->active())
		  m_grid->afterDraw(this);

	  glPopMatrix();
  }

  void SpriteBatchNode::addChild(NonNullPassRefPtr<Aphid::Node> prpChild, int z, int tag)
  {
	  RefPtr<Node> child = prpChild;

	  ASSERT( child);
	  ASSERT( child->toSprite());
	  ASSERT( child->toSprite()->texture()->name() == m_textureAtlas->texture()->name());

	  Base::addChild(child, z, tag);

	  size_t index = atlasIndexForChild(child->toSprite(), z);
	  insertChildSprite(child->toSprite(), index);
  }

  void SpriteBatchNode::reorderChild(PassRefPtr<Aphid::Node> prpChild, int z)
  {
	  RefPtr<Node> child = prpChild;

	  ASSERT(child);
	  ASSERT(m_children->contains(child.get()));

	  if (z == child->zOrder())
		  return;

	  // TODO: Instead of removing/adding, it is more efficient to reorder manually
	  removeChild(child.get(), false);
	  addChild(child, z);
  }

  void SpriteBatchNode::removeChild(Aphid::Node *prpChild, bool cleanup)
  {
	  RefPtr<Node> child = prpChild;
	  Sprite *sprite = child->toSprite();
	  ASSERT(child && sprite);

	  removeSpriteFromAtlas(sprite);
	  Base::removeChild(child.get(), cleanup);
  }

  void SpriteBatchNode::removeAllChildren(bool cleanup)
  {
	  // Invalidate atlas index. issue #569
	  m_children->allSpritesUseSelfRender();
	  Base::removeAllChildren(cleanup);

	  m_descendants.clear();
	  m_textureAtlas->removeAllQuads();
  }

  //--------------------------------------------------------------------------------------------------------------------
  // Draw
  void SpriteBatchNode::draw()
  {
	  Base::draw();

	  // Optimization: Fast Dispatch
	  if (m_textureAtlas->totalQuads() == 0)
		  return;

	  Sprite *child;

	  size_t i = m_descendants.size();

	  if (i > 0) {

		  while (i-- > 0) {
			  child = m_descendants.at(i).get();

			  // fast dispatch
			  child->updateTransform();

#if CC_SPRITEBATCHNODE_DEBUG_DRAW
				//Issue #528
				Rect rect = child->boundingBox();
				Point vertices[4]={
					PointMake(rect.origin.x,rect.origin.y),
					PointMake(rect.origin.x+rect.size.width,rect.origin.y),
					PointMake(rect.origin.x+rect.size.width,rect.origin.y+rect.size.height),
					PointMake(rect.origin.x,rect.origin.y+rect.size.height),
				};
				ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITEBATCHNODE_DEBUG_DRAW
		  }
	  }

	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Unneeded states: -

	  bool newBlend = m_blendFunc.src != CC_BLEND_SRC || m_blendFunc.dst != CC_BLEND_DST;
	  if (newBlend)
		  glBlendFunc(m_blendFunc.src, m_blendFunc.dst);

	  m_textureAtlas->drawQuads();
	  if (newBlend)
		  glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
  }

  void SpriteBatchNode::increaseAtlasCapacity()
  {
	  // if we're going beyond the current TextureAtlas's capacity,
	  // all the previously initialized sprites will need to redo their texture coords
	  // this is likely computationally expensive
	  size_t quantity = (m_textureAtlas->capacity() + 1) * 4 / 3;

	  oa_debug("SpriteBatchNode: resizing TextureAtlas capacity from [%lu] to [%lu].",
	  (long) m_textureAtlas->capacity(),
	  (long) quantity);


	  if (!m_textureAtlas->resize(quantity)) {
		  // serious problems
		  oa_error("Not enough memory to resize the atlas");
		  ASSERT_NOT_REACHED();
	  }
  }

  //TODO: can be improved?
  size_t SpriteBatchNode::rebuiltIndexInOrder(Node *node, size_t index)
  {
	  Sprite *sprite = node->toSprite();

	  ASSERT(sprite);

	  NodeList *nodeChildren = node->children();
	  if (nodeChildren) {
		  for (NodeVector::const_iterator it = nodeChildren->internalStorage().begin(); it != nodeChildren->internalStorage().end(); ++it) {
			  if (it->get()->zOrder() < 0)
				  index = rebuiltIndexInOrder(it->get(), index);
		  }
	  }

	  if (node != this) {
		  sprite->setAtlasIndex(index);
		  index++;
	  }

	  if (nodeChildren) {
		  for (NodeVector::const_iterator it = nodeChildren->internalStorage().begin(); it != nodeChildren->internalStorage().end(); ++it) {
			  if (it->get()->zOrder() >= 0)
				  index = rebuiltIndexInOrder(it->get(), index);
		  }
	  }

	  return index;
  }

  size_t SpriteBatchNode::highestAtlasIndexInChild(Sprite *sprite)
  {
	  if (sprite->childrenSize() == 0)
		  return sprite->atlasIndex();
	  else
		  return highestAtlasIndexInChild(sprite->children()->optionalLast()->toSprite());
  }

  size_t SpriteBatchNode::lowestAtlasIndexInChild(Aphid::Sprite *sprite)
  {
	  if (sprite->childrenSize() == 0)
		  return sprite->atlasIndex();
	  else
		  return lowestAtlasIndexInChild(sprite->children()->at(0)->toSprite());
  }

  size_t SpriteBatchNode::atlasIndexForChild(Sprite *sprite, int z)
  {
	  NodeList *brothers = sprite->parent()->children(); //TODO check null parent()?
	  size_t childIndex = brothers->indexOf(sprite);

	  // ignore parent Z if parent is batchnode
	  bool ignoreParent = (sprite->parent() == this);
	  Sprite *previous = 0;
	  if (childIndex > 0)
		  previous = brothers->at(childIndex - 1)->toSprite();

	  // first child of the sprite sheet
	  if (ignoreParent) {
		  if (childIndex == 0)
			  return 0;
		  // else
		  return highestAtlasIndexInChild(previous) + 1;
	  }

	  // parent is a CCSprite, so, it must be taken into account

	  // first child of an CCSprite ?
	  if (childIndex == 0) {
		  Sprite *p = sprite->parent()->toSprite();

		  // less than parent and brothers
		  if (z < 0)
			  return p->atlasIndex();
		  else
			  return p->atlasIndex() + 1;

	  } else {
		  // previous & sprite belong to the same branch
		  if ((previous->zOrder() < 0 && z < 0) || (previous->zOrder() >= 0 && z >= 0))
			  return highestAtlasIndexInChild(previous) + 1;

		  // else (previous < 0 and sprite >= 0 )
		  Sprite *p = sprite->parent()->toSprite();
		  return p->atlasIndex() + 1;
	  }

	  ASSERT_NOT_REACHED();
	  return 0;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // add/remove/reorder
  void SpriteBatchNode::insertChildSprite(PassRefPtr<Sprite> prpSprite, size_t index)
  {
	  RefPtr<Sprite> sprite = prpSprite;
	  sprite->useBatchNode(this);
	  sprite->setAtlasIndex(index);
	  sprite->setDirty(true);

	  if (m_textureAtlas->totalQuads() == m_textureAtlas->capacity())
		  increaseAtlasCapacity();

	  ccV3F_C4B_T2F_Quad quad = sprite->quad();
	  m_textureAtlas->insertQuad(&quad, index);

	  m_descendants.insert(index, sprite);

	  // update indices
	  size_t i = index + 1;
	  for (; i < m_descendants.size(); i++) {
		  Sprite *child = m_descendants.at(i).get();
		  child->setAtlasIndex(child->atlasIndex() + 1);
	  }

	  // add children recursively

	  if (sprite->childrenSize() > 0) {
		  NodeList *children = sprite->children();
		  for (NodeVector::const_iterator it = children->internalStorage().begin(); it != children->internalStorage().end(); ++it) {
			  size_t idx = atlasIndexForChild(it->get()->toSprite(), it->get()->zOrder());
			  insertChildSprite(it->get()->toSprite(), idx);
		  }
	  }
  }

  void SpriteBatchNode::removeSpriteFromAtlas(Sprite *sprite)
  {
	  // remove from TextureAtlas
	  m_textureAtlas->removeQuad(sprite->atlasIndex());

	  // Cleanup sprite. It might be reused (issue #569)
	  sprite->useSelfRender();

	  size_t index = m_descendants.find(sprite);
	  if (index != ATF::notFound) {
		  m_descendants.remove(index);

		  // update all sprites beyond this one
		  size_t count = m_descendants.size();

		  for (; index < count; index++) {
			  Sprite *s = m_descendants.at(index).get();
			  s->setAtlasIndex(s->atlasIndex() - 1);
		  }
	  }

	  if (sprite->childrenSize() > 0) {
		  NodeList *children = sprite->children();
		  for (NodeVector::const_iterator it = children->internalStorage().begin(); it != children->internalStorage().end(); ++it)
			  removeSpriteFromAtlas(it->get()->toSprite());
	  }
  }

  void SpriteBatchNode::updateBlendFunc()
  {
	  if (!m_textureAtlas->texture()->hasPremultipliedAlpha()) {
		  m_blendFunc.src = GL_SRC_ALPHA;
		  m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	  }
  }

  void SpriteBatchNode::setTexture(PassRefPtr<Texture2D> texture)
  {
	  m_textureAtlas->setTexture(texture);
	  updateBlendFunc();
  }

}