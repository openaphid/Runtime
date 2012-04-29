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

#include "NodeList.h"
#include "Node.h"
#include "Sprite.h"
#include "OAUtil.h"
#include "AJNodeList.h"

namespace Aphid {
	NodeList::NodeList(size_t initCapacity)
	{
		if (initCapacity)
			m_nodes.reserveInitialCapacity(initCapacity);
		
		LEAK_DETECT_INC("NodeList");
	}
	
	NodeList::~NodeList()
	{
		//oa_debug("NodeList destructed");
		LEAK_DETECT_DEC("NodeList");
	}
	
	Node* NodeList::at(size_t index) const
	{
		if (index < m_nodes.size())
			return m_nodes.at(index).get();
		return 0;
	}
	
	Node* NodeList::optionalLast() const
	{
		if (m_nodes.size() == 0)
			return 0;
		return m_nodes.last().get();
	}
	
	void NodeList::add(PassRefPtr<Node> node)
	{
		ASSERT(node);
		m_nodes.append(node);
	}
	
	void NodeList::insert(PassRefPtr<Node> prpNode, int z)
	{
		ASSERT(prpNode);
		RefPtr<Node> child = prpNode;
		Node* last = optionalLast();
		
		if (!last || last->m_zOrder <= z)
			m_nodes.append(child);
		else {
			size_t size = m_nodes.size();
			for (size_t i = 0; i < size; i++) {
				RefPtr<Node> t = m_nodes[i];
				if (t->m_zOrder > z) {
					m_nodes.insert(i, child);
					break;
				}
			} //end of for
		} //end of if
		
		child->m_zOrder = z;
	}
	
	bool NodeList::contains(Node* node) const
	{
		return m_nodes.find(node) != ATF::notFound;
	}

	size_t NodeList::indexOf(Node* node) const
	{
		return m_nodes.find(node);
	}
	
	bool NodeList::remove(Node* node)
	{
		size_t index = m_nodes.find(node);
		if (index != ATF::notFound) {
			m_nodes.remove(index);
		}
		
		return false;
	}
	
	Node* NodeList::getNodeByTag(int tag) const
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter) {
			if (iter->get()->m_tag == tag)
				return iter->get();
		}
		
		return 0;
	}
	
	void NodeList::setAllParent(Node* parent)
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
			iter->get()->setParent(parent);
	}
	
	void NodeList::removeAll()
	{
		m_nodes.clear();
	}
	
	void NodeList::allOnEnter()
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
			iter->get()->onEnter();
	}
	
	void NodeList::allOnEnterTransitionDidFinish()
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
			iter->get()->onEnterTransitionDidFinish();
	}
	
	void NodeList::allOnExit()
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
			iter->get()->onExit();
	}
	
	void NodeList::allCleanup()
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
			iter->get()->cleanup();
	}
	
	void NodeList::allSpritesUseSelfRender()
	{
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter) {
			Sprite* sprite = iter->get()->toSprite();
			if (sprite)
				sprite->useSelfRender();
		}
	}
	
	void NodeList::markObjects(AJ::MarkStack &markStack, unsigned markID)
	{
		if (isMarked(markID))
			return;
		
		DirectWrapper<AJNodeList>::markObjects(markStack, markID);
		for (NodeVector::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
			iter->get()->markObjects(markStack, markID);
	}
	
	Node* NodeList::hitTest(const Point &p, PlatformTouchEvent *event)
	{
		int len = m_nodes.size();
		for (int i = len - 1; i >= 0; i--) {
			Node* child = m_nodes.at(i).get();
			//AffineTransform transform = child->parentToNodeTransform();
			//Point childP = PointApplyAffineTransform(p, transform);
			Point childP = child->convertParentToNodeSpace(p);
			Node* hit = child->hitTest(childP, event);
			if (hit)
				return hit;
		}
		
		return 0;
	}
	
	bool NodeList::pointInside(const Point& p, PlatformTouchEvent* event)
	{
		for (NodeVector::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
			Node* child = it->get();
			//AffineTransform transform = child->parentToNodeTransform();
			//Point childP = PointApplyAffineTransform(p, transform);
			Point childP = child->convertParentToNodeSpace(p);
			if (child->pointInside(childP, event))
				return true;
		}
		
		return false;
	}
}