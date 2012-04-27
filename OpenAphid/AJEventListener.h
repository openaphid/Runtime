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

#ifndef OpenAphid_AJEventListener_h
#define OpenAphid_AJEventListener_h

#include "Event.h"
#include "AJCallback.h"

namespace Aphid {
	class AJOAGlobalObject;
	
	class AJEventListener : public EventListener {
		typedef AJEventListener Self;
	public:
		virtual ~AJEventListener();
		
		static PassRefPtr<Self> create(AJ::ExecState* exec, AJ::AJValue listener);
		static PassRefPtr<Self> create(AJ::ExecState* exec, AJ::AJObject* listener);
		
		static AJ::AJValue findAJCallback(EventListener* listener);
		static void setAJListener(Aphid::EventFlag flag, Aphid::EventTarget *target, AJ::ExecState* exec, AJ::AJValue value);
		
		virtual void handleEvent(Event* event);
		virtual AJEventListener* toAJEventListener() {return this;}
		
		AJ::AJObject* callback() const {return m_callback ? m_callback->optObject() : 0;}
		void markCallback(AJ::MarkStack& markStack, unsigned markID);
//		{
//			if (m_callback)
//				m_callback->markObject(markStack, markID);
//		}
		
	protected:
		AJEventListener(AJ::AJObject* ajCallback);
		
		RefPtr<MappedAJObject> m_callback;
	};
	
	AJ::AJValue toAJ(AJ::ExecState* exec, AJOAGlobalObject* globalObject, EventTarget* target);
	void markEventTarget(EventTarget* target, AJ::MarkStack& markStack, unsigned markID);
}

#endif
