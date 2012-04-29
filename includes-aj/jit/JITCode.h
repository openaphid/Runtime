
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
/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef JITCode_h
#define JITCode_h

#if ENABLE(JIT)

#include "CallFrame.h"
#include "AJValue.h"
#include "MacroAssemblerCodeRef.h"
#include "Profiler.h"

namespace AJ {

    class AJGlobalData;
    class RegisterFile;

    class JITCode {
        typedef MacroAssemblerCodeRef CodeRef;
        typedef MacroAssemblerCodePtr CodePtr;
    public:
        JITCode()
        {
        }

        JITCode(const CodeRef ref)
            : m_ref(ref)
        {
        }

        bool operator !() const
        {
            return !m_ref.m_code.executableAddress();
        }

        CodePtr addressForCall()
        {
            return m_ref.m_code;
        }

        // This function returns the offset in bytes of 'pointerIntoCode' into
        // this block of code.  The pointer provided must be a pointer into this
        // block of code.  It is ASSERTed that no codeblock >4gb in size.
        unsigned offsetOf(void* pointerIntoCode)
        {
            intptr_t result = reinterpret_cast<intptr_t>(pointerIntoCode) - reinterpret_cast<intptr_t>(m_ref.m_code.executableAddress());
            ASSERT(static_cast<intptr_t>(static_cast<unsigned>(result)) == result);
            return static_cast<unsigned>(result);
        }

        // Execute the code!
        inline AJValue execute(RegisterFile* registerFile, CallFrame* callFrame, AJGlobalData* globalData, AJValue* exception)
        {
            return AJValue::decode(ctiTrampoline(m_ref.m_code.executableAddress(), registerFile, callFrame, exception, Profiler::enabledProfilerReference(), globalData));
        }

        void* start()
        {
            return m_ref.m_code.dataLocation();
        }

        size_t size()
        {
            ASSERT(m_ref.m_code.executableAddress());
            return m_ref.m_size;
        }

        ExecutablePool* getExecutablePool()
        {
            return m_ref.m_executablePool.get();
        }

        // Host functions are a bit special; they have a m_code pointer but they
        // do not individully ref the executable pool containing the trampoline.
        static JITCode HostFunction(CodePtr code)
        {
            return JITCode(code.dataLocation(), 0, 0);
        }

    private:
        JITCode(void* code, PassRefPtr<ExecutablePool> executablePool, size_t size)
            : m_ref(code, executablePool, size)
        {
        }

        CodeRef m_ref;
    };

};

#endif

#endif
