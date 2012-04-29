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
#include "Console.h"
#include "AJConsole.h"

#include "OAUtil.h"

#include "ScriptCallStack.h"
#include "Diagnostic.h"
#include "AJOABinding.h"

#include <time.h>

namespace Aphid {
	using namespace AJ;
	
	///-------------------------------------------------------------------------------------------------------------------
	Console::Console()
	{
		LEAK_DETECT_INC("Console");
	}
	
	Console::~Console()
	{
		//oa_debug("Console destructed");
		LEAK_DETECT_DEC("Console");
	}
	
	void Console::debug(ScriptCallStack& stack)
	{
		addMessage(MessageType_Log, MessageLevel_Debug, stack);
	}
	
	void Console::error(ScriptCallStack& stack)
	{
		addMessage(MessageType_Log, MessageLevel_Error, stack);
	}
	
	void Console::info(ScriptCallStack& stack)
	{
		addMessage(MessageType_Log, MessageLevel_Info, stack);
	}
	
	void Console::log(ScriptCallStack& stack)
	{
		addMessage(MessageType_Log, MessageLevel_Log, stack);
	}
	
	void Console::warn(ScriptCallStack& stack)
	{
		addMessage(MessageType_Log, MessageLevel_Warn, stack);
	}
	
	void Console::trace(ScriptCallStack& stack)
	{
		//addMessage(MessageType_Trace, MessageLevel_Debug, stack);
		printf("Stack Trace:\n");
    for (unsigned i = 0; i < stack.size(); ++i) {
			const ScriptCallFrame& frame = stack.frame(i);
			printf("\t%s, [%s: %d]\n", frame.functionName().UTF8String().data(), frame.sourceURLString().UTF8String().data(), frame.lineNO());
    }
		fflush(stdout);
	}
	
	void Console::assertCondition(bool condition, ScriptCallStack& stack)
	{
		if (!condition)
			addMessage(MessageType_Assert, MessageLevel_Error, stack);
	}
	
	void Console::addMessage(MessageType type, MessageLevel level, ScriptCallStack& stack)
	{
		UNUSED_PARAM(type);
		
		const ScriptCallFrame& lastFrame = stack.frame(0);
		time_t now = time(NULL);
		char nowstr[64];
		
		strftime(nowstr, 64, "%D,%T", gmtime(&now));
		
		const char* levelString;
		switch(level) {
			case MessageLevel_Debug:
				levelString = "DEBUG";
				break;
			case MessageLevel_Log:
				levelString = "LOG";
				break;
			case MessageLevel_Info:
				levelString = "INFO";
				break;
			case MessageLevel_Warn:
				levelString = "WARN";
				break;
			case MessageLevel_Error:
				levelString = "ERROR";
				break;
			default:
				levelString = "UNKNOWN";
				break;
		}

		String output = String::format("[%s:%d] %s>> ", lastFrame.sourceURLString().UTF8String().data(), lastFrame.lineNO(), levelString);
		
		unsigned count = lastFrame.argsCount();
		for (unsigned int i = 0; i < count; ++i) {
			output.append(String::format("%s ", lastFrame.argAt(i).get().toString(stack.execState()).UTF8String().data()));
		}
		printf("%s %s", nowstr, output.utf8().data());
		printf("\n");
		fflush(stdout);
		
		if (level >= MessageLevel_Warn) {
			switch(level) {
				case MessageLevel_Warn:
					Diagnostic::warn(toUString(output), false);
					break;
				case MessageLevel_Error:
					Diagnostic::error(toUString(output), false);
					break;
				default:
					break;
			}
		}
		//TODO: enhance later, add remote logging in v2
	}
} //namespace Aphid
