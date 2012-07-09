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
#include "OAJNIUtil.h"

namespace Aphid {
	using JNI::Cache;

	PassRefPtr<PlatformTouchEvent> TouchDispatcher::toPlatformTouchEvent(JNIEnv* env, jobject jtouchEvent)
	{
		Director* director = Director::sharedDirector();
		ASSERT(director);

		RefPtr<PlatformTouchEvent> ret = PlatformTouchEvent::create();
		ret->setIdentifier(JNI::callMethod<jint>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getEventIdentifier));
		ret->setTimestamp(JNI::callMethod<jlong>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getEventTime));

		int count = JNI::callMethod<jint>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getTouchCount);

		jintArray jids = (jintArray) JNI::callMethod<jobject>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getIds);
		jfloatArray jscreenXs =
				(jfloatArray) JNI::callMethod<jobject>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getScreenXs);
		jfloatArray jscreenYs =
				(jfloatArray) JNI::callMethod<jobject>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getScreenYs);
		jfloatArray jclientXs =
				(jfloatArray) JNI::callMethod<jobject>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getClientXs);
		jfloatArray jclientYs =
				(jfloatArray) JNI::callMethod<jobject>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getClientYs);

		jint* jptrIds = env->GetIntArrayElements(jids, 0);
		jfloat* jptrScreenXs = env->GetFloatArrayElements(jscreenXs, 0);
		jfloat* jptrScreenYs = env->GetFloatArrayElements(jscreenYs, 0);
		jfloat* jptrClientXs = env->GetFloatArrayElements(jclientXs, 0);
		jfloat* jptrClientYs = env->GetFloatArrayElements(jclientYs, 0);

		for (int i = 0; i < count; i++) {
			int id = jptrIds[i];
			ASSERT(id > 0);

			RefPtr<PlatformTouch> touch = m_touchMap.get(id);
			if (!touch) {
				touch = PlatformTouch::create();
				touch->setIdentifier(id);
				m_touchMap.add(id, touch);
		}

			//TODO: this is a grat opptunity to check moved touches
			touch->setScreenLocation(
					director->convertToGL(
							PointMake(jptrScreenXs[i], jptrScreenYs[i])
									)
									);
			touch->setClientLocation(
					director->convertToGL(
							PointMake(jptrClientXs[i], jptrClientYs[i])
									)
									);
			ret->addTouch(touch);
	}

		env->ReleaseIntArrayElements(jids, jptrIds, 0);
		env->ReleaseFloatArrayElements(jscreenXs, jptrScreenXs, 0);
		env->ReleaseFloatArrayElements(jscreenYs, jptrScreenYs, 0);
		env->ReleaseFloatArrayElements(jclientXs, jptrClientXs, 0);
		env->ReleaseFloatArrayElements(jclientYs, jptrClientYs, 0);
		env->DeleteLocalRef(jids);
		env->DeleteLocalRef(jscreenXs);
		env->DeleteLocalRef(jscreenYs);
		env->DeleteLocalRef(jclientXs);
		env->DeleteLocalRef(jclientYs);
		return ret.release();
	}

	void TouchDispatcher::handleAndroidTouchEvent(JNIEnv* env, jobject jtouchEvent)
	{
		Director* director = Director::sharedDirector();
		if (!director)
			return;

		RefPtr<PlatformTouchEvent> platformEvent = toPlatformTouchEvent(env, jtouchEvent);
		PlatformTouchVector changedTouches;

		const PlatformTouchVector& allTouches = platformEvent->allTouches();

		int actionIndex = JNI::callMethod<jint>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getActionIndex);
		
		EventFlag phase =
				(EventFlag) JNI::callMethod<jint>(env, jtouchEvent, Cache::s_aphidtouchevent_jmethod_getEventPhase);

		switch (phase) {
		case EventFlagTouchStart: {
			RefPtr<PlatformTouch> touch = allTouches.at(actionIndex);
			changedTouches.append(touch);
			Scene* scene = director->runningScene();
			if (scene) {
				TouchEventTarget* target = scene->hitTest(scene->convertToNodeSpace(touch->clientLocation()), platformEvent.get());
					touch->setTarget(target);
				}
			}
			break;
		case EventFlagTouchMove:
			changedTouches.append(allTouches); //TODO: should we compare the new event to the old one to compute a list of moved touches precisely?
			break;
		case EventFlagTouchEnd:
		case EventFlagTouchCancel: {
			RefPtr<PlatformTouch> t = allTouches.at(actionIndex);
			changedTouches.append(t);
			platformEvent->removeTouchAtIndex(actionIndex);
			m_touchMap.remove(t->identifier());
		}
			break;
		default:
			oa_error("Invalid touch phase: %d", phase);
			break;
		}

		if (director->multipleTouchEnabled())
			dispatchMultiTouches(changedTouches, platformEvent.get(), phase);
		else {
			TouchEventTarget* target = changedTouches.at(0)->target();
		if (target) {
			platformEvent->setTarget(target);
				target->handleTouchEvent(phase, changedTouches, platformEvent.get());
			platformEvent->setTarget(0);
			} else
				oa_debug("no touch target");
		}
		
	}//end of TouchDispatcher::handleAndroidTouchEvent

} //namespace Aphid
