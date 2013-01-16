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

#ifndef OpenAphid_LinkedList_h
#define OpenAphid_LinkedList_h

namespace Aphid {
	template <typename T>
	class _LinkElement {
	public:
		T* next() const {return m_next;}
		void setNext(T* next) {m_next = next;}
		
		T* prev() const {return m_prev;}
		void setPrev(T* prev) {m_prev = prev;}
		
		void* linkedList() const {return m_linkedList;}
		void setLinkedList(void* list) {m_linkedList = list;}
		
	protected:
		_LinkElement()
		: m_next(0)
		, m_prev(0)
		, m_linkedList(0)
		{}
	private:
		T* m_next;
		T* m_prev;
		void* m_linkedList;
	};
	
	template<typename T>
	class _LinkedList {
	public:
		_LinkedList()
		: m_head(0)
		, m_end(0)
#if OA_DEV
		, m_size(0)
#endif
		{
			
		}
		
		void append(T el)
		{
			ASSERT(el);
			if (!el->linkedList()) {
				if (m_end) {
					m_end->setNext(el);
					el->setPrev(m_end);
					el->setNext(0);
					m_end = el;
				} else {
					m_head = m_end = el;
					el->setPrev(0);
					el->setNext(0);
				}
				
				el->setLinkedList(this);
				
#if OA_DEV			
				m_size++;
#endif
			} else 
				oa_debug("can't append el %p, it's already belong to another linked list", el);
		}
		
		void insertBefore(T loc, T el)
		{
			ASSERT(el);
			if (!el->linkedList()) {
				if (loc) {
					T prev = loc->prev();
					
					if (prev)
						prev->setNext(el);
					el->setPrev(prev);
					el->setNext(loc);
					loc->setPrev(el);
					
					if (loc == m_head)
						m_head = el;
					
#if OA_DEV			
					m_size++;
#endif
					el->setLinkedList(this);
				} else {
					ASSERT(!m_head);
					append(el);
				}
			} else 
				oa_debug("can't insert el, it's already belong to another linked list");
		}
		
		void remove(T el)
		{
			if (el->linkedList() == this) {
				T prev = el->prev();
				T next = el->next();
				
#if OA_DEV
				if (!prev && !next)
					ASSERT(el == m_head);
#endif
				
				if (prev)
					prev->setNext(next);
				else {
					ASSERT(el == m_head);
					m_head = next;
				}
				
				if (next)
					next->setPrev(prev);
				else {
					ASSERT(el == m_end);
					m_end = prev;
				}
				
				el->setPrev(0);
				el->setNext(0);
				
#if OA_DEV
				m_size--; //debug only , it's incorrect when the el is not in the list
#endif
			}else 
				oa_debug("can't remove el %p, it's not contained in this list", el);
		}
		
		void clear()
		{
			T cursor = m_head;
			while (cursor) {
				cursor->setLinkedList(0);
				cursor = cursor->next();
			}
			m_head = m_end = 0;
#if OA_DEV
			m_size = 0;
#endif
			
		}
		
		T head() const {return m_head;}
		T end() const {return m_end;}
		
	private:
		T m_head;
		T m_end;
		
#if OA_DEV
		size_t m_size;
#endif
	};
}

#endif
