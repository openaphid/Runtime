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

#ifndef OpenAphid_Scheduler_h
#define OpenAphid_Scheduler_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "OAData.h"

#include "LinkedList.h"
#include "TimerTarget.h"
#include "AJCallback.h"

namespace Aphid {
	class JSScheduler;
	class TimerTarget;
	class TimerTargetSlot;
	class PendingTimerTargetSlot;
	class TimerUpdateSlot;
	class TimerCallbackSlot;
	
	typedef Vector<RefPtr<PendingTimerTargetSlot> > PendingTimerTargetQueue;
	
	typedef HashMap<TimerTarget*, RefPtr<TimerTargetSlot> > TimerTargetSlotMap;
	
	typedef HashMap<TimerTarget*, RefPtr<TimerUpdateSlot> > TimerUpdateSlotMap;
	typedef _LinkedList<TimerUpdateSlot* > TimerUpdateSlotList;
	
	typedef _LinkedList<TimerTargetSlot* > TimerTargetSlotList;
	typedef _LinkedList<TimerCallbackSlot* > TimerCallbackSlotList;
	typedef Vector<RefPtr<TimerCallbackSlot> > TimerCallbackSlotVector;
	typedef HashMap<TimerTarget*, TimerCallbackSlotVector > TimerCallbackSlotMap;
	
	///-------------------------------------------------------------------------------------------------------------------
	class Scheduler : public RefCounted<Scheduler> {
	public:
		virtual ~Scheduler();
		
		static PassRefPtr<Scheduler> create()
		{
			return adoptRef(new Scheduler());
		}
		
		static Scheduler* sharedScheduler();
		
		static int s_gc_frame_interval;
		static int s_gc_frame_tick;
		
		void scheduleCallback(PassRefPtr<TimerTarget> target, TimerFunction callback, TimeSec interval, bool paused, int repeat=OA_REPEAT_FOREVER, TimeSec delay=0);
		void scheduleAJCallback(PassRefPtr<AJTimerTarget> target, AJ::AJObject* callback, TimeSec interval, bool paused, int repeat=OA_REPEAT_FOREVER, TimeSec delay=0);
		
		void unscheduleCallback(TimerTarget* target, TimerFunction callback);
		void unscheduleCallback(TimerTarget* target, AJ::AJObject* callback);
		void unscheduleCallback(TimerTarget* target, const AJTimerCallback& callback)
		{
			if (callback.isAJCallback())
				unscheduleCallback(target, callback.ajFunction());
			else
				unscheduleCallback(target, callback.nativeFunction());
		}
		void unscheduleCallbacks(TimerTarget* target);		
		
		void scheduleUpdate(PassRefPtr<TimerTarget> target, int priority, bool paused);
		void unscheduleUpdate(TimerTarget* target);
		
		void unscheduleAll();
		
		//TODO: should pause() and resume() be queued events?
		void pause(TimerTarget* target);
		void resume(TimerTarget* target);
		int isPaused(TimerTarget* target);
		
		void tick(TimeSec dt);
		
		void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		Scheduler();
		
		void doScheduleCallback(PassRefPtr<TimerTarget> target, const AJTimerCallback& callback, TimeSec interval, bool paused, int repeat=OA_REPEAT_FOREVER, TimeSec delay=0);
		void doUnscheduleCallback(TimerTarget* target, const AJTimerCallback& callback);
		void doUnscheduleCallbacks(TimerTarget* target);		
		
		void doScheduleUpdate(PassRefPtr<TimerTarget> target, int priority, bool paused);
		void doUnscheduleUpdate(TimerTarget* target);
		
		void doUnscheduleAll();
		
		void doPause(TimerTarget* target);
		void doResume(TimerTarget* target);
		
		void doTick(TimeSec dt);
		
		static void test(); //TODO: remove later
		
	private:
		float m_timeScale;
		
		bool m_updateLocked;
		
		TimerTargetSlotMap m_targetSlotMap;
		TimerTargetSlotList m_targetSlotList;
		
		TimerUpdateSlotMap m_updateSlotMap;
		TimerUpdateSlotList m_update0Slots;
		TimerUpdateSlotList m_updateNegSlots;
		TimerUpdateSlotList m_updatePosSlots;
		
		TimerCallbackSlotList m_callbackSlots;
		TimerCallbackSlotMap m_callbackSlotMap;
		
		PendingTimerTargetQueue m_pendingQueue;
		
		unsigned m_markID;
		
	}; // class Scheduler
	
	typedef enum {
		Slot_Undefined,
		Slot_Schedule_Callback,
		Slot_Unschedule_Callback,
		Slot_Unschedule_Callbacks,
		Slot_Schedule_Update,
		Slot_Unschedule_Update,
		Slot_Unschedule_All,
		Slot_Pause,
		Slot_Resume
	} PendingSchedulerSlotAction;
	
	///-------------------------------------------------------------------------------------------------------------------
	class TimerTargetSlot : public RefCounted<TimerTargetSlot>, public _LinkElement<TimerTargetSlot> {
		friend class Scheduler; //TODO: remove later
	public:
		virtual ~TimerTargetSlot();
		
		static PassRefPtr<TimerTargetSlot> create()
		{
			return adoptRef(new TimerTargetSlot());
		}
	protected:
		TimerTargetSlot();
	private:
		RefPtr<TimerTarget> m_target;
		bool m_paused;
	};
	
	class PendingTimerTargetSlot : public RefCounted<PendingTimerTargetSlot> {
		friend class Scheduler;
	public:
		virtual ~PendingTimerTargetSlot();
		
		static PassRefPtr<PendingTimerTargetSlot> create()
		{
			return adoptRef(new PendingTimerTargetSlot());
		}
	protected:
		PendingTimerTargetSlot();
	private:
		RefPtr<TimerTarget> m_target;		
		bool m_paused;
		
		int m_priority;
		
		AJTimerCallback m_callback;
		TimeSec m_interval;
		TimeSec m_delay;
		int m_repeat;
		
		PendingSchedulerSlotAction m_action;
	};
	
	class TimerUpdateSlot : public RefCounted<TimerUpdateSlot>, public _LinkElement<TimerUpdateSlot> {
		friend class Scheduler;
	public:
		virtual ~TimerUpdateSlot();
		
		static PassRefPtr<TimerUpdateSlot> create()
		{
			return adoptRef(new TimerUpdateSlot());
		}
		
	protected:
		TimerUpdateSlot();
	private:
		TimerTargetSlot* m_targetSlot;
		int m_priority;
	};
	
	class TimerCallbackSlot : public RefCounted<TimerCallbackSlot>, public _LinkElement<TimerCallbackSlot> {
		friend class Scheduler;
	public:
		virtual ~TimerCallbackSlot();
		static PassRefPtr<TimerCallbackSlot> create()
		{
			return adoptRef(new TimerCallbackSlot());
		}
		
	protected:
		TimerCallbackSlot();
	private:
		TimerTargetSlot* m_targetSlot;
		AJTimerCallback m_callback;
		TimeSec m_interval;
		TimeSec m_elapsed;
		TimeSec m_delay;
		int m_repeat;
	};
	
}// namespace Aphid

#endif
