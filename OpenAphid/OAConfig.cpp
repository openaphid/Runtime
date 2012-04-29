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
#include "OAConfig.h"
#include "OAUtil.h"

#if ENABLE(LEAK_DETECT)
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "wtf/HashMap.h"
#include "StringHash.h"
#include "wtf/RefCountedLeakCounter.h"
#include "wtf/text/WTFString.h"
#include "CString.h"
#include <vector>
#include <strings.h>
#endif


namespace Aphid {
#if ENABLE(LEAK_DETECT)
	using ATF::HashMap;
	class RefLeakCounter;
	
	typedef HashMap<Aphid::String, RefLeakCounter* >  LeakHash;
	static LeakHash s_leak_hash;
	
	static bool compareKeys(const String* k1, const String* k2) {
		//return k1->length() > k2->length();
		return strcmp(k1->utf8().data(), k2->utf8().data()) < 0;
	}
	
	RefLeakCounter::~RefLeakCounter()	
	{
		report();
		ATF::deleteAllValues(s_leak_hash);
		s_leak_hash.clear();
	}
	
	void RefLeakCounter::increaseRef(const char* name)
	{
		LeakHash::iterator it = s_leak_hash.find(name);
		RefLeakCounter* c;
		if (it == s_leak_hash.end()) {
			c = new RefLeakCounter(name);
			s_leak_hash.add(name, c);
		} else
			c = it->second;
		c->increment();
	}
	
	void RefLeakCounter::decreaseRef(const char* name)
	{
		LeakHash::iterator it = s_leak_hash.find(name);
		ASSERT(it != s_leak_hash.end());
		it->second->decrement();
	}	
	
	void RefLeakCounter::report()
	{
		if (m_count != 0)
			oa_info("ALIVE: %d\t%s", m_count, m_description);
	}
	
	void RefLeakCounter::reportAlives()
	{
		typedef std::vector<String*> KeysVector;
		KeysVector keys;
		for (LeakHash::iterator it = s_leak_hash.begin(); it != s_leak_hash.end(); ++it)
			keys.push_back(&(it->first));
		
		std::sort(keys.begin(), keys.end(), compareKeys);
		
		for (KeysVector::iterator it = keys.begin(); it != keys.end(); ++it) {
			String* s = *it;
			s_leak_hash.find(*s)->second->report();
		}
	}
#endif

}
