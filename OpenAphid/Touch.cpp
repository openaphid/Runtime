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

#include "OAUtil.h"
#include "AJTouchEvent.h"
#include "AJTouch.h"
#include "AJTouchList.h"

#include "Node.h"

namespace Aphid {
  ///-------------------------------------------------------------------------------------------------------------------
  /// Touch
  Touch::Touch()
  : m_screenLocation(PointZero)
  , m_clientLocation(PointZero)
  , m_timestamp(0)
  , m_id(0)
  {
	  LEAK_DETECT_INC("Touch");
  }

  Touch::~Touch()
  {
	  LEAK_DETECT_DEC("Touch");
  }

  void Touch::markObjects(AJ::MarkStack &markStack, unsigned int markID)
  {
	  if (isMarked(markID))
		  return;
	  DirectWrapper<AJTouch>::markObjects(markStack, markID);
	  if (m_target) {
		  Node *node = m_target->toNode();
		  if (node)
			  node->markObjects(markStack, markID);
	  }
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// TouchList
  TouchList::TouchList()
  {
	  LEAK_DETECT_INC("TouchList");
  }

  TouchList::~TouchList()
  {
	  LEAK_DETECT_DEC("TouchList");
  }

  void TouchList::addTouch(PassRefPtr<Touch> touch)
  {
	  if (touch) {
		  ASSERT(m_touches.find(touch) == ATF::notFound);
		  m_touches.append(touch);
	  }
  }

  Touch *TouchList::identifiedTouch(unsigned int identifier) const
  {
	  for (TouchVector::const_iterator it = m_touches.begin(); it != m_touches.end(); ++it) {
		  if (it->get()->identifier() == identifier)
			  return it->get();
	  }

	  return 0;
  }

  void TouchList::markObjects(AJ::MarkStack &markStack, unsigned int markID)
  {
	  if (isMarked(markID))
		  return;
	  DirectWrapper<AJTouchList>::markObjects(markStack, markID);
	  for (TouchVector::iterator it = m_touches.begin(); it != m_touches.end(); ++it) {
		  it->get()->markObjects(markStack, markID);
	  }
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// TouchEvent
  TouchEvent::TouchEvent(const AtomicString& type)
  : Base(type)
  {
	  LEAK_DETECT_INC("TouchEvent");
  }

  TouchEvent::~TouchEvent()
  {
	  LEAK_DETECT_DEC("TouchEvent");
  }

  const AJ::ClassInfo *TouchEvent::wrapperClassInfo() const
  {
	  return &AJTouchEvent::s_info;
  }

  AJ::AJObject *TouchEvent::createWrapper(AJ::ExecState *exec, AJOAGlobalObject *globalObject)
  {
	  return createAJOAWrapper<AJTouchEvent>(exec, globalObject, this);
  }

  TouchList *TouchEvent::targetTouches()
  {
	  if (!m_targetTouches) {
		  m_targetTouches = TouchList::create();
		  ASSERT(m_touches);

		  size_t len = m_touches->size();
		  for (size_t index = 0; index < len; index++) {
			  Touch *touch = m_touches->at(index);
			  if (touch->target() == this->target())
				  m_targetTouches->addTouch(touch);
		  }
	  }

	  return m_targetTouches.get();
  }

  void TouchEvent::markObjects(AJ::MarkStack &markStack, unsigned int markID)
  {
	  if (isMarked(markID))
		  return;
	  Base::markObjects(markStack, markID);
	  if (m_touches)
		  m_touches->markObjects(markStack, markID);
	  if (m_targetTouches)
		  m_targetTouches->markObjects(markStack, markID);
	  if (m_changedTouches)
		  m_changedTouches->markObjects(markStack, markID);
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// TouchEventTarget
  void TouchEventTarget::handleTouchEvent(Aphid::EventFlag flag, const PlatformTouchVector &touches, Aphid::PlatformTouchEvent *event)
  {
	  if (isTouchEnabled()) { //TODO: is this check necessary?
		  if (flags() & flag) {
			  EventListener *listener = findEventListener(flag);
			  ASSERT(listener);

			  RefPtr<TouchEvent> touchEvent = event->toTouchEvent(flag);

			  touchEvent->setChangedTouches(PlatformTouchEvent::toTouchList(touches));
			  touchEvent->setCurrentTarget(this);

			  //TODO: touch phase?
			  listener->handleEvent(touchEvent.get());

			  if (!touchEvent->isPropagationStopped())
				  event->stopPropagation();

			  touchEvent->setCurrentTarget(0);
		  }
	  }

	  if (!event->isPropagationStopped()) {
		  EventTarget *next = nextResponder();
		  if (next && next->toTouchEventTarhet())
			  next->toTouchEventTarhet()->handleTouchEvent(flag, touches, event);
	  }
  }
}
