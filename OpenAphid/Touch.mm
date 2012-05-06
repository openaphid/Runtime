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
#include "Touch.h"
#include "Director.h"
#include "AJEventListener.h"

#import <UIKit/UIView.h>

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// PlatformTouch
	PlatformTouch::PlatformTouch()
	: m_screenLocation(PointZero)
	, m_clientLocation(PointZero)
	, m_timestamp(0)
	, m_id(0)
	{
		LEAK_DETECT_INC("PlatformTouch");
	}
	
	PlatformTouch::~PlatformTouch()
	{
		LEAK_DETECT_DEC("PlatformTouch");
	}
	
	ATF::PassRefPtr<Touch> PlatformTouch::toTouch()
	{
		RefPtr<Touch> touch = Touch::create();
		
		touch->setIdentifier(m_id);
		touch->setScreenLocation(m_screenLocation);
		touch->setClientLocation(m_clientLocation);
		touch->setTarget(m_target);
		touch->setTimestamp(m_timestamp);
		
		
		return touch.release();
	}
	
	void PlatformTouch::markTouchObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		markEventTarget(m_target.get(), markStack, markID);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// PlatformTouchEvent
	PlatformTouchEvent::PlatformTouchEvent()
	: m_timestamp(0)
	, m_id(0)
	, m_propagationStopped(false)
	{
		LEAK_DETECT_INC("PlatformTouchEvent");
	}
	
	PlatformTouchEvent::~PlatformTouchEvent()
	{
		LEAK_DETECT_DEC("PlatformTouchEvent");
	}
	
	bool PlatformTouchEvent::addTouch(PassRefPtr<Aphid::PlatformTouch> prpTouch)
	{
		RefPtr<PlatformTouch> touch = prpTouch;
		ASSERT(touch);
		
		if (m_allTouches.find(touch) == ATF::notFound) {
			m_allTouches.append(touch);
			return true;
		}
		
		return false;
	}
	
	bool PlatformTouchEvent::removeTouch(Aphid::PlatformTouch *touch)
	{
		ASSERT(touch);
		
		size_t index = m_allTouches.find(touch);
		if (index != ATF::notFound) {
			m_allTouches.remove(index);
			return true;
		}
		
		return false;			
	}
	
	PassRefPtr<TouchEvent> PlatformTouchEvent::toTouchEvent(EventFlag flag)
	{
		AtomicString type;
		switch (flag) {
			case EventFlagTouchStart:
				type = EventNames::touchstartEvent;
				break;
			case EventFlagTouchMove:
				type = EventNames::touchmoveEvent;
				break;
			case EventFlagTouchEnd:
				type = EventNames::touchendEvent;
				break;
			case EventFlagTouchCancel:
				type = EventNames::touchcancelEvent;
				break;				
			default:
				oa_error("Unknown event flag: %d", flag);
				ASSERT_NOT_REACHED();
				break;
		}
		
		RefPtr<TouchEvent> event = TouchEvent::create(type);
		
		event->setTarget(m_target);
		event->setTimestamp(m_timestamp);
		event->setTouches(toTouchList(m_allTouches));		
		
		return event.release();
	}
	
	void PlatformTouchEvent::markTouchObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		markEventTarget(m_target.get(), markStack, markID);
		
		for (PlatformTouchVector::iterator it = m_allTouches.begin(); it != m_allTouches.end(); ++it)
			it->get()->markTouchObjects(markStack, markID);
	}
	
	ATF::PassRefPtr<TouchList> PlatformTouchEvent::toTouchList(const PlatformTouchVector &platformTouches)
	{
		RefPtr<TouchList> touches = TouchList::create();
		for (PlatformTouchVector::const_iterator it = platformTouches.begin(); it != platformTouches.end(); ++it) {
			touches->addTouch(it->get()->toTouch());
		}
		
		return touches.release();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TouchDispatcher
	static inline unsigned NSTimeToMS(NSTimeInterval t)
	{
		return (unsigned)(t * 1000.0);
	}
	
	RefPtr<TouchDispatcher> TouchDispatcher::s_dispatcher = 0;
	
	TouchDispatcher* TouchDispatcher::sharedDispatcher()
	{
		if (!s_dispatcher)
			s_dispatcher = adoptRef(new TouchDispatcher());
		return s_dispatcher.get();
	}
	
	PlatformTouch* TouchDispatcher::mapTouch(UITouch *touch)
	{
		Director* director = Director::sharedDirector();
		ASSERT(director);
		PlatformTouchMap::iterator it = m_touchMap.find((unsigned)touch);
		PlatformTouch* ret = 0;
		if (it != m_touchMap.end()) {
			ret = it->second.get();
		} else {
			RefPtr<PlatformTouch> t = PlatformTouch::create();
			t->setIdentifier((unsigned)touch);
			m_touchMap.add((unsigned)touch, t);
			ret = t.get();
		}
		
		ret->setScreenLocation(director->convertToGL([touch locationInView:nil]));
		ret->setClientLocation(director->convertToGL([touch locationInView:director->glView()]));
		ret->setTimestamp(NSTimeToMS(touch.timestamp));
		
		return ret;
	}
	
	PassRefPtr<PlatformTouch> TouchDispatcher::unmapTouch(UITouch *touch)
	{
		PlatformTouchMap::iterator it = m_touchMap.find((unsigned)touch);
		if (it != m_touchMap.end()) {
			RefPtr<PlatformTouch> touch = it->second;
			m_touchMap.remove(it);
			return touch.release();
		}
		
		return 0;
	}
	
	PlatformTouchEvent* TouchDispatcher::toPlatformTouchEvent(UIEvent *event)
	{
		Director* director = Director::sharedDirector();
		ASSERT(director);
		
		if (!m_touchEvent || m_touchEvent->identifier() != (unsigned)event) {
			oa_debug("new ui event: %p", event);
			m_touchEvent = PlatformTouchEvent::create();
			m_touchEvent->setIdentifier((unsigned)event);
			
			for (UITouch* t in [event touchesForView:director->glView()]) {
				PlatformTouch* touch = mapTouch(t);
				m_touchEvent->addTouch(touch);
			}
		}
		
		return m_touchEvent.get();
	}
	
	
	void TouchDispatcher::dispatchMultiTouches(const PlatformTouchVector &touches, PlatformTouchEvent* platformEvent, Aphid::EventFlag flag)
	{
		typedef HashMap<TouchEventTarget*, PlatformTouchVector* > TargetTouchesMap;
		TargetTouchesMap map;
		
		for (PlatformTouchVector::const_iterator it = touches.begin(); it != touches.end(); ++it) {
			PlatformTouch* touch = it->get();
			TouchEventTarget* target = touch->target();
			if (target) {
				std::pair<TargetTouchesMap::iterator, bool> result = map.add(target, 0);
				PlatformTouchVector*& entry = result.first->second;
				const bool isNew = result.second;
				if (isNew)
					entry = new PlatformTouchVector();
				
				ASSERT(entry->find(touch) == ATF::notFound);
				entry->append(touch);
			}
		}
		
		for (TargetTouchesMap::iterator it = map.begin(); it != map.end(); ++it) {
			TouchEventTarget* target = it->first;
			PlatformTouchVector* entry = it->second;
			platformEvent->setTarget(target);
			target->handleTouchEvent(flag, *entry, platformEvent);
			platformEvent->setTarget(0);
		}
		
		ATF::deleteAllValues(map);
	}
	
	void TouchDispatcher::handleUITouches(NSSet *ts, UIEvent *event, Aphid::EventFlag flag)
	{
		if ([ts count] == 0)
			return;
		
		Director* director = Director::sharedDirector();
		ASSERT(director);
		
		PlatformTouchEvent* platformEvent = toPlatformTouchEvent(event);
		
		UIView* view = director->glView();
		
		PlatformTouchVector touches;
		
		for (UITouch* t in ts) { 
			if (flag == EventFlagTouchStart || flag == EventFlagTouchMove) {
				PlatformTouch* touch = mapTouch(t);
				platformEvent->addTouch(touch);
				touches.append(touch);
			} else {
				RefPtr<PlatformTouch> touch = unmapTouch(t);
				platformEvent->removeTouch(touch.get());
				touches.append(touch.release());
			}
		}
		
		if (flag == EventFlagTouchStart) {
			Scene* scene = director->runningScene();
			if (scene) {
				for (PlatformTouchVector::const_iterator it = touches.begin(); it != touches.end(); ++it) {
					PlatformTouch* touch = it->get();
					TouchEventTarget* target = scene->hitTest(scene->convertToNodeSpace(touch->clientLocation()), platformEvent);
					touch->setTarget(target);
				}
			}
		}
		
		if (view.multipleTouchEnabled)
			dispatchMultiTouches(touches, platformEvent, flag);
		else {
			TouchEventTarget* target = touches.at(0)->target();
			if (target) {
				platformEvent->setTarget(target);
				target->handleTouchEvent(flag, touches, platformEvent);
				platformEvent->setTarget(0);
			}
		}
		
		/*
		if (flag == EventFlagTouchEnd || flag == EventFlagTouchCancel) {
			for (PlatformTouchVector::const_iterator it = touches.begin(); it != touches.end(); ++it)
				platformEvent->removeTouch(it->get());
		}*/
	}
	
	void TouchDispatcher::markTouchObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (m_touchEvent)
			m_touchEvent->markTouchObjects(markStack, markID);
		for (PlatformTouchMap::iterator it = m_touchMap.begin(); it != m_touchMap.end(); ++it) {
			it->second->markTouchObjects(markStack, markID);
		}
	}
	
}
