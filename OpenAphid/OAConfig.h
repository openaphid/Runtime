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

#ifndef OpenAphid_OAConfig_h
#define OpenAphid_OAConfig_h

#include <wtf/UnusedParam.h>

#define OA_NATIVE_BENCHMARK 0

#define OA_TARGET_IOS 1
#define OA_TARGET_ANDROID 0

#if DEBUG
#define OA_DEV 1
#else
#define OA_DEV 0
#endif

#if OA_DEV
#define OA_RELEASE_INLINE
#else
//#define OA_RELEASE_INLINE __attribute__ ((always_inline))
#define OA_RELEASE_INLINE inline __attribute__ ((always_inline))
#endif

#if OA_DEV
#define ENABLE_LEAK_DETECT 1
#else
#define ENABLE_LEAK_DETECT 0
#endif

//controls the marking process in ActionManager, should be turned off currently
#define OA_MARK_ACTION_OBJECTS 0

//flags for performance experiments, only enabled to collect performance metrics
#define OA_PERF_SCHEDULER_GC 0

#define OA_MAX_CONCURRENT_DOWNLOAD 4

#if ENABLE_LEAK_DETECT
#define LEAK_DETECT_INC(x) RefLeakCounter::increaseRef((x))
#define LEAK_DETECT_DEC(x) RefLeakCounter::decreaseRef((x))
//TODO: use a separate leak detector for JS objects
#define AJ_LEAK_DETECT_INC(x) RefLeakCounter::increaseRef((x))
#define AJ_LEAK_DETECT_DEC(x) RefLeakCounter::decreaseRef((x))
#define ALIVE_REPORT() RefLeakCounter::reportAlives()
#else
#define LEAK_DETECT_INC(x) do {} while (0)
#define LEAK_DETECT_DEC(x) do {} while (0)
#define AJ_LEAK_DETECT_INC(x) do {} while (0)
#define AJ_LEAK_DETECT_DEC(x) do {} while (0)
#define LEAK_REPORT() do {} while (0)
#define ALIVE_REPORT() do {} while (0)
#endif

#if ENABLE_LEAK_DETECT
namespace Aphid {
	
	class RefLeakCounter {
	public:
		~RefLeakCounter();
		
		static void increaseRef(const char* name);
		static void decreaseRef(const char* name);
		static void reportAlives();
	protected:
		explicit RefLeakCounter(const char* name)
		: m_count(0)
		, m_description(name)
		{
			
		}
		void report();
		void increment() {m_count++;}
		void decrement() {m_count--;}
	private:
		int m_count;
		const char* m_description;
	};
}
#endif

#define RELEASE_ASSERT(assertion) do \
if (!(assertion)) { \
WTFReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #assertion); \
CRASH(); \
} \
while (0)

#define RELEASE_ASSERT_NOT_REACHED() do { \
WTFReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, 0); \
CRASH(); \
} while (0)

#endif
