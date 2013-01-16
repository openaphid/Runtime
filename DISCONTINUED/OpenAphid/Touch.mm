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
	/// TouchDispatcher
	static inline unsigned NSTimeToMS(NSTimeInterval t)
	{
		return (unsigned)(t * 1000.0);
	}

	//TODO should handle touch.phase
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
	
	void TouchDispatcher::handleUITouches(NSSet *ts, UIEvent *event, Aphid::EventFlag flag)
	{
		if ([ts count] == 0)
			return;
		
		Director* director = Director::sharedDirector();
		if (!director)
			return;
		
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
	
}
