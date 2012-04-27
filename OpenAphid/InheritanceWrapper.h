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

#ifndef OpenAphid_InheritanceWrapper_h
#define OpenAphid_InheritanceWrapper_h

#include "runtime/WeakGCPtr.h"
#include "runtime/AJObject.h"
#include "OAUtil.h"

namespace Aphid {
	class AJOAGlobalObject;
	
	class InheritanceWrapper {
	public:
		InheritanceWrapper()
		: m_markID(0)
		{
		}
		
		virtual ~InheritanceWrapper() {}
		
		AJ::AJObject* wrapper() const
		{
			return m_wrapper.get();
		}
		
		void setWrapper(AJ::AJObject* wrapper)
		{
			//TODO: disable this check
			if (wrapper && !wrapper->inherits(wrapperClassInfo())) {
				oa_error("Invalid type of wrapper");
				ASSERT_NOT_REACHED();
			}
			m_wrapper = wrapper;
		}
		
		void clearWrapper(AJ::AJObject* wrapper)
		{
			if (!m_wrapper.clear(wrapper))
				oa_error("can't clear wrapper: %p", wrapper);
		}
		
		AJ::AJObject* directWrapper() const
		{
			return m_wrapper.forceGet();
		}
		
		bool isInheritanceWrapper() const {return true;}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const = 0;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject) = 0;
		
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
		AJ::WeakGCPtr<AJ::AJObject> m_wrapper;
		unsigned m_markID;
	};
}


#endif
