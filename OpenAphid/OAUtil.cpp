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

#include "OAUtil.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#if PLATFORM(ANDROID)
#include "OAAndroidLog.h"
#endif

namespace Aphid {
	
#if !PLATFORM(ANDROID)
	static inline char* oa_log_level_str(OALogLevel flag)
	{
		char* flagStr = 0;
		switch (flag) {
		case OALogLevel_DEBUG:
			flagStr = "DEBUG";
			break;
		case OALogLevel_INFO:
			flagStr = "INFO";
			break;
		case OALogLevel_WARN:
			flagStr = "WARN";
			break;
		case OALogLevel_ERROR:
			flagStr = "ERROR";
			break;
		case OALogLevel_TODO:
			flagStr = "TODO";
			break;
		default:
			flagStr = "Unknown";
			break;
		}
		return flagStr;
	}
#endif
	
#if PLATFORM(ANDROID)
	static inline android_LogPriority convert_android_log_level(OALogLevel flag)
	{
		android_LogPriority level = ANDROID_LOG_DEBUG;
		switch (flag) {
		case OALogLevel_DEBUG:
			level = ANDROID_LOG_DEBUG;
			break;
		case OALogLevel_INFO:
			level = ANDROID_LOG_INFO;
			break;
		case OALogLevel_WARN:
			level = ANDROID_LOG_WARN;
			break;
		case OALogLevel_ERROR:
			level = ANDROID_LOG_ERROR;
			break;
		case OALogLevel_TODO:
			level = ANDROID_LOG_WARN;
			break;
		default:
			level = ANDROID_LOG_DEBUG;
			break;
		}
		return level;
	}
#endif
	
	void oa_log_debug(OALogLevel flag, const char* file, int line, const char* function, const char* format, ...)
	{
#if !PLATFORM(ANDROID)
		time_t now = time(NULL);
		char nowstr[64];
		
		strftime(nowstr, 64, "%D,%T", gmtime(&now));
		
		if (file) {//TODO: maybe error prone, only works for UNIX file separator.
			file = strrchr(file, '/');
			if (file)
				file = file + 1;
		}
		
		char* flagStr = oa_log_level_str(flag);
#endif
		
#if PLATFORM(ANDROID)
		android_LogPriority androidLevel = convert_android_log_level(flag);
#endif
		
		if (function) {
#if PLATFORM(ANDROID)
			__android_log_print(androidLevel, AJ_ANDROID_TAG, "%s:%d[%s]: ", file, line, function);
#else
			printf("%s %s %s:%d[%s]: ", flagStr, nowstr, file, line, function);
#endif
		}
		else {
#if PLATFORM(ANDROID)
			__android_log_print(androidLevel, AJ_ANDROID_TAG, "%s:%d: ", file, line);
#else
			printf("%s %s %s:%d: ", flagStr, nowstr, file, line);
#endif
		}
		
		va_list argList;
		va_start(argList, format);
#if PLATFORM(ANDROID)
		__android_log_vprint(androidLevel, AJ_ANDROID_TAG, format, argList);
#else
		vfprintf(stdout, format, argList);
#endif
		va_end(argList);
		
#if !PLATFORM(ANDROID)
		puts("");
		fflush(stdout);		
#endif
	}

	void oa_log(OALogLevel flag, const char* format, ...)
	{
		va_list argList;
		
#if !PLATFORM(ANDROID)
		time_t now = time(NULL);
		char nowstr[64];
		
		char* flagStr = oa_log_level_str(flag);
		strftime(nowstr, 64, "%D,%T", gmtime(&now));
#endif		
		
#if PLATFORM(ANDROID)
		android_LogPriority androidLevel = convert_android_log_level(flag);
#endif
		
		
		
#if PLATFORM(ANDROID)
		//__android_log_print(androidLevel, AJ_ANDROID_TAG, "%s %s: ", flagStr, nowstr);
#else
		printf("%s %s: ", flagStr, nowstr);
#endif
		
		va_start(argList, format);
#if PLATFORM(ANDROID)
		__android_log_vprint(androidLevel, AJ_ANDROID_TAG, format, argList);
#else
		vfprintf(stdout, format, argList);
#endif
		va_end(argList);
		
#if !PLATFORM(ANDROID)
		puts("");
		fflush(stdout);		
#endif
	}
	
#if OA_PERF_SCHEDULER_GC	
	unsigned StopWatch::s_exp_id = 0;
	unsigned StopWatch::s_exp_count = 0;
#endif
}
