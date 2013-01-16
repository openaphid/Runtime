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

#ifndef OpenAphid_TimerTarget_h
#define OpenAphid_TimerTarget_h

#include "OAData.h"

namespace Aphid {
	class Node;
	
	///-------------------------------------------------------------------------------------------------------------------
	class TimerTarget {
	public:
		virtual ~TimerTarget() {}
		
		void ref() { refTimerTarget(); }
		void deref() { derefTimerTarget(); }
		virtual void update(TimeSec dt)
		{
			UNUSED_PARAM(dt);
		}
		virtual Node* toNode() {return 0;}
		
	protected:
		virtual void refTimerTarget() = 0;
		virtual void derefTimerTarget() = 0;
	};

	///-------------------------------------------------------------------------------------------------------------------
	typedef void (*TimerFunction)(TimerTarget* thisObject, TimeSec dt);
	#define OA_REPEAT_FOREVER -1
}

#endif
