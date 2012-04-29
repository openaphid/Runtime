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

#include "OAUtil.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

namespace Aphid {
	void oa_log_debug(const char* flag, const char* file, int line, const char* function, const char* format, ...)
	{
		time_t now = time(NULL);
		char nowstr[64];
		
		strftime(nowstr, 64, "%D,%T", gmtime(&now));
		
		if (file) {//TODO: maybe error prone, only works for UNIX file separator.
			file = strrchr(file, '/');
			if (file)
				file = file + 1;
		}
		if (function)
			printf("%s %s %s:%d[%s]: ", flag, nowstr, file, line, function);
		else
			printf("%s %s %s:%d: ", flag, nowstr, file, line);
		
		va_list argList;
		va_start(argList, format);
		vfprintf(stdout, format, argList);
		va_end(argList);
		
		puts("");
		fflush(stdout);		
	}

	void oa_log(const char* flag, const char* format, ...)
	{
		time_t now = time(NULL);
		char nowstr[64];
		va_list argList;
		
		strftime(nowstr, 64, "%D,%T", gmtime(&now));
		printf("%s %s: ", flag, nowstr);		
		
		va_start(argList, format);
		vfprintf(stdout, format, argList);
		va_end(argList);
		
		puts("");
		fflush(stdout);		
	}
	
#if OA_PERF_SCHEDULER_GC	
	unsigned StopWatch::s_exp_id = 0;
	unsigned StopWatch::s_exp_count = 0;
#endif
}
