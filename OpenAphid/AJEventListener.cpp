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

#include <runtime/AJObject.h>
#include <runtime/AJValue.h>
#include <runtime/AJLock.h>

#include "AJEventListener.h"
#include "OAGlobalObject.h"
#include "AJOAGlobalObject.h"
#include "AJEvent.h"
#include "AJNode.h"
#include "AJXMLHttpRequest.h"

namespace Aphid {
  using namespace AJ;

  AJEventListener::AJEventListener(AJ::AJObject *ajCallback)
  {
	  LEAK_DETECT_INC("AJEventListener");
	  m_callback = AJObjectManager::sharedManager()->putMapped(ajCallback);
  }

  AJEventListener::~AJEventListener()
  {
	  LEAK_DETECT_DEC("AJEventListener");
  }

  PassRefPtr<AJEventListener> AJEventListener::create(AJ::ExecState *exec, AJ::AJValue listener)
  {
	  if (!listener.isObject())
		  return 0;
	  return create(exec, asObject(listener));
  }

  PassRefPtr<AJEventListener> AJEventListener::create(AJ::ExecState *exec, AJ::AJObject *listener)
  {
	  UNUSED_PARAM(exec);

	  ASSERT(listener);

	  //TODO: stricter type checking?
	  return adoptRef(new AJEventListener(listener));
  }

  AJValue AJEventListener::findAJCallback(Aphid::EventListener *listener)
  {
	  if (!listener || !listener->toAJEventListener())
		  return jsNull();

	  AJEventListener *ajListener = listener->toAJEventListener();
	  return ajListener->callback();
  }

  void AJEventListener::setAJListener(Aphid::EventFlag flag, Aphid::EventTarget *target, AJ::ExecState *exec, AJ::AJValue value)
  {
	  if (value.isObject()) {
		  target->registerEventListener(flag, AJEventListener::create(exec, asObject(value)));
	  } else
		  target->unregisterEventListeners(flag);
  }

  void AJEventListener::handleEvent(Aphid::Event *event)
  {
	  AJOAGlobalObject *globalObject = OAGlobalObject::sharedInstance()->wrapper();
	  RELEASE_ASSERT(globalObject);

	  ExecState *exec = globalObject->globalExec();

	  AJLock lock(exec);

	  AJObject *callback = m_callback->optObject();

	  RELEASE_ASSERT(callback);

	  AJValue function = callback->get(exec, Identifier(exec, "handleEvent"));
	  CallData callData;
	  CallType callType = function.getCallData(callData);
	  if (callType == CallTypeNone) {
		  function = AJValue();
		  callType = callback->getCallData(callData);
	  }

	  if (callType != CallTypeNone) {
		  ref();

		  MarkedArgumentBuffer args;
		  args.append(toAJ(exec, globalObject, event));

		  //TODO: DynamicGlobalObjectScope?
		  AJValue ret = function ? call(exec, function, callType, callData, callback, args) : call(exec, callback, callType, callData, toAJ(exec, globalObject, event->currentTarget()), args);

		  if (exec->hadException())
			  reportAJException(exec, exec->exception());
		  else {
			  bool retVal;
			  if (ret.getBoolean(retVal) && !retVal)
				  event->preventDefault();
		  }

		  deref();
	  } else {

	  }
  }

  void AJEventListener::markCallback(AJ::MarkStack &markStack, unsigned int markID)
  {
	  if (m_callback)
		  m_callback->markObject(markStack, markID);
  }


  ///-------------------------------------------------------------------------------------------------------------------
  AJValue toAJ(AJ::ExecState *exec, AJOAGlobalObject *globalObject, EventTarget *target)
  {
	  if (!target) {
		  oa_warn("null target for toJS??");
		  return jsNull();
	  }

	  if (target->toNode())
		  return toAJ(exec, globalObject, target->toNode());

	  if (target->toXHR())
		  return toAJ(exec, globalObject, target->toXHR());

	  oa_error("Unknown type of event target");

	  ASSERT_NOT_REACHED();

	  return jsUndefined();
  }

  void markEventTarget(EventTarget *target, AJ::MarkStack& markStack, unsigned markID)
  {
	  if (target) {
		  if (target->toNode()) {
			  target->toNode()->markObjects(markStack, markID);
			  return;
		  }

		  if (target->toXHR()) {
			  target->toXHR()->markObjects(markStack, markID);
			  return;
		  }

		  oa_error("Unknown type of event target to mark");

		  ASSERT_NOT_REACHED();
	  }
  }
}
