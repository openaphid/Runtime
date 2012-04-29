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

#ifndef OpenAphid_AtomicStringHash_h
#define OpenAphid_AtomicStringHash_h

#include <AtomicString.h>
#include <wtf/HashTraits.h>

namespace Aphid {
	struct AtomicStringHash {
		static unsigned hash(const AtomicString& key)
		{
			return key.impl()->existingHash();
		}
		
		static bool equal(const AtomicString& a, const AtomicString& b)
		{
			return a == b;
		}
		
		static const bool safeToCompareToEmptyOrDeleted = false;
	};
}

namespace ATF {
	template<> struct HashTraits<Aphid::AtomicString> : GenericHashTraits<Aphid::AtomicString> {
		static const bool emptyValueIsZero = true;
		static void constructDeletedValue(Aphid::AtomicString& slot) { new (&slot) Aphid::AtomicString(HashTableDeletedValue); }
		static bool isDeletedValue(const Aphid::AtomicString& slot) { return slot.isHashTableDeletedValue(); }
	};
}

#endif
