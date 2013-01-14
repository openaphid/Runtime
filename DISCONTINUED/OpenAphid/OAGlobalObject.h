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

#ifndef OpenAphid_OAGlobalObject_h
#define OpenAphid_OAGlobalObject_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

#include "DirectWrapper.h"

#include "Console.h"
#include "NamespaceAphid.h"
#include "NamespaceCore.h"
#include "NamespaceG2D.h"
#include "NamespaceActions.h"
#if PLATFORM(IPHONE)
#include "NamespaceiOS.h"
#elif PLATFORM(ANDROID)
#include "NamespaceAndroid.h"
#endif
#include "NamespaceExt.h"

#include "OAConfig.h"

namespace Aphid {
	class AJOAGlobalObject;
	
	///-------------------------------------------------------------------------------------------------------------------
	class OAGlobalObject : public RefCounted<OAGlobalObject>, public DirectWrapper<AJOAGlobalObject> {
	public:
		virtual ~OAGlobalObject();
		
		static bool s_explicit_gc;
		
		static bool s_develop_mode;
		
		static void createInstance();
		static OAGlobalObject* sharedInstance();
		static void destroyInstance();
		
		void initialize();
		
		Console* console() const;
		Console* optConsole() const {return m_console.get();}
		
		NamespaceAphid* namespaceAphid() const {return m_namespaceAphid.get();}
		
		NamespaceG2D* namespaceG2D() const {return m_namespaceG2D.get();}
		
		NamespaceCore* namespaceCore() const;
		NamespaceCore* optNamespaceCore() const {return m_namespaceCore.get();}
		
		NamespaceJS* namespaceJS() const;
		NamespaceJS* optNamespaceJS() const {return m_namespaceJS.get();}
		
		NamespaceActions* namespaceActions() const;
		NamespaceActions* optNamespaceActions() const {return m_namespaceActions.get();}
		
		NamespaceExt* namespaceExt() const;
		NamespaceExt* optNamespaceExt() const {return m_namespaceExt.get();}

#if PLATFORM(IPHONE)
		NamespaceiOS* namespaceiOS() const;
		NamespaceiOS* optNamespaceiOS() const {return m_namespaceiOS.get();}
		
		NamespaceExt* namespaceExtiOS() const;
		NamespaceExt* optNamespaceExtiOS() const {return m_namespaceExtiOS.get();}
#elif PLATFORM(ANDROID)
		NamespaceAndroid* namespaceAndroid() const;
		NamespaceAndroid* optNamespaceAndroid() const {return m_namespaceAndroid.get();}
		
		NamespaceExt* namespaceExtAndroid() const;
		NamespaceExt* optNamespaceExtAndroid() const {return m_namespaceExtAndroid.get();}
#endif
		
		bool garbageCollection();
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		struct HeapStatistic {
			uint64_t lastTime;
			AJ::Heap::Statistics stats;
		};
		
	private:
		static RefPtr<OAGlobalObject> s_shared_instance;
		static bool s_destroyed;
		
		OAGlobalObject();
		
		mutable RefPtr<Console> m_console;
		
		RefPtr<NamespaceAphid> m_namespaceAphid;
		RefPtr<NamespaceG2D> m_namespaceG2D;
		
		mutable RefPtr<NamespaceCore> m_namespaceCore;
		mutable RefPtr<NamespaceJS> m_namespaceJS;
		mutable RefPtr<NamespaceActions> m_namespaceActions;
		mutable RefPtr<NamespaceExt> m_namespaceExt;
		
#if PLATFORM(IPHONE)
		mutable RefPtr<NamespaceiOS> m_namespaceiOS;
		mutable RefPtr<NamespaceExt> m_namespaceExtiOS;
#elif PLATFORM(ANDROID)
		mutable RefPtr<NamespaceAndroid> m_namespaceAndroid;
		mutable RefPtr<NamespaceExt> m_namespaceExtAndroid;
#endif
		
		HeapStatistic m_heapStats; //TODO, placeholder for future GC tuning
	}; // class OAGlobalObject
	
}// namespace Aphid

#endif
