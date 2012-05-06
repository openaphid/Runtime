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

#ifndef OpenAphid_Touch_h
#define OpenAphid_Touch_h

#include <wtf/Vector.h>
#include "Event.h"

#include "OAData.h"
#include "DirectWrapper.h"

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#endif


namespace Aphid {	
	class TouchEventTarget;
	class Scene;
	class TouchDispatcher;
	class AJTouch;
	class AJTouchList;
	
	///-------------------------------------------------------------------------------------------------------------------
	//TODO add previousLocation support in next release
	class Touch : public ATF::RefCounted<Touch>, public DirectWrapper<AJTouch> {
		typedef Touch Self;
	public:
		~Touch();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		float screenX() const {return m_screenLocation.x;}
		float screenY() const {return m_screenLocation.y;}
		Point screenLocation() const {return m_screenLocation;}
		void setScreenLocation(const Point& p) {m_screenLocation = p;}
		
		float clientX() const {return m_clientLocation.x;}
		float clientY() const {return m_clientLocation.y;}
		Point clientLocation() const {return m_clientLocation;}
		void setClientLocation(const Point& p) {m_clientLocation = p;}
		
		float pageX() const {return m_clientLocation.x;}
		float pageY() const {return m_clientLocation.y;}
		
		unsigned identifier() const {return m_id;}
		void setIdentifier(unsigned i) {m_id = i;}
		
		unsigned timestamp() const {return m_timestamp;}
		void setTimestamp(unsigned t) {m_timestamp = t;}
		
		TouchEventTarget* target() const {return m_target.get();}
		void setTarget(PassRefPtr<TouchEventTarget> t) {m_target = t;}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		Touch();
		
		Point m_screenLocation;
		Point m_clientLocation;
		
		unsigned m_id;
		unsigned m_timestamp;
		RefPtr<TouchEventTarget> m_target;
	};
	
	typedef ATF::Vector<RefPtr<Touch> > TouchVector;
	
	///-------------------------------------------------------------------------------------------------------------------
	class TouchList : public ATF::RefCounted<TouchList>, public DirectWrapper<AJTouchList> {
		typedef TouchList Self;
	public:
		~TouchList();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		size_t size() const {return m_touches.size();}
		Touch* at(size_t index) const {return index < m_touches.size() ? m_touches.at(index).get() : 0;}
		Touch* identifiedTouch(unsigned identifier) const;
		void addTouch(PassRefPtr<Touch> touch);
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		TouchList();
		
		TouchVector m_touches;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class TouchEvent : public Event {
		typedef Event Base;
	public:
		~TouchEvent();
		
		static PassRefPtr<TouchEvent> create(const AtomicString& type)
		{
			return adoptRef(new TouchEvent(type));
		}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		TouchList* touches() const {return m_touches.get();}
		void setTouches(PassRefPtr<TouchList> touches) {m_touches = touches;}
		TouchList* targetTouches();
		TouchList* changedTouches() {return m_changedTouches.get();}
		void setChangedTouches(PassRefPtr<TouchList> touches) {m_changedTouches = touches;}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		TouchEvent(const AtomicString& type);
		
		RefPtr<TouchList> m_touches;
		RefPtr<TouchList> m_targetTouches;
		RefPtr<TouchList> m_changedTouches;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class PlatformTouch : public RefCounted<PlatformTouch> {
		typedef PlatformTouch Self;
	public:
		~PlatformTouch();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		Point screenLocation() const {return m_screenLocation;}
		void setScreenLocation(const Point& p) {m_screenLocation = p;}
		
		Point clientLocation() const {return m_clientLocation;}
		void setClientLocation(const Point& p) {m_clientLocation = p;}
		
		unsigned identifier() const {return m_id;}
		void setIdentifier(unsigned i) {m_id = i;}
		
		unsigned timestamp() const {return m_timestamp;}
		void setTimestamp(unsigned t) {m_timestamp = t;}
		
		TouchEventTarget* target() const {return m_target.get();}
		void setTarget(PassRefPtr<TouchEventTarget> t) {m_target = t;}
		
		PassRefPtr<Touch> toTouch();
		
		void markTouchObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		PlatformTouch();
		
		Point m_screenLocation;
		Point m_clientLocation;
		unsigned m_id;
		unsigned m_timestamp;
		RefPtr<TouchEventTarget> m_target;
	};
	
	typedef Vector<RefPtr<PlatformTouch> > PlatformTouchVector;
	
	///-------------------------------------------------------------------------------------------------------------------
	class PlatformTouchEvent : public RefCounted<PlatformTouchEvent> {
		typedef PlatformTouchEvent Self;
		friend class TouchDispatcher;
	public:
		virtual ~PlatformTouchEvent();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		TouchEventTarget* target() const {return m_target.get();}
		void setTarget(PassRefPtr<TouchEventTarget> t) {m_target = t;}
		
		unsigned identifier() const {return m_id;}
		void setIdentifier(unsigned i) {m_id = i;}
		
		unsigned timestamp() const {return m_timestamp;}
		void setTimestamp(unsigned t) {m_timestamp = t;}
		
		const PlatformTouchVector& allTouches() const {return m_allTouches;}
		
		bool isPropagationStopped() const {return m_propagationStopped;}
		void stopPropagation() {m_propagationStopped = true;}
		
		PassRefPtr<TouchEvent> toTouchEvent(EventFlag flag);
		
		void markTouchObjects(AJ::MarkStack& markStack, unsigned markID);
		
		static PassRefPtr<TouchList> toTouchList(const PlatformTouchVector& platformTouches);
		
	protected:
		PlatformTouchEvent();
		
		bool addTouch(PassRefPtr<PlatformTouch> touch);
		bool removeTouch(PlatformTouch* touch);
		
		RefPtr<TouchEventTarget> m_target;
		PlatformTouchVector m_allTouches;
		unsigned m_id;
		unsigned m_timestamp;
		bool m_propagationStopped;
	};

	///-------------------------------------------------------------------------------------------------------------------
	class TouchEventTarget : public EventTarget {
	public:
		virtual ~TouchEventTarget() {}
		
		virtual bool pointInside(const Point& p, PlatformTouchEvent* event)
		{
			UNUSED_PARAM(p);
			UNUSED_PARAM(event);
			return false;
		}
		virtual TouchEventTarget* hitTest(const Point& p, PlatformTouchEvent* event)
		{
			UNUSED_PARAM(p);
			UNUSED_PARAM(event);
			return 0;
		}
		
		virtual void handleTouchEvent(Aphid::EventFlag flag, const PlatformTouchVector &touches, Aphid::PlatformTouchEvent *event);
		
		virtual TouchEventTarget* toTouchEventTarhet() {return this;}
		
		//TODO: need be virtual?
		bool isTouchEnabled() const {return m_touchEnabled;}
		void setTouchEnabled(bool b) {m_touchEnabled = b;}
		
		bool isUserInteractionEnabled() const {return m_userInteractionEnabled;}
		void setUserInteractionEnabled(bool b) {m_userInteractionEnabled = b;}
		
	protected:
		TouchEventTarget()
		: m_touchEnabled(false)
		, m_userInteractionEnabled(true)
		{}
		
	private:
		bool m_touchEnabled;
		bool m_userInteractionEnabled;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class TouchDispatcher : public RefCounted<TouchDispatcher> {
		typedef HashMap<unsigned, RefPtr<PlatformTouch> > PlatformTouchMap;
	public:
		virtual ~TouchDispatcher(){}
		
		static TouchDispatcher* sharedDispatcher();
		
		//Handle touches
#ifdef __OBJC__		
		void handleUITouches(NSSet* ts, UIEvent* event, EventFlag flag);
		
		PlatformTouch* mapTouch(UITouch* touch);
		PassRefPtr<PlatformTouch> unmapTouch(UITouch* touch);
		PlatformTouchEvent* toPlatformTouchEvent(UIEvent* event);
#else
		void handleUITouches(void* ts, void* event, EventFlag flag);
		
		PlatformTouch* mapTouch(void* touch);
		PassRefPtr<PlatformTouch> unmapTouch(void* touch);
		PlatformTouchEvent* toPlatformTouchEvent(void* event);
#endif
		
		void markTouchObjects(AJ::MarkStack& markStack, unsigned markID);
	
	protected:
		TouchDispatcher(){}
		
		static RefPtr<TouchDispatcher> s_dispatcher;
		
		void dispatchMultiTouches(const PlatformTouchVector& touches, PlatformTouchEvent* event, EventFlag flag);
		
		PlatformTouchMap m_touchMap;
		RefPtr<PlatformTouchEvent> m_touchEvent; //TODO: may need a map??
	};
}

#endif
