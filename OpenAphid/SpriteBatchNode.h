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

#ifndef OpenAphid_SpriteBatchNode_h
#define OpenAphid_SpriteBatchNode_h

#include "Node.h"
#include "TextureAtlas.h"

namespace Aphid {
	class SpriteBatchNode : public Node, public TextureProtocol {
		typedef Node Base;
		typedef ATF::Vector<RefPtr<Sprite> > SpriteVector;
	public:
		virtual ~SpriteBatchNode();
		
		static ATF::PassRefPtr<SpriteBatchNode> create(ATF::PassRefPtr<Texture2D> texture, size_t capacity=29)
		{
			return ATF::adoptRef(new SpriteBatchNode(texture, capacity));
		}
		
		void increaseAtlasCapacity();
		size_t rebuiltIndexInOrder(Node* node, size_t index);
		size_t highestAtlasIndexInChild(Sprite* sprite);
		size_t lowestAtlasIndexInChild(Sprite* sprite);
		size_t atlasIndexForChild(Sprite* sprite, int z);
		void insertChildSprite(PassRefPtr<Sprite> prpSprite, size_t index);
		void removeSpriteFromAtlas(Sprite* sprite);
		
		virtual void visit();		
		virtual void addChild(NonNullPassRefPtr<Node> node, int z, int tag);
		using Node::addChild;
		virtual void reorderChild(PassRefPtr<Node> node, int z);
		virtual void removeChild(Node *node, bool cleanup);
		virtual void removeAllChildren(bool cleanUp);
		
		virtual void draw();
		
		TextureAtlas* textureAtlas() const {return m_textureAtlas.get();}
		
		//TextureProtocol
		void updateBlendFunc();
		Texture2D* texture() const {return m_textureAtlas->texture();}
		virtual void setTexture(PassRefPtr<Texture2D> texture);
		
	protected:
		SpriteBatchNode(PassRefPtr<Texture2D> texture, size_t capacity);
		
		RefPtr<TextureAtlas> m_textureAtlas;
		ccBlendFunc m_blendFunc;
		SpriteVector m_descendants;
	};
}

#endif
