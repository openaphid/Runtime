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
#include "Scheduler.h"

#include <runtime/AJLock.h>
#include "AJCallback.h"

#include "OAUtil.h"
#include "OAGlobalObject.h"
#include "AJOAGlobalObject.h"

namespace Aphid {
	using namespace AJ;
	///-------------------------------------------------------------------------------------------------------------------
	Scheduler::Scheduler()
	: m_timeScale(1.0f)
	, m_markID(0)
	{
		//TODO:
		LEAK_DETECT_INC("Scheduler");
	}
	Scheduler::~Scheduler()
	{
		//TODO:
		LEAK_DETECT_DEC("Scheduler");
	}
	
	Scheduler* Scheduler::sharedScheduler()
	{
		Scheduler* scheduler = OAGlobalObject::sharedInstance()->namespaceG2D()->scheduler();
		ASSERT(scheduler);
		return scheduler;
	}
	
	void Scheduler::scheduleCallback(PassRefPtr<TimerTarget> prpTarget, TimerFunction callback, TimeSec interval, bool paused, int repeat, TimeSec delay)
	{
		if (repeat == 0) {
			oa_debug("repeat is 0?");
			return;
		}
		
		RefPtr<TimerTarget> target = prpTarget;
		ASSERT(target && callback && interval >= 0);
		
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_callback = AJTimerCallback(callback);
		slot->m_interval = interval;		
		slot->m_paused = paused;
		slot->m_repeat = repeat;
		slot->m_delay = delay;
		slot->m_action = Slot_Schedule_Callback;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::scheduleAJCallback(PassRefPtr<AJTimerTarget> prpTarget, AJ::AJObject* callback, TimeSec interval, bool paused, int repeat, TimeSec delay)
	{
		if (repeat == 0) {
			oa_debug("repeat is 0?");
			return;
		}
		
		RefPtr<TimerTarget> target = prpTarget;
		ASSERT(target && callback && interval >= 0);
		
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_callback = AJTimerCallback(callback);
		slot->m_interval = interval;		
		slot->m_paused = paused;
		slot->m_repeat = repeat;
		slot->m_delay = delay;
		slot->m_action = Slot_Schedule_Callback;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::unscheduleCallback(TimerTarget* target, TimerFunction callback) 
	{
		ASSERT(target && callback);
		
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_callback = AJTimerCallback(callback);
		slot->m_action = Slot_Unschedule_Callback;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::unscheduleCallback(Aphid::TimerTarget *target, AJ::AJObject *callback)
	{
		ASSERT(target && callback);
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_callback = AJTimerCallback(callback);
		slot->m_action = Slot_Unschedule_Callback;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::unscheduleCallbacks(TimerTarget* target) {
		ASSERT(target);
		
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_action = Slot_Unschedule_Callbacks;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::scheduleUpdate(PassRefPtr<TimerTarget> prpTarget, int priority, bool paused)
	{
		RefPtr<TimerTarget> target = prpTarget;
		
		ASSERT(target);
		
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_priority = priority;
		slot->m_paused = paused;
		slot->m_action = Slot_Schedule_Update;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::unscheduleUpdate(TimerTarget* target)
	{
		ASSERT(target);
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_target = target;
		slot->m_action = Slot_Unschedule_Update;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::unscheduleAll()
	{
		RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
		slot->m_action = Slot_Unschedule_All;
		m_pendingQueue.append(slot.release());
	}
	
	void Scheduler::pause(TimerTarget* target) 
	{
		ASSERT(target);
		
		doPause(target);
		
		if (!m_pendingQueue.isEmpty()) {
			RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
			slot->m_target = target;
			slot->m_action = Slot_Pause;
			m_pendingQueue.append(slot.release());
		}
	}
	
	void Scheduler::resume(TimerTarget* target)
	{
		ASSERT(target);
		
		doResume(target);
		
		if (!m_pendingQueue.isEmpty()) {
			RefPtr<PendingTimerTargetSlot> slot = PendingTimerTargetSlot::create();
			slot->m_target = target;
			slot->m_action = Slot_Resume;
			m_pendingQueue.append(slot.release());
		}
	}
	
	int Scheduler::isPaused(TimerTarget* target)
	{
		ASSERT(target);
		TimerTargetSlotMap::iterator it = m_targetSlotMap.find(target);
		if (it != m_targetSlotMap.end())
			return it->second->m_paused;
		else
			return -1;
	}
	
	void Scheduler::test()
	{
		ALIVE_REPORT();
	}
	
	int Scheduler::s_gc_frame_interval = 1;
	int Scheduler::s_gc_frame_tick = 0;
	
#define OA_REPEAT_ENDS 0
	void Scheduler::tick(TimeSec dt)
	{
		m_updateLocked = true;
		
		{//apply pending schedule/unschedule first
			size_t size = m_pendingQueue.size();
			if (size > 0) {
				for (size_t i = 0; i < size; i++) {
					RefPtr<PendingTimerTargetSlot> slot = m_pendingQueue.at(i);
					switch(slot->m_action) {
						case Slot_Schedule_Callback:
							doScheduleCallback(slot->m_target, slot->m_callback, slot->m_interval, slot->m_paused, slot->m_repeat, slot->m_delay);
							break;
						case Slot_Unschedule_Callback:
							doUnscheduleCallback(slot->m_target.get(), slot->m_callback);
							break;
						case Slot_Unschedule_Callbacks:
							doUnscheduleCallbacks(slot->m_target.get());
							break;
						case Slot_Schedule_Update:
							doScheduleUpdate(slot->m_target, slot->m_priority, slot->m_paused);
							break;
						case Slot_Unschedule_Update:
							doUnscheduleUpdate(slot->m_target.get());
							break;
						case Slot_Unschedule_All:
							doUnscheduleAll();
							break;
						case Slot_Pause:
							doPause(slot->m_target.get());
							break;
						case Slot_Resume:
							doResume(slot->m_target.get());
							break;
						default:
							oa_error("Unknown slot action:%d", slot->m_action);
							ASSERT_NOT_REACHED();
							break;
					}
				}
				m_pendingQueue.remove(0, size);
			}
		}
		
		dt = m_timeScale * dt;
		
		AJOAGlobalObject* globalObject = OAGlobalObject::sharedInstance()->wrapper();
		RELEASE_ASSERT(globalObject);
		AJLock lock(globalObject->globalExec());
		
#if OA_PERF_SCHEDULER_GC
		StopWatch::s_exp_id = Heap::s_mark_id;
		StopWatch::s_exp_count = 0;
		StopWatch watch;
		const int loop = 100;
		for (int i = 0; i < 100; i++)
			doTick(dt);
		oa_info("doTick() %d times: %.1fms ", loop, watch.checkpoint());
		for (int i = 0; i < loop; i++)
			OAGlobalObject::sharedInstance()->garbageCollection(); //TODO: evaluate the performance impact later
		oa_info("gc %d times: %.1fms ", loop, watch.checkpoint());
		oa_info("gc times: %d, exp_count: %d", Heap::s_mark_id - StopWatch::s_exp_id, StopWatch::s_exp_count);
#else
		doTick(dt);
		
		s_gc_frame_tick++;
		if (s_gc_frame_tick >= s_gc_frame_interval) {
			OAGlobalObject::sharedInstance()->garbageCollection(); //TODO: evaluate the performance impact later
			s_gc_frame_tick = 0;
		}
#endif
				
		//TODO: remove later as it's present to prevent test() being stripped
		if (dt > 10)
			test();		
		
		m_updateLocked = false;
	}
	
	void Scheduler::doTick(TimeSec dt)
	{
		{
			TimerUpdateSlot* cursor = m_updateNegSlots.head();
			while (cursor) {
				if (!cursor->m_targetSlot->m_paused)
					cursor->m_targetSlot->m_target->update(dt);
				cursor = cursor->next();
			}
			
			cursor = m_update0Slots.head();
			while (cursor){
				if (!cursor->m_targetSlot->m_paused)
					cursor->m_targetSlot->m_target->update(dt);
				cursor = cursor->next();
			}
			
			cursor = m_updatePosSlots.head();
			while (cursor){
				if (!cursor->m_targetSlot->m_paused)
					cursor->m_targetSlot->m_target->update(dt);
				cursor = cursor->next();
			}
		}
		
		{
			TimerCallbackSlot* cursor = m_callbackSlots.head();
			while (cursor) {
				if (!cursor->m_targetSlot->m_paused) {
					if (cursor->m_delay > 0) //TODO: the whole logic can be optimized by using a priority queue/heap
						cursor->m_delay -= dt;
					if (cursor->m_delay <= 0) {
						cursor->m_elapsed += dt;
						if (cursor->m_elapsed >= cursor->m_interval) {
							if (cursor->m_repeat != OA_REPEAT_FOREVER)
								cursor->m_repeat--;
							if (cursor->m_repeat == OA_REPEAT_ENDS)
								unscheduleCallback(cursor->m_targetSlot->m_target.get(), cursor->m_callback);
							cursor->m_callback.fireTimer(cursor->m_targetSlot->m_target.get(), cursor->m_elapsed);
							cursor->m_elapsed = 0;
						}						
						
					}					
				}
				cursor = cursor->next();
			}
		}
	}
	
	void Scheduler::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (m_markID == markID)
			return;
		m_markID = markID;
		//TODO mark pending and callbacks?
		TimerTargetSlot* cursor = m_targetSlotList.head();
		while (cursor) {
			Node * node = cursor->m_target->toNode();
			if (node && !node->isMarked(markID))
				node->markObjects(markStack, markID);
			cursor = cursor->next();
		}
		
		if (!m_pendingQueue.isEmpty()) {
			for (PendingTimerTargetQueue::iterator it = m_pendingQueue.begin(); it != m_pendingQueue.end(); ++it) {
				PendingTimerTargetSlot* slot = it->get();
				if (slot->m_target) {
					Node* node = slot->m_target->toNode();
					if (node && !node->isMarked(markID))
						node->markObjects(markStack, markID);
				}
			}
		}
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	// Protected methods
	void Scheduler::doScheduleCallback(PassRefPtr<TimerTarget> prpTarget, const AJTimerCallback& callback, TimeSec interval, bool paused, int repeat, TimeSec delay)
	{
		RefPtr<TimerTarget> target = prpTarget;
		
		ASSERT(target && callback.isValid());
		
		RefPtr<TimerTargetSlot> targetSlot = 0;
		
		TimerTargetSlotMap::iterator tsIt = m_targetSlotMap.find(target.get());
		
		if (tsIt == m_targetSlotMap.end()) {
			targetSlot = TimerTargetSlot::create();
			targetSlot->m_target = target;
			targetSlot->m_paused = paused;
			m_targetSlotMap.add(target.get(), targetSlot);
			m_targetSlotList.append(targetSlot.get());
		} else {
			targetSlot = tsIt->second;
			ASSERT(targetSlot->m_paused == paused);
		}
		
		ASSERT(targetSlot);
		
		TimerCallbackSlotMap::iterator csIt = m_callbackSlotMap.find(target.get());
		if (csIt == m_callbackSlotMap.end()) 
			csIt = m_callbackSlotMap.add(target.get(), TimerCallbackSlotVector()).first; //TODO: caused an additional value copy, remove it later
		
		TimerCallbackSlotVector& queue = csIt->second;
		
		TimerCallbackSlotVector::iterator it = queue.begin();
		for (; it != queue.end(); ++it) {
			TimerCallbackSlot* slot = it->get();
			if (slot->m_callback == callback) {
				slot->m_interval = interval;
				slot->m_elapsed = 0;
				slot->m_repeat = repeat < OA_REPEAT_FOREVER ? OA_REPEAT_FOREVER : repeat;
				slot->m_delay = delay;
				break;
			}
		}
		
		if (it == queue.end()) {
			RefPtr<TimerCallbackSlot> slot = TimerCallbackSlot::create();
			slot->m_targetSlot = targetSlot.get();
			slot->m_callback = callback;
			slot->m_interval = interval;
			slot->m_delay = delay;
			slot->m_repeat = repeat;
			queue.append(slot);
			
			m_callbackSlots.append(slot.get());
		}
		
	}
	
	void Scheduler::doUnscheduleCallback(TimerTarget* target, const AJTimerCallback& callback)
	{
		ASSERT(target && callback.isValid());
		
		TimerCallbackSlotMap::iterator csIt = m_callbackSlotMap.find(target);
		if (csIt != m_callbackSlotMap.end()) {
			TimerCallbackSlotVector& queue = csIt->second;
			TimerCallbackSlotVector::iterator it = queue.begin();
			for (; it != queue.end(); ++it) {
				if (it->get()->m_callback == callback)
					break;
			}
			
			if (it != queue.end()) {
				m_callbackSlots.remove(it->get());
				queue.remove(std::distance(queue.begin(), it));
				if (queue.isEmpty() && m_updateSlotMap.find(target) == m_updateSlotMap.end()) {
					oa_debug("remove target: %p", target);
					m_callbackSlotMap.remove(csIt);
					TimerTargetSlotMap::iterator targetIt = m_targetSlotMap.find(target);
					if (targetIt != m_targetSlotMap.end()) {
						m_targetSlotList.remove(targetIt->second.get());
						m_targetSlotMap.remove(targetIt);
					}
				}
			} else
				oa_error("Can't find the scheduled callback for target %p", target);
		} else 
			oa_warn("Can't find the scheduled target %p with callbacks", target);
	}
	
	void Scheduler::doUnscheduleCallbacks(TimerTarget* target)
	{
		ASSERT(target);
		TimerCallbackSlotMap::iterator csIt = m_callbackSlotMap.find(target);
		if (csIt != m_callbackSlotMap.end()) {
			TimerCallbackSlotVector& queue = csIt->second;
			for (TimerCallbackSlotVector::iterator it = queue.begin(); it != queue.end(); ++it)
				m_callbackSlots.remove(it->get());
			queue.clear();
			m_callbackSlotMap.remove(csIt);
			if (m_updateSlotMap.find(target) == m_updateSlotMap.end()) {
				oa_debug("remove target in unscheduleCallbacks: %p", target);
				TimerTargetSlotMap::iterator targetIt = m_targetSlotMap.find(target);
				if (targetIt != m_targetSlotMap.end()) {
					m_targetSlotList.remove(targetIt->second.get());
					m_targetSlotMap.remove(targetIt);
				}
			}
		} else
			oa_debug("Can't unschedule callbacks for target: %p", target);
		
		doUnscheduleUpdate(target);
	}
	
	void Scheduler::doScheduleUpdate(PassRefPtr<TimerTarget> prpTarget, int priority, bool paused)
	{
		RefPtr<TimerTarget> target = prpTarget;
		
		if (m_updateSlotMap.find(target.get()) != m_updateSlotMap.end()) {
			oa_error("Can't re-schedule a target: %p", target.get());
		} else {
			RefPtr<TimerTargetSlot> targetSlot = 0;
			TimerTargetSlotMap::iterator it = m_targetSlotMap.find(target.get());
			if (it != m_targetSlotMap.end()) {
				targetSlot = it->second;
				targetSlot->m_paused = paused;
			}
			else {
				targetSlot = TimerTargetSlot::create();
				targetSlot->m_target = target;
				targetSlot->m_paused = paused;
				m_targetSlotMap.add(target.get(), targetSlot);
				m_targetSlotList.append(targetSlot.get());
			}
			
			ASSERT(targetSlot);
			
			RefPtr<TimerUpdateSlot> updateSlot = TimerUpdateSlot::create();
			updateSlot->m_targetSlot = targetSlot.get();
			updateSlot->m_priority = priority;
			
			m_updateSlotMap.add(target.get(), updateSlot.get());
			
			if (priority == 0) 
				m_update0Slots.append(updateSlot.get());
			else {
				TimerUpdateSlotList* list = 0;
				if (priority < 0)
					list = &m_updateNegSlots;				
				else
					list = &m_updatePosSlots;
				
				TimerUpdateSlot* cursor = list->head();
				while (cursor) {
					if (cursor->m_priority > priority)
						break;
					cursor = cursor->next();
				}
				list->insertBefore(cursor, updateSlot.get());
			}
		}
	}
	
	void Scheduler::doUnscheduleUpdate(TimerTarget* target)
	{
		ASSERT(target);
		
		TimerUpdateSlotMap::iterator usIt = m_updateSlotMap.find(target);
		if (usIt != m_updateSlotMap.end()) {
			RefPtr<TimerUpdateSlot> slot = usIt->second;
			int priority = slot->m_priority;
			TimerUpdateSlotList* list = 0;
			if (priority < 0)
				list = &m_updateNegSlots;
			else if (priority == 0)
				list = &m_update0Slots;
			else
				list = &m_updatePosSlots;
			
			list->remove(slot.get());
			m_updateSlotMap.remove(target);
			
			if (m_callbackSlotMap.find(target) == m_callbackSlotMap.end()) {
				oa_debug("Remove target %p", target);
				TimerTargetSlotMap::iterator targetIt = m_targetSlotMap.find(target);
				if (targetIt != m_targetSlotMap.end()) {
					m_targetSlotList.remove(targetIt->second.get());
					m_targetSlotMap.remove(targetIt);
				}
			}
			
		} else 
			oa_debug("Can't find the scheduled update target: %p", target);
	}
	
	void Scheduler::doUnscheduleAll()
	{
		m_updateNegSlots.clear();
		m_update0Slots.clear();
		m_updatePosSlots.clear();			
		m_updateSlotMap.clear();
		
		m_callbackSlots.clear();
		m_callbackSlotMap.clear();
		
		m_targetSlotMap.clear();
		m_targetSlotList.clear();
		
		oa_debug("Unscheduled all");		
	}
	
	void Scheduler::doPause(TimerTarget* target) 
	{
		ASSERT(target);
		TimerTargetSlotMap::iterator it = m_targetSlotMap.find(target);
		if (it != m_targetSlotMap.end())
			it->second->m_paused = true;
	}
	
	void Scheduler::doResume(TimerTarget* target)
	{
		ASSERT(target);
		
		TimerTargetSlotMap::iterator it = m_targetSlotMap.find(target);
		if (it != m_targetSlotMap.end())
			it->second->m_paused = false;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TimerTargetSlot
	TimerTargetSlot::TimerTargetSlot()
	: m_target(0)
	, m_paused(false)
	{
		LEAK_DETECT_INC("TimerTargetSlot");
	}
	
	TimerTargetSlot::~TimerTargetSlot()
	{
		LEAK_DETECT_DEC("TimerTargetSlot");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// PendingTimerTargetSlot
	
	PendingTimerTargetSlot::PendingTimerTargetSlot()
	: m_target(0)
	, m_paused(false)
	, m_priority(0)
	, m_interval(0)
	, m_action(Slot_Undefined)
	{
		LEAK_DETECT_INC("PendingTimerTargetSlot");
	}
	
	PendingTimerTargetSlot::~PendingTimerTargetSlot()
	{
		LEAK_DETECT_DEC("PendingTimerTargetSlot");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TimerUpdateSlot
	TimerUpdateSlot::TimerUpdateSlot()
	: m_targetSlot(0)
	, m_priority(0)
	{
		LEAK_DETECT_INC("TimerUpdateSlot");
	}
	
	TimerUpdateSlot::~TimerUpdateSlot()
	{
		LEAK_DETECT_DEC("TimerUpdateSlot");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TimerCallbackSlot
	TimerCallbackSlot::TimerCallbackSlot()
	: m_targetSlot(0)
	, m_interval(0)
	, m_elapsed(0)
	{
		LEAK_DETECT_INC("TimerCallbackSlot");
	}
	
	TimerCallbackSlot::~TimerCallbackSlot()
	{
		LEAK_DETECT_DEC("TimerCallbackSlot");
	}
	
} //namespace Aphid
