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

#ifndef OpenAphid_ScriptCallStack_h
#define OpenAphid_ScriptCallStack_h

#include <runtime/UString.h>
#include <runtime/StringBuilder.h>
#include <runtime/ArgList.h>
#include <runtime/AJValue.h>
#include <runtime/Protect.h>
#include <wtf/PassRefPtr.h>
#include <wtf/Vector.h>
#include <wtf/Noncopyable.h>

namespace AJ {
	class ExecState;
	class AJValue;
	class InternalFunction;
}

namespace Aphid {	
	class ScriptCallFrame {
	public:
		ScriptCallFrame(const AJ::UString& functionName, const AJ::UString& urlString, int lineNO, const AJ::ArgList& args, unsigned skipCount);
		~ScriptCallFrame();
		
		const AJ::UString& sourceURLString() const {return m_sourceURLString;}
		unsigned lineNO() const {return m_lineNO;}
		
		const AJ::UString& functionName() const {return m_functionName;}
		
		const AJ::ProtectedAJValue& argAt(unsigned index) const 
		{
			return m_arguments[index];
		}
		
		unsigned argsCount() const
		{
			return m_arguments.size();
		}
		
	private:
		AJ::UString m_functionName;
		AJ::UString m_sourceURLString;
		unsigned m_lineNO;
		Vector<AJ::ProtectedAJValue> m_arguments;
	};
	
	class ScriptCallStack : public Noncopyable {
	public:
		ScriptCallStack(AJ::ExecState* exec, const AJ::ArgList& args, unsigned skipCount=0);
		~ScriptCallStack();
		
		AJ::ExecState* execState() const {return m_exec;}
		AJ::ExecState* globalExecState() const;
		
		unsigned size();
		const ScriptCallFrame& frame(unsigned index);
		
	private:
		void fillFrames();
		bool m_filled;
		
		AJ::ExecState* m_exec;
		Vector<ScriptCallFrame> m_frames;
		AJ::InternalFunction* m_caller;
	};
}

#endif
