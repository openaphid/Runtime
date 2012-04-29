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

#ifndef OpenAphid_Event_h
#define OpenAphid_Event_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/HashMap.h>
#include <runtime/UString.h>
#include <AtomicString.h>

#include "InheritanceWrapper.h"
#include "OAConfig.h"
#include "LinkedList.h"

namespace AJ {
	class ExecState;
}

namespace Aphid {
	class EventTarget;
	class TouchEventTarget;
	class Node;
	class AJEventListener;
	class XMLHttpRequest;
	
	enum EventFlag {
		EventFlagTouchHitTest = 1,
		EventFlagTouchStart = 1 << 1,
		EventFlagTouchMove = 1 << 2,
		EventFlagTouchEnd = 1 << 3,
		EventFlagTouchCancel = 1 << 4,
		EventFlagRequestLoadStart = 1 << 5,
		EventFlagRequestProgress = 1 << 6,
		EventFlagRequestAbort = 1 << 7,
		EventFlagRequestError = 1 << 8,
		EventFlagRequestLoad = 1 << 9,
		EventFlagRequestTimeout = 1 << 10,
		EventFlagRequestLoadEnd = 1 << 11,
		EventFlagRequestReadyStateChange = 1 << 12
	};
	
	enum EventPhase {
		EventPhaseUnknown = 0,
		CAPTURING_PHASE = 1,
		AT_TARGET = 2,
		BUBBLING_PHASE = 3
	};
	
	enum EventPropagation {
		EventPropagationStop = 0,
		EventPropagationContinue = 1,
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class EventNames : public Noncopyable {
	public:
		static AtomicString touchstartEvent;
		static AtomicString touchendEvent;
		static AtomicString touchmoveEvent;
		static AtomicString touchcancelEvent;
		static AtomicString readystatechangeEvent;
		static AtomicString loadEvent;
		static AtomicString loadendEvent;
		static AtomicString errorEvent;
		static AtomicString abortEvent;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Event : public ATF::RefCounted<Event>, public InheritanceWrapper {
	public:
		~Event();
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		//Getter/Setter
		Aphid::AtomicString type() const {return m_type;}
		void setType(const Aphid::AtomicString& s) {m_type = s;}
		
		EventTarget* target() const {return m_target.get();}
		void setTarget(PassRefPtr<EventTarget> t) {m_target = t;}
		
		EventTarget* currentTarget() const {return m_currentTarget;}
		void setCurrentTarget(EventTarget* t) {m_currentTarget = t;}
		
		EventPhase eventPhase() const {return m_eventPhase;}
		void setEventPhase(EventPhase phase) {m_eventPhase = phase;}
		
		unsigned timestamp() const {return m_timestamp;}
		void setTimestamp(unsigned t) {m_timestamp = t;}
		
		bool bubbles() const {return true;}
		bool cancelable() const {return true;}
		
		bool isPropagationStopped() const {return m_propagationStopped;}
		void stopPropagation() {m_propagationStopped = true;}
		
		void preventDefault() {} //do nothing
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		Event(const AtomicString& type);
		
		Aphid::AtomicString m_type;
		RefPtr<EventTarget> m_target;
		EventTarget* m_currentTarget;
		EventPhase m_eventPhase;
		unsigned m_timestamp;
		bool m_propagationStopped;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class ProgressEvent : public Event {
		typedef Event Base;
	public:
		virtual ~ProgressEvent();
		
		static PassRefPtr<ProgressEvent> create(const Aphid::AtomicString& type, bool lengthComputable=false, unsigned loaded=0, unsigned total=0)
		{
			return adoptRef(new ProgressEvent(type, lengthComputable, loaded, total));
		}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		bool lengthComputable() const {return m_lengthComputable;}
		unsigned loaded() const {return m_loaded;}
		unsigned total() const {return m_total;}
		
	protected:
		ProgressEvent(const Aphid::AtomicString& type, bool lengthComputable, unsigned loaded, unsigned total);
		
		bool m_lengthComputable;
		unsigned m_loaded;
		unsigned m_total;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class EventListener : public ATF::RefCounted<EventListener>, public _LinkElement<EventListener> {
		typedef EventListener Self;
	public:
		virtual ~EventListener();
		
		virtual void handleEvent(Event* event) = 0;
		virtual AJEventListener* toAJEventListener() {return 0;}
		
	protected:
		EventListener();
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	//TODO: only support single listener in current impl, may support multiple listeners in a future release
	class EventTarget {
		typedef ATF::HashMap<unsigned, RefPtr<EventListener> > ListenerMap;
		typedef _LinkedList<EventListener*> ListenerList;
	public:
		virtual ~EventTarget();
		
		void ref() { refEventTarget(); }
		void deref() { derefEventTarget(); }
		
		virtual Node* toNode() {return 0;}
		virtual XMLHttpRequest* toXHR() {return 0;}
		
		unsigned flags() const {return m_flags;}
		
		EventListener* findEventListener(EventFlag flag);
		void registerEventListener(EventFlag flag, ATF::PassRefPtr<EventListener> listener);
		bool unregisterEventListeners(EventFlag flag);
		void clearEventListeners();
		
		void markEventListeners(AJ::MarkStack& markStack, unsigned markID);
		
		virtual EventTarget* nextResponder() {return 0;}
		virtual TouchEventTarget* toTouchEventTarhet() {return 0;}
		
	protected:
		EventTarget()
		: m_flags(0)
		, m_listeners(0)
		{}
		
		virtual void refEventTarget() = 0;
		virtual void derefEventTarget() = 0;
		
		void ensureListenerMap();
		
		unsigned m_flags;
		ListenerMap* m_listeners;
		ListenerList* m_listenerList;
	};
}


#endif
