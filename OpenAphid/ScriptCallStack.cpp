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

#include "ScriptCallStack.h"

#include "interpreter/CallFrame.h"
#include <interpreter/Interpreter.h>
#include "runtime/InternalFunction.h"
#include "runtime/AJGlobalObject.h"

namespace Aphid {
	using namespace AJ;
	
	///-------------------------------------------------------------------------------------------------------------------
	ScriptCallFrame::ScriptCallFrame(const UString& functionName, const UString& urlString, int lineNO, const ArgList& args, unsigned skipCount)
	: m_functionName(functionName)
	, m_sourceURLString(urlString)
	, m_lineNO(lineNO)
	{
		size_t count = args.size();
		for (size_t i = skipCount; i < count; ++i)
			m_arguments.append(args.at(i));
	}
	
	ScriptCallFrame::~ScriptCallFrame()
	{
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	ScriptCallStack::ScriptCallStack(ExecState* exec, const ArgList& args, unsigned skipCount)
	: m_filled(false)
	, m_exec(exec)
	, m_caller(0)
	{
		int tmpLineNO;
		intptr_t sourceID;
		UString urlString;
		AJValue function;
		
		exec->interpreter()->retrieveLastCaller(exec, tmpLineNO, sourceID, urlString, function);
		
		unsigned lineNO = tmpLineNO >= 0 ? tmpLineNO : 0;
		
		if (function) {
			m_caller = asInternalFunction(function);
			m_frames.append(ScriptCallFrame(m_caller->name(exec), urlString, lineNO, args, skipCount));
		} else
			m_frames.append(ScriptCallFrame(UString(), urlString, lineNO, args, skipCount));
	}
	
	ScriptCallStack::~ScriptCallStack()
	{
	}
	
	ExecState* ScriptCallStack::globalExecState() const
	{
		return m_exec->lexicalGlobalObject()->globalExec();
	}
	
	void ScriptCallStack::fillFrames()
	{
		if (m_filled || !m_caller)
			return;
		
		/*
		AJValue function = m_exec->interpreter()->retrieveCaller(m_exec, m_caller);
		while (!function.isNull()) {
			InternalFunction* internal = asInternalFunction(function);
			m_frames.append(ScriptCallFrame(internal->name(m_exec), UString(), 0, ArgList(), 0));
			function = m_exec->interpreter()->retrieveCaller(m_exec, internal);
		}*/
		CallFrame* callFrame = m_exec->callerFrame();
		
		while(callFrame) {
			UString functionName;
			int tmpLineNO;
			intptr_t sourceID;
			UString urlString;
			AJValue function;
			
			m_exec->interpreter()->retrieveLastCaller(callFrame, tmpLineNO, sourceID, urlString, function);
			
			if (function) {
				functionName = asInternalFunction(function)->name(m_exec);
			} else {
				if (!m_frames.isEmpty())
					break;
			}
			
			unsigned lineNO = tmpLineNO >= 0 ? tmpLineNO : 0;
			m_frames.append(ScriptCallFrame(functionName, urlString, lineNO, ArgList(), 0));
			
			if (!function)
				break;
			callFrame = callFrame->callerFrame();
		}
		
		m_filled = true;
	}
	
	unsigned ScriptCallStack::size()
	{
		fillFrames();
		return m_frames.size();
	}
	
	const ScriptCallFrame& ScriptCallStack::frame(unsigned index)
	{
		if (index > 0)
			fillFrames();
		ASSERT(m_frames.size() > index);
		return m_frames[index];
	}
	
} //namespace Aphid
