
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
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Interpreter_h
#define Interpreter_h

#include "ArgList.h"
#include "FastAllocBase.h"
#include "AJCell.h"
#include "AJValue.h"
#include "AJObject.h"
#include "Opcode.h"
#include "RegisterFile.h"

#include <wtf/HashMap.h>

namespace AJ {

    class CodeBlock;
    class EvalExecutable;
    class FunctionExecutable;
    class InternalFunction;
    class AJFunction;
    class AJGlobalObject;
    class ProgramExecutable;
    class Register;
    class ScopeChainNode;
    class SamplingTool;
    struct CallFrameClosure;
    struct HandlerInfo;
    struct Instruction;
    
    enum DebugHookID {
        WillExecuteProgram,
        DidExecuteProgram,
        DidEnterCallFrame,
        DidReachBreakpoint,
        WillLeaveCallFrame,
        WillExecuteStatement
    };

    // We use a smaller reentrancy limit on iPhone because of the high amount of
    // stack space required on the web thread.
    enum { MaxLargeThreadReentryDepth = 100, MaxSmallThreadReentryDepth = 32 };

    class Interpreter : public FastAllocBase {
        friend class JIT;
        friend class CachedCall;
    public:
        Interpreter();

        RegisterFile& registerFile() { return m_registerFile; }
        
        Opcode getOpcode(OpcodeID id)
        {
            #if ENABLE(COMPUTED_GOTO_INTERPRETER)
                return m_opcodeTable[id];
            #else
                return id;
            #endif
        }

        OpcodeID getOpcodeID(Opcode opcode)
        {
            #if ENABLE(COMPUTED_GOTO_INTERPRETER)
                ASSERT(isOpcode(opcode));
                return m_opcodeIDTable.get(opcode);
            #else
                return opcode;
            #endif
        }

        bool isOpcode(Opcode);
        
        AJValue execute(ProgramExecutable*, CallFrame*, ScopeChainNode*, AJObject* thisObj, AJValue* exception);
        AJValue execute(FunctionExecutable*, CallFrame*, AJFunction*, AJObject* thisObj, const ArgList& args, ScopeChainNode*, AJValue* exception);
        AJValue execute(EvalExecutable* evalNode, CallFrame* exec, AJObject* thisObj, ScopeChainNode* scopeChain, AJValue* exception);

        AJValue retrieveArguments(CallFrame*, AJFunction*) const;
        AJValue retrieveCaller(CallFrame*, InternalFunction*) const;
        void retrieveLastCaller(CallFrame*, int& lineNumber, intptr_t& sourceID, UString& sourceURL, AJValue& function) const;
        
        void getArgumentsData(CallFrame*, AJFunction*&, ptrdiff_t& firstParameterIndex, Register*& argv, int& argc);
        
        SamplingTool* sampler() { return m_sampler.get(); }

        NEVER_INLINE AJValue callEval(CallFrame*, RegisterFile*, Register* argv, int argc, int registerOffset, AJValue& exceptionValue);
        NEVER_INLINE HandlerInfo* throwException(CallFrame*&, AJValue&, unsigned bytecodeOffset, bool);
        NEVER_INLINE void debug(CallFrame*, DebugHookID, int firstLine, int lastLine);

        void dumpSampleData(ExecState* exec);
        void startSampling();
        void stopSampling();
    private:
        enum ExecutionFlag { Normal, InitializeAndReturn };

        CallFrameClosure prepareForRepeatCall(FunctionExecutable*, CallFrame*, AJFunction*, int argCount, ScopeChainNode*, AJValue* exception);
        void endRepeatCall(CallFrameClosure&);
        AJValue execute(CallFrameClosure&, AJValue* exception);

        AJValue execute(EvalExecutable*, CallFrame*, AJObject* thisObject, int globalRegisterOffset, ScopeChainNode*, AJValue* exception);

#if ENABLE(INTERPRETER)
        NEVER_INLINE bool resolve(CallFrame*, Instruction*, AJValue& exceptionValue);
        NEVER_INLINE bool resolveSkip(CallFrame*, Instruction*, AJValue& exceptionValue);
        NEVER_INLINE bool resolveGlobal(CallFrame*, Instruction*, AJValue& exceptionValue);
        NEVER_INLINE bool resolveGlobalDynamic(CallFrame*, Instruction*, AJValue& exceptionValue);
        NEVER_INLINE void resolveBase(CallFrame*, Instruction* vPC);
        NEVER_INLINE bool resolveBaseAndProperty(CallFrame*, Instruction*, AJValue& exceptionValue);
        NEVER_INLINE ScopeChainNode* createExceptionScope(CallFrame*, const Instruction* vPC);

        void tryCacheGetByID(CallFrame*, CodeBlock*, Instruction*, AJValue baseValue, const Identifier& propertyName, const PropertySlot&);
        void uncacheGetByID(CodeBlock*, Instruction* vPC);
        void tryCachePutByID(CallFrame*, CodeBlock*, Instruction*, AJValue baseValue, const PutPropertySlot&);
        void uncachePutByID(CodeBlock*, Instruction* vPC);        
#endif // ENABLE(INTERPRETER)

        NEVER_INLINE bool unwindCallFrame(CallFrame*&, AJValue, unsigned& bytecodeOffset, CodeBlock*&);

        static ALWAYS_INLINE CallFrame* slideRegisterWindowForCall(CodeBlock*, RegisterFile*, CallFrame*, size_t registerOffset, int argc);

        static CallFrame* findFunctionCallFrame(CallFrame*, InternalFunction*);

        AJValue privateExecute(ExecutionFlag, RegisterFile*, CallFrame*, AJValue* exception);

        void dumpCallFrame(CallFrame*);
        void dumpRegisters(CallFrame*);
        
        bool isCallBytecode(Opcode opcode) { return opcode == getOpcode(op_call) || opcode == getOpcode(op_construct) || opcode == getOpcode(op_call_eval); }

        void enableSampler();
        int m_sampleEntryDepth;
        OwnPtr<SamplingTool> m_sampler;

        int m_reentryDepth;

        RegisterFile m_registerFile;
        
#if ENABLE(COMPUTED_GOTO_INTERPRETER)
        Opcode m_opcodeTable[numOpcodeIDs]; // Maps OpcodeID => Opcode for compiling
        HashMap<Opcode, OpcodeID> m_opcodeIDTable; // Maps Opcode => OpcodeID for decompiling
#endif
    };
    
} // namespace AJ

#endif // Interpreter_h
