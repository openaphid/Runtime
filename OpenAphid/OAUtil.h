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

#ifndef OpenAphid_OAUtil_h
#define OpenAphid_OAUtil_h

#include "OAConfig.h"

#include <mach/mach_time.h>
#include <string>
#include <ctime>

namespace Aphid {	
	//TODO: P3 ObjC logging support
	
#define OA_FUNCTION 0
//#define OA_FUNCTION __FUNCTION__
	//#define OA_FUNCTION __PRETTY_FUNCTION__
	
#if OA_DEV
#define oa_debug(...) oa_log_debug("DEBUG", __FILE__, __LINE__, OA_FUNCTION, __VA_ARGS__)
#define oa_info(...) oa_log_debug("INFO", __FILE__, __LINE__, OA_FUNCTION, __VA_ARGS__)
#define oa_warn(...) oa_log_debug("WARN", __FILE__, __LINE__, OA_FUNCTION, __VA_ARGS__)
#define oa_error(...) oa_log_debug("ERROR", __FILE__, __LINE__, OA_FUNCTION, __VA_ARGS__)
#else
#define oa_debug(...) do {} while (0)
#define oa_info(...) oa_log("INFO", __VA_ARGS__)
#define oa_warn(...) oa_log("WARN", __VA_ARGS__)
#define oa_error(...) oa_log("ERROR", __VA_ARGS__)
#endif
	
	void oa_log_debug(const char* flag, const char* file, int line, const char* function, const char* format, ...);
	void oa_log(const char* flag, const char* format, ...);
	
#define OA_TODO() oa_log_debug("TODO", __FILE__, __LINE__, __PRETTY_FUNCTION__, "work in progress");
	
#define OA_FREE(x) if ((x)) {free((x)); x=0;}
#define OA_DELETE(x) if (x) {delete (x); x=0;}
	
#define EXPAND_MACRO(x) EXPAND_MACRO_(x)  
#define EXPAND_MACRO_(x) #x
	
	class StopWatch {
	public:
		
#if OA_PERF_SCHEDULER_GC
		static unsigned s_exp_id;
		static unsigned s_exp_count;
#endif
		
		StopWatch() 
		: m_start(mach_absolute_time())
		, m_checkpoint(mach_absolute_time())
		{
			
		}
		
		double checkpoint(bool fromStart=false)
		{
			uint64_t elapse = mach_absolute_time() - (fromStart ? m_start : m_checkpoint);
			
			static mach_timebase_info_data_t s_oa_timebase_info;
			if (s_oa_timebase_info.numer == 0) 
				mach_timebase_info(&s_oa_timebase_info);
			uint64_t elapseNano = elapse * s_oa_timebase_info.numer / s_oa_timebase_info.denom;
			m_checkpoint = mach_absolute_time();
			return elapseNano / 1000000.0;
		}
		
		~StopWatch()
		{
		}
	private:		
		uint64_t m_start;
		uint64_t m_checkpoint;
	};
	
	
}


#endif
