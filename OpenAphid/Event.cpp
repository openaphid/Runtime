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
#include "Event.h"

#include "OAUtil.h"
#include "AJEvent.h"
#include "AJProgressEvent.h"
#include "AJOABinding.h"
#include "AJEventListener.h"

namespace Aphid {
	using Aphid::AtomicString;
	///-------------------------------------------------------------------------------------------------------------------
	AtomicString EventNames::touchstartEvent = "touchstart";
	AtomicString EventNames::touchendEvent = "touchend";
	AtomicString EventNames::touchmoveEvent = "touchmove";
	AtomicString EventNames::touchcancelEvent = "touchcancel";
	AtomicString EventNames::readystatechangeEvent = "readystatechange";
	AtomicString EventNames::loadEvent = "load";
	AtomicString EventNames::loadendEvent = "loadend";
	AtomicString EventNames::errorEvent = "error";
	AtomicString EventNames::abortEvent = "abort";
	///-------------------------------------------------------------------------------------------------------------------
	/// Event
	Event::Event(const AtomicString& type)
	: m_type(type)
	, m_eventPhase(EventPhaseUnknown)
	, m_timestamp((unsigned)(ATF::currentTime() * 1000.0))
	, m_propagationStopped(false)
	{
		LEAK_DETECT_INC("Event");
	}
	
	Event::~Event()
	{
		LEAK_DETECT_DEC("Event");
	}
	
	const AJ::ClassInfo* Event::wrapperClassInfo() const
	{
		return &AJEvent::s_info;
	}
	
	AJ::AJObject* Event::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJEvent>(exec, globalObject, this);
	}
	
	void Event::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		InheritanceWrapper::markObjects(markStack, markID);
		markEventTarget(m_target.get(), markStack, markID);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// ProgressEvent
	ProgressEvent::ProgressEvent(const AtomicString& type, bool lengthComputable, unsigned loaded, unsigned total)
	: Base(type)
	, m_lengthComputable(lengthComputable)
	, m_loaded(loaded)
	, m_total(total)
	{
		LEAK_DETECT_INC("ProgressEvent");
	}
	
	ProgressEvent::~ProgressEvent()
	{
		LEAK_DETECT_DEC("ProgressEvent");
	}
	
	const AJ::ClassInfo* ProgressEvent::wrapperClassInfo() const 
	{
		return &AJProgressEvent::s_info;
	}
	
	AJ::AJObject* ProgressEvent::createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		return createAJOAWrapper<AJProgressEvent>(exec, globalObject, this);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EventListener
	EventListener::EventListener()
	{
		LEAK_DETECT_INC("EventListener");
	}
	
	EventListener::~EventListener()
	{
		LEAK_DETECT_DEC("EventListener");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// EventTarget
	EventTarget::~EventTarget()
	{
		OA_DELETE(m_listeners);
	}
	
	EventListener* EventTarget::findEventListener(EventFlag flag)
	{
		if (m_listeners) {
			ListenerMap::iterator it = m_listeners->find(flag);
			if (it != m_listeners->end())
				return it->second.get();
		}
		
		return 0;
	}
	
	void EventTarget::registerEventListener(Aphid::EventFlag flag, ATF::PassRefPtr<EventListener> prpListener)
	{
		RefPtr<EventListener> listener = prpListener;
		
		ASSERT(listener);
		
		unregisterEventListeners(flag);
		
		ensureListenerMap();
		
		m_listenerList->append(listener.get());
		m_listeners->add(flag, listener);		
		
		m_flags |= flag;
	}
	
	bool EventTarget::unregisterEventListeners(Aphid::EventFlag flag)
	{
		if (!m_listeners)
			return false;
		
		ListenerMap::iterator it = m_listeners->find(flag);
		if (it != m_listeners->end()) {
			m_listenerList->remove(it->second.get());
			m_listeners->remove(it);
			m_flags &= ~flag;
			return true;
		}
		
		return false;
	}
	
	void EventTarget::clearEventListeners()
	{
		if (!m_listeners)
			return;
		
		m_flags = 0;
		m_listeners->clear();
	}
	
	void EventTarget::ensureListenerMap()
	{
		if (!m_listeners) {
			m_listeners = new ListenerMap();
			m_listenerList = new ListenerList();
		}
	}
	
	void EventTarget::markEventListeners(AJ::MarkStack& markStack, unsigned markID)
	{
		if (m_flags && m_listeners) {
			EventListener* cursor = m_listenerList->head();
			while (cursor) {
				AJEventListener* l = cursor->toAJEventListener();
				if (l)
					l->markCallback(markStack, markID);
				cursor = cursor->next();
			}
			
			/*
			for (ListenerMap::iterator it = m_listeners->begin(); it != m_listeners->end(); ++it) {
				AJEventListener* l = it->second->toAJEventListener();
				if (l)
					l->markCallback(markStack, markID);
			}
			 */
		}
	}	
}