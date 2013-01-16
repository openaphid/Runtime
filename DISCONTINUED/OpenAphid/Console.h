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
#ifndef OpenAphid_Console_h
#define OpenAphid_Console_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

namespace Aphid {
	class AJConsole;
	class ScriptCallStack;
	
	enum MessageType {
		MessageType_Log,
		MessageType_Object,
		MessageType_Trace,
		MessageType_Assert
	};
	
	enum MessageLevel {
		MessageLevel_Log,
		MessageLevel_Warn,
		MessageLevel_Error,
		MessageLevel_Debug,
		MessageLevel_Info
	}; 
	
	///-------------------------------------------------------------------------------------------------------------------
	class Console : public RefCounted<Console>, public DirectWrapper<AJConsole> {		
	public:
		virtual ~Console();
		
		static PassRefPtr<Console> create() {return adoptRef(new Console());}
		
		void debug(ScriptCallStack& stack);
		void error(ScriptCallStack& stack);
		void info(ScriptCallStack& stack);
		void log(ScriptCallStack& stack);
		void warn(ScriptCallStack& stack);
		void trace(ScriptCallStack& stack);
		void assertCondition(bool condition, ScriptCallStack& stack);
		
	private:
		Console();
		void addMessage(MessageType type, MessageLevel level, ScriptCallStack& stack);
	}; // class Console
	
}// namespace Aphid

#endif
