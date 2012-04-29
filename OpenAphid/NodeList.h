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

#ifndef OpenAphid_NodeList_h
#define OpenAphid_NodeList_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "wtf/Vector.h"
#include "DirectWrapper.h"

#include "OAData.h"
#include "Touch.h"

namespace Aphid {
	class Node;
	class Sprite;
	class AJNodeList;
	
	typedef Vector<RefPtr<Node> > NodeVector;
	
	class NodeList : public RefCounted<NodeList>, public DirectWrapper<AJNodeList> {
		friend class Node;
		friend class Sprite;
	public:
		virtual ~NodeList();
		
		static PassRefPtr<NodeList> create(size_t initCapacity=0)
		{
			return adoptRef(new NodeList(initCapacity));
		}
		
		size_t size() const {return m_nodes.size();}
		Node* at(size_t index) const;
		Node* optionalLast() const;
		void add(PassRefPtr<Node> node);
		void insert(PassRefPtr<Node> prpNode, int z);
		bool contains(Node* node) const;
		size_t indexOf(Node* node) const;
		bool remove(Node* node);
		Node* getNodeByTag(int tag) const;		
		void removeAll();
		
		void setAllParent(Node* parent);
		void allOnEnter();
		void allOnEnterTransitionDidFinish();
		void allOnExit();
		void allCleanup();
		void allSpritesUseSelfRender();
		
		NodeVector& internalStorage() {return m_nodes;} //TODO: ugly visibility...
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		Node* hitTest(const Point &p, PlatformTouchEvent* event);
		bool pointInside(const Point& p, PlatformTouchEvent* event);
		
	private:
		NodeList(size_t initCapacity);
		
	private:
		NodeVector m_nodes;
	};
}

#endif
