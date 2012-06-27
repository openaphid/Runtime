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

namespace Aphid {
	PlatformTouchEvent* TouchDispatcher::toPlatformTouchEvent(int eventHash, long eventTime)
	{
		if (!m_touchEvent || m_touchEvent->identifier() != eventHash) {
			oa_debug("new motion event: %p", eventHash);
			m_touchEvent = PlatformTouchEvent::create();
			m_touchEvent->setIdentifier(eventHash);
		}
		return m_touchEvent.get();
	}

	PassRefPtr<PlatformTouch> TouchDispatcher::toPlatformTouch(JNIEnv* env, jobject jtouch)
	{
		Director* director = Director::sharedDirector();
		ASSERT(director);

		unsigned identifier = JNI::callMethod<jint>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getIdentifier);
		RefPtr<PlatformTouch> ret = PlatformTouch::create();
		ret->setIdentifier(identifier);

		ret->setScreenLocation(
				director->convertToGL(
						PointMake(
								JNI::callMethod<jfloat>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getScreenX),
								JNI::callMethod<jfloat>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getScreenY)
										)
										)
										);

		ret->setClientLocation(
				director->convertToGL(
						PointMake(
								JNI::callMethod<jfloat>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getClientX),
								JNI::callMethod<jfloat>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getClientY)
										)
										)
										);

		ret->setTimestamp(JNI::callMethod<jint>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getTimestamp));

		return ret.release();		
	}

	PlatformTouch* TouchDispatcher::mapTouch(JNIEnv* env, jobject jtouch)
	{
		Director* director = Director::sharedDirector();
		ASSERT(director);

		PlatformTouch* ret = 0;
		unsigned identifier = JNI::callMethod<jint>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getIdentifier);

		PlatformTouchMap::iterator it = m_touchMap.find(identifier);
		if (it != m_touchMap.end())
			ret = it->second.get();
		else {
			RefPtr<PlatformTouch> t = toPlatformTouch(env, jtouch);
			m_touchMap.add(identifier, t);
			ret = t.get();
		}

		return ret;
	}

	PassRefPtr<PlatformTouch> TouchDispatcher::unmapTouch(JNIEnv* env, jobject jtouch)
	{
		unsigned identifier = JNI::callMethod<jint>(env, jtouch, JNI::Cache::s_aphidtouch_jmethod_getIdentifier);
		PlatformTouchMap::iterator it = m_touchMap.find(identifier);
		if (it != m_touchMap.end()) {
			RefPtr<PlatformTouch> touch = it->second;
			m_touchMap.remove(it);
			return touch.release();
		} else {
			oa_debug("Failed to unmap touch: %d", identifier);
			return toPlatformTouch(env, jtouch); //TODO: should be backported to iOS version
		}
	}

	void TouchDispatcher::handleAndroidSingleTouch(JNIEnv* env, int eventHash, long eventTime, EventFlag flag,
			jobject jtouch)
	{
		Director* director = Director::sharedDirector();
		if (!director)
			return;

		PlatformTouchEvent* platformEvent = toPlatformTouchEvent(eventHash, eventTime);

		PlatformTouchVector touches;

		if (flag == EventFlagTouchStart || flag == EventFlagTouchMove) {
			PlatformTouch* touch = mapTouch(env, jtouch);
			platformEvent->addTouch(touch);
			touches.append(touch);
		} else {
			RefPtr<PlatformTouch> touch = unmapTouch(env, jtouch);
			platformEvent->removeTouch(touch.get());
			touches.append(touch.release());
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

		TouchEventTarget* target = touches.at(0)->target();
		if (target) {
			platformEvent->setTarget(target);
			target->handleTouchEvent(flag, touches, platformEvent);
			platformEvent->setTarget(0);
		}
	}
}
