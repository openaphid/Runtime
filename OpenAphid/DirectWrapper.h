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

#ifndef OpenAphid_DirectWrapper_h
#define OpenAphid_DirectWrapper_h

#include <runtime/WeakGCPtr.h>
#include <runtime/MarkStack.h>
#include "OAUtil.h"

namespace Aphid {	
	template<class T> class DirectWrapper {
	public:
		DirectWrapper()
		: m_markID(0)
		{			
		}
		
		virtual ~DirectWrapper() {}
		
		T* wrapper() const
		{
			return m_wrapper.get();
		}
		
		void setWrapper(T* wrapper)
		{
			m_wrapper = wrapper;
		}
		
		void clearWrapper(T* wrapper)
		{
			if (!m_wrapper.clear(wrapper))
				oa_error("can't clear wrapper: %p", wrapper);
		}
		
		T* directWrapper() const
		{
			return m_wrapper.forceGet();
		}
		
		bool isInheritanceWrapper() const {return false;}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID)
		{
			if (isMarked(markID))
				return;
			
			if (m_wrapper.forceGet())
				markStack.append(m_wrapper.forceGet());
			
			m_markID = markID;
		}
		
		bool isMarked(unsigned markID) const {return m_markID == markID;}
		
	private:
		AJ::WeakGCPtr<T> m_wrapper;
		unsigned m_markID;
	};	
}


#endif
