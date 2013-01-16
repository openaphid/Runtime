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
#include "AJCallback.h"

#include <runtime/AJObject.h>
#include <runtime/AJCell.h>
#include <runtime/AJValue.h>
#include <runtime/AJLock.h>
#include <runtime/InternalFunction.h>
#include <Collector.h>

#include "OAGlobalObject.h"
#include "AJOAGlobalObject.h"
#include "AJNode.h"
#include "ActionInstant.h"

#include "OAUtil.h"

namespace Aphid {
  using namespace AJ;

  ///-------------------------------------------------------------------------------------------------------------------
  /// MappedAJObject
  MappedAJObject::MappedAJObject(AJObject *object)
  : m_weakObject(object)
  , m_markID(0)
  {
	  LEAK_DETECT_INC("MappedAJObject");
  }

  MappedAJObject::~MappedAJObject()
  {
	  LEAK_DETECT_DEC("MappedAJObject");
	  if (m_weakObject.forceGet())
		  AJObjectManager::sharedManager()->dismissMapped(m_weakObject.forceGet());
  }

  void MappedAJObject::markObject(AJ::MarkStack &markStack, unsigned markID)
  {
	  if (m_markID == markID)
		  return;
	  m_markID = markID;
	  if (m_weakObject.forceGet())
		  markStack.append(m_weakObject.forceGet());
	  else
		  oa_debug("no need to markObject?!");
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// ManagedAJObject
  ManagedAJObject::ManagedAJObject(AJObject *object)
  : m_weakObject(object)
  , m_markID(0)
  {
	  LEAK_DETECT_INC("ManagedAJObject");
  }

  ManagedAJObject::~ManagedAJObject()
  {
	  LEAK_DETECT_DEC("ManagedAJObject");
	  if (m_weakObject.forceGet())
		  AJObjectManager::sharedManager()->dismissManaged(m_weakObject.forceGet());
  }

  void ManagedAJObject::markObject(AJ::MarkStack &markStack, unsigned markID)
  {
	  if (m_markID == markID)
		  return;
	  m_markID = markID;
	  if (m_weakObject.forceGet())
		  markStack.append(m_weakObject.forceGet());
	  else
		  oa_debug("no need to markObject?!");
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// ManagedPendingObject
  ManagedPendingObject::ManagedPendingObject(PendingObject *object)
  : m_object(object)
  {
	  LEAK_DETECT_INC("ManagedPendingObject");
  }

  ManagedPendingObject::~ManagedPendingObject()
  {
	  LEAK_DETECT_DEC("ManagedPendingObject");
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// AJObjectManager
  AJObjectManager::AJObjectManager()
  {
	  oa_debug("new AJObjectManager %p", this);
  }

  AJObjectManager::~AJObjectManager()
  {
	  //ASSERT_NOT_REACHED();
  }

  RefPtr<AJObjectManager> AJObjectManager::s_shared_manager = 0;

  AJObjectManager *AJObjectManager::sharedManager()
  {
	  if (!s_shared_manager)
		  s_shared_manager = adoptRef(new AJObjectManager());
	  return s_shared_manager.get();
  }

  PassRefPtr<MappedAJObject> AJObjectManager::putMapped(AJ::AJObject *object)
  {
	  if (object) {
		  MappedAJObjectMap::iterator it = m_mappedMap.find(object);
		  if (it == m_mappedMap.end()) {
			  RefPtr<MappedAJObject> slot = MappedAJObject::create(object);
			  m_mappedMap.add(object, slot.get());
			  return slot.release();
		  } else
			  return it->second; //TODO: may be incorrect?
	  }

	  return 0;
  }

  void AJObjectManager::dismissMapped(AJ::AJObject *object)
  {
	  if (object) {
		  MappedAJObjectMap::iterator it = m_mappedMap.find(object);
		  if (it != m_mappedMap.end()) {
			  m_mappedMap.remove(it);
		  }
	  }
  }

  PassRefPtr<ManagedAJObject> AJObjectManager::putManaged(AJ::AJObject *object)
  {
	  if (object) {
		  ManagedAJObjectMap::iterator it = m_managedMap.find(object);
		  if (it == m_managedMap.end()) {
			  RefPtr<ManagedAJObject> slot = ManagedAJObject::create(object);
			  m_managedMap.add(object, slot.get());
			  m_managedList.append(slot.get());
			  return slot.release();
		  } else
			  return it->second; //TODO: may be incorrect?
	  }

	  return 0;
  }

  void AJObjectManager::dismissManaged(AJ::AJObject *object)
  {
	  if (object) {
		  ManagedAJObjectMap::iterator it = m_managedMap.find(object);
		  if (it != m_managedMap.end()) {
			  m_managedList.remove(it->second);
			  m_managedMap.remove(it);
		  }
	  }
  }

  void AJObjectManager::putPending(PendingObject *object)
  {
	  if (object) {
		  ManagedPendingObjectMap::iterator it = m_pendingMap.find(object);
		  if (it == m_pendingMap.end()) {
			  RefPtr<ManagedPendingObject> slot = ManagedPendingObject::create(object);
			  m_pendingList.append(slot.get());
			  m_pendingMap.add(object, slot.release());
		  }
	  }
  }

  void AJObjectManager::dismissPending(PendingObject *object)
  {
	  if (object) {
		  ManagedPendingObjectMap::iterator it = m_pendingMap.find(object);
		  if (it != m_pendingMap.end()) {
			  m_pendingList.remove(it->second.get());
			  m_pendingMap.remove(it);
		  }
	  }
  }

  void AJObjectManager::putCallAJFunctionAction(Aphid::CallAJFunctionAction *action)
  {
	  if (action) {
		  ASSERT(!action->prev() && !action->next() && action != m_callFuncList.head());
		  m_callFuncList.append(action);
	  }
  }

  void AJObjectManager::dismissCallAJFunctionAction(Aphid::CallAJFunctionAction *action)
  {
	  if (action)
		  m_callFuncList.remove(action);
  }

  void AJObjectManager::markObjects(AJ::MarkStack &markStack, unsigned markID)
  {
	  {//mark managed list
		  ManagedAJObject *cursor = m_managedList.head();
		  while (cursor) {
			  cursor->markObject(markStack, markID);
			  cursor = cursor->next();
		  }
	  }

	  {//mark pending list
		  ManagedPendingObject *cursor = m_pendingList.head();
		  while (cursor) {
			  cursor->pendingObject()->markWhenPending(markStack, markID);
			  cursor = cursor->next();
		  }
	  }

	  {//mark CallAJFunctionAction
		  CallAJFunctionAction *cursor = m_callFuncList.head();
		  while (cursor) {
			  cursor->markObjects(markStack, markID);
			  cursor = cursor->next();
		  }
	  }
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// AJTimerTarget
  AJObject *AJTimerTarget::jsUpdateCallback() const
  {
	  return m_function ? m_function->optObject() : 0;
  }

  void AJTimerTarget::setAJUpdateCallback(AJ::ExecState *exec, AJ::AJObject *callback)
  {
	  UNUSED_PARAM(exec);

	  if (callback) {
		  m_function = AJObjectManager::sharedManager()->putMapped(callback);
	  }
	  else {
		  m_function = 0;
	  }
  }

  void AJTimerTarget::update(TimeSec dt)
  {
	  //Base::update(dt);
	  if (m_function) {
		  AJOAGlobalObject *globalObject = OAGlobalObject::sharedInstance()->wrapper();
		  ASSERT(globalObject);

		  ExecState *exec = globalObject->globalExec();

		  AJValue jsTarget = toAJ(exec, globalObject, this->toNode());
		  AJObject *callback = m_function->forceObject();
		  ASSERT(callback);

		  CallData callData;
		  CallType callType = callback->getCallData(callData);

		  if (callType != CallTypeNone) {
			  ref();

			  MarkedArgumentBuffer args;
			  args.append(jsTarget);
			  args.append(jsNumber(exec, dt));

			  AJ::call(exec, callback, callType, callData, jsTarget, args);

			  if (exec->hadException())
				  reportAJException(exec, exec->exception());

			  deref();
		  } else
			  oa_error("Invalid call type for invokeUpdate: %d", callType);
	  }
  }

  ///-------------------------------------------------------------------------------------------------------------------
  AJTimerCallback::AJTimerCallback(AJObject *ajFunction)
  : m_cFunction(0)
  , m_isAJFunction(true)
  {
	  ASSERT(ajFunction);
	  m_ajFunction = AJObjectManager::sharedManager()->putManaged(ajFunction);
  }

  void AJTimerCallback::fireTimer(Aphid::TimerTarget *target, TimeSec dt)
  {
	  if (m_isAJFunction) {
		  if (m_ajFunction) {
			  AJOAGlobalObject *globalObject = OAGlobalObject::sharedInstance()->wrapper();
			  ASSERT(globalObject);
			  ASSERT(target->toNode()); //TODO maybe a false assumption?

			  ExecState *exec = globalObject->globalExec();

			  AJValue jsTarget = toAJ(exec, globalObject, target->toNode());
			  AJObject *callback = m_ajFunction->optObject();
			  ASSERT(callback);

			  CallData callData;
			  CallType callType = callback->getCallData(callData);

			  if (callType != CallTypeNone) {
				  MarkedArgumentBuffer args; //TODO: re-think the arguments, should them be wrapped in an event?
				  args.append(jsTarget);
				  args.append(callback);
				  args.append(jsNumber(exec, dt));

				  AJValue ret = AJ::call(exec, callback, callType, callData, jsTarget, args);

				  if (exec->hadException())
					  reportAJException(exec, exec->exception());
				  else {
					  if (ret.isBoolean() && !ret.toBoolean(exec))
						  Scheduler::sharedScheduler()->unscheduleCallback(target, callback);
				  }
			  } else
				  oa_error("Invalid call type for invokeUpdate: %d", callType);
		  }
	  } else if (m_cFunction)
		  m_cFunction(target, dt);
  }
}
