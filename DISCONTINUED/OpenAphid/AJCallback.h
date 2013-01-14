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

#ifndef OpenAphid_AJCallback_h
#define OpenAphid_AJCallback_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <runtime/WeakGCPtr.h>
#include <runtime/AJObject.h>
#include "DirectWrapper.h"

#include "LinkedList.h"
#include "TimerTarget.h"
#include "PendingObject.h"

#include "OAConfig.h"
#include "OAData.h"

namespace Aphid {
	class CallAJFunctionAction;
	
	class MappedAJObject : public ATF::RefCounted<MappedAJObject>/*, public _LinkElement<MappedAJObject>*/ {
		typedef MappedAJObject Self;
	public:
		virtual ~MappedAJObject();
		
		static PassRefPtr<Self> create(AJ::AJObject* object)
		{
			return adoptRef(new Self(object));
		}
		
		AJ::AJObject* optObject() const {return m_weakObject.get();}
		
		AJ::AJObject* forceObject() const {return m_weakObject.forceGet();}		
		
		void markObject(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		MappedAJObject(AJ::AJObject* object);
		
		AJ::WeakGCPtr<AJ::AJObject> m_weakObject;
		unsigned m_markID;
	};
	
	class ManagedAJObject : public RefCounted<ManagedAJObject>, public _LinkElement<ManagedAJObject> {
		typedef ManagedAJObject Self;
	public:
		virtual ~ManagedAJObject();
		
		static PassRefPtr<Self> create(AJ::AJObject* object)
		{
			return adoptRef(new Self(object));
		}
		
		AJ::AJObject* optObject() const {return m_weakObject.get();}
		
		AJ::AJObject* forceObject() const {return m_weakObject.forceGet();}		
		
		void markObject(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		ManagedAJObject(AJ::AJObject* object);
		
		AJ::WeakGCPtr<AJ::AJObject> m_weakObject;
		unsigned m_markID;
	};
	
	class ManagedPendingObject : public ATF::RefCounted<ManagedPendingObject>, public _LinkElement<ManagedPendingObject> {
		typedef ManagedPendingObject Self;
	public:
		virtual ~ManagedPendingObject();
		
		static PassRefPtr<Self> create(PendingObject* object)
		{
			return adoptRef(new Self(object));
		}
		
		PendingObject* pendingObject() const {return m_object;}
		
	protected:
		ManagedPendingObject(PendingObject* object);
		
		PendingObject* m_object;
	};
	
	class AJObjectManager : public RefCounted<AJObjectManager> {
		typedef _LinkedList<ManagedAJObject* > ManagedAJObjectList;
		typedef _LinkedList<ManagedPendingObject* > ManagedPendingObjectList;
		typedef _LinkedList<CallAJFunctionAction* > CallAJFunctionActionList;
		
		typedef ATF::HashMap<AJ::AJObject*, MappedAJObject* > MappedAJObjectMap; //NOTES: holds weak references of values
		typedef ATF::HashMap<AJ::AJObject*, ManagedAJObject* > ManagedAJObjectMap; //NOTES: holds weak references of values
		typedef ATF::HashMap<PendingObject*, RefPtr<ManagedPendingObject> > ManagedPendingObjectMap; //NOTES: ManagedPendingObjectMap holds strong references of values
	public:
		virtual ~AJObjectManager();
		
		static AJObjectManager* sharedManager();
		
		PassRefPtr<MappedAJObject> putMapped(AJ::AJObject* object);
		void dismissMapped(AJ::AJObject* object);
		
		PassRefPtr<ManagedAJObject> putManaged(AJ::AJObject* object);
		void dismissManaged(AJ::AJObject* object);
		
		void putPending(PendingObject* o);
		void dismissPending(PendingObject* o);
		
		void putCallAJFunctionAction(CallAJFunctionAction *action);
		void dismissCallAJFunctionAction(CallAJFunctionAction *action);
		
		void markObjects(AJ::MarkStack& markStack, unsigned markID);		
		
	protected:
		AJObjectManager();
		
		static RefPtr<AJObjectManager> s_shared_manager;
		
		MappedAJObjectMap m_mappedMap;
		//MappedAJObjectList m_slotList; //Notes: using m_slotList improves the performance of marking stage by 1.1%, but it doesn't solve the cyclic reference problem between native and js values.
		ManagedAJObjectMap m_managedMap;
		ManagedAJObjectList m_managedList;
		ManagedPendingObjectMap m_pendingMap;
		ManagedPendingObjectList m_pendingList;
		CallAJFunctionActionList m_callFuncList;
		int m_updateCallbackCount;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class AJTimerTarget : public TimerTarget {
		typedef TimerTarget Base;
	public:		
		virtual ~AJTimerTarget() {}
		
		virtual void update(TimeSec dt);
		
		virtual void setAJUpdateCallback(AJ::ExecState* exec, AJ::AJObject* callback);		
		AJ::AJObject* jsUpdateCallback() const;
		
	protected:
		AJTimerTarget() {}
		
		void markTimerTargetCallbacks(AJ::MarkStack& markStack, unsigned markID)
		{
			if (m_function)
				m_function->markObject(markStack, markID);
		}
		
	private:		
		//ATF::RefPtr<AJUpdateCallback> m_callback;
		RefPtr<MappedAJObject> m_function;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	/// AJTimerCallback
	class AJTimerCallback : public ATF::FastAllocBase {
	public:		
		explicit AJTimerCallback()
		: m_cFunction(0)
		, m_isAJFunction(false)
		{
		}
		
		explicit AJTimerCallback(TimerFunction function)
		: m_cFunction(function)
		, m_isAJFunction(false)
		{}
		
		explicit AJTimerCallback(AJ::AJObject* ajFunction);
		
		explicit AJTimerCallback(const AJTimerCallback& o)
		: m_cFunction(o.m_cFunction)
		, m_ajFunction(o.m_ajFunction)
		, m_isAJFunction(o.m_isAJFunction)
		{}
		
		bool isNativeCallback() const {return !m_isAJFunction;}
		bool isAJCallback() const {return m_isAJFunction;}
		
		TimerFunction nativeFunction() const {return m_isAJFunction ? 0 : m_cFunction;}
		AJ::AJObject* ajFunction() const {return m_isAJFunction?m_ajFunction->optObject() : 0;}
		
		bool isValid() const 
		{
			return (m_isAJFunction && m_ajFunction) || m_cFunction;
		}
		
		void fireTimer(TimerTarget* target, TimeSec dt);
		
		void markTimerCallback(AJ::MarkStack& markStack, unsigned markID)
		{
			if (m_ajFunction)
				m_ajFunction->markObject(markStack, markID);
		}
		
		bool operator==(const AJTimerCallback& b) const
		{
			return m_isAJFunction == b.m_isAJFunction && m_cFunction == b.m_cFunction && m_ajFunction == b.m_ajFunction;
		}
		
	private:
		//TODO: should use a union here
		TimerFunction m_cFunction;
		RefPtr<ManagedAJObject> m_ajFunction;
		bool m_isAJFunction;
	};
}

#endif
