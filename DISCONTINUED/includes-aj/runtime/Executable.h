
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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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

#ifndef Executable_h
#define Executable_h

#include "AJFunction.h"
#include "Interpreter.h"
#include "Nodes.h"
#include "SamplingTool.h"

namespace AJ {

    class CodeBlock;
    class Debugger;
    class EvalCodeBlock;
    class ProgramCodeBlock;
    class ScopeChainNode;

    struct ExceptionInfo;

    class ExecutableBase : public RefCounted<ExecutableBase> {
        friend class JIT;

    protected:
        static const int NUM_PARAMETERS_IS_HOST = 0;
        static const int NUM_PARAMETERS_NOT_COMPILED = -1;
    
    public:
        ExecutableBase(int numParameters)
            : m_numParameters(numParameters)
        {
        }

        virtual ~ExecutableBase() {}

        bool isHostFunction() const { return m_numParameters == NUM_PARAMETERS_IS_HOST; }

    protected:
        int m_numParameters;

#if ENABLE(JIT)
    public:
        JITCode& generatedJITCode()
        {
            ASSERT(m_jitCode);
            return m_jitCode;
        }

        ExecutablePool* getExecutablePool()
        {
            return m_jitCode.getExecutablePool();
        }

    protected:
        JITCode m_jitCode;
#endif
    };

#if ENABLE(JIT)
    class NativeExecutable : public ExecutableBase {
    public:
        NativeExecutable(JITCode thunk)
            : ExecutableBase(NUM_PARAMETERS_IS_HOST)
        {
            m_jitCode = thunk;
        }

        ~NativeExecutable();
    };
#endif

    class VPtrHackExecutable : public ExecutableBase {
    public:
        VPtrHackExecutable()
            : ExecutableBase(NUM_PARAMETERS_IS_HOST)
        {
        }

        ~VPtrHackExecutable();
    };

    class ScriptExecutable : public ExecutableBase {
    public:
        ScriptExecutable(AJGlobalData* globalData, const SourceCode& source)
            : ExecutableBase(NUM_PARAMETERS_NOT_COMPILED)
            , m_source(source)
            , m_features(0)
        {
#if ENABLE(CODEBLOCK_SAMPLING)
            if (SamplingTool* sampler = globalData->interpreter->sampler())
                sampler->notifyOfScope(this);
#else
            UNUSED_PARAM(globalData);
#endif
        }

        ScriptExecutable(ExecState* exec, const SourceCode& source)
            : ExecutableBase(NUM_PARAMETERS_NOT_COMPILED)
            , m_source(source)
            , m_features(0)
        {
#if ENABLE(CODEBLOCK_SAMPLING)
            if (SamplingTool* sampler = exec->globalData().interpreter->sampler())
                sampler->notifyOfScope(this);
#else
            UNUSED_PARAM(exec);
#endif
        }

        const SourceCode& source() { return m_source; }
        intptr_t sourceID() const { return m_source.provider()->asID(); }
        const UString& sourceURL() const { return m_source.provider()->url(); }
        int lineNo() const { return m_firstLine; }
        int lastLine() const { return m_lastLine; }

        bool usesEval() const { return m_features & EvalFeature; }
        bool usesArguments() const { return m_features & ArgumentsFeature; }
        bool needsActivation() const { return m_features & (EvalFeature | ClosureFeature | WithFeature | CatchFeature); }

        virtual ExceptionInfo* reparseExceptionInfo(AJGlobalData*, ScopeChainNode*, CodeBlock*) = 0;

    protected:
        void recordParse(CodeFeatures features, int firstLine, int lastLine)
        {
            m_features = features;
            m_firstLine = firstLine;
            m_lastLine = lastLine;
        }

        SourceCode m_source;
        CodeFeatures m_features;
        int m_firstLine;
        int m_lastLine;
    };

    class EvalExecutable : public ScriptExecutable {
    public:

        ~EvalExecutable();

        EvalCodeBlock& bytecode(ExecState* exec, ScopeChainNode* scopeChainNode)
        {
            if (!m_evalCodeBlock) {
                AJObject* error = compile(exec, scopeChainNode);
                ASSERT_UNUSED(!error, error);
            }
            return *m_evalCodeBlock;
        }

        AJObject* compile(ExecState*, ScopeChainNode*);

        ExceptionInfo* reparseExceptionInfo(AJGlobalData*, ScopeChainNode*, CodeBlock*);
        static PassRefPtr<EvalExecutable> create(ExecState* exec, const SourceCode& source) { return adoptRef(new EvalExecutable(exec, source)); }

    private:
        EvalExecutable(ExecState* exec, const SourceCode& source)
            : ScriptExecutable(exec, source)
            , m_evalCodeBlock(0)
        {
        }
        EvalCodeBlock* m_evalCodeBlock;

#if ENABLE(JIT)
    public:
        JITCode& jitCode(ExecState* exec, ScopeChainNode* scopeChainNode)
        {
            if (!m_jitCode)
                generateJITCode(exec, scopeChainNode);
            return m_jitCode;
        }

    private:
        void generateJITCode(ExecState*, ScopeChainNode*);
#endif
    };

    class ProgramExecutable : public ScriptExecutable {
    public:
        static PassRefPtr<ProgramExecutable> create(ExecState* exec, const SourceCode& source)
        {
            return adoptRef(new ProgramExecutable(exec, source));
        }

        ~ProgramExecutable();

        ProgramCodeBlock& bytecode(ExecState* exec, ScopeChainNode* scopeChainNode)
        {
            if (!m_programCodeBlock) {
                AJObject* error = compile(exec, scopeChainNode);
                ASSERT_UNUSED(!error, error);
            }
            return *m_programCodeBlock;
        }

        AJObject* checkSyntax(ExecState*);
        AJObject* compile(ExecState*, ScopeChainNode*);

        // CodeBlocks for program code are transient and therefore do not gain from from throwing out there exception information.
        ExceptionInfo* reparseExceptionInfo(AJGlobalData*, ScopeChainNode*, CodeBlock*) { ASSERT_NOT_REACHED(); return 0; }

    private:
        ProgramExecutable(ExecState* exec, const SourceCode& source)
            : ScriptExecutable(exec, source)
            , m_programCodeBlock(0)
        {
        }
        ProgramCodeBlock* m_programCodeBlock;

#if ENABLE(JIT)
    public:
        JITCode& jitCode(ExecState* exec, ScopeChainNode* scopeChainNode)
        {
            if (!m_jitCode)
                generateJITCode(exec, scopeChainNode);
            return m_jitCode;
        }

    private:
        void generateJITCode(ExecState*, ScopeChainNode*);
#endif
    };

    class FunctionExecutable : public ScriptExecutable {
        friend class JIT;
    public:
        static PassRefPtr<FunctionExecutable> create(ExecState* exec, const Identifier& name, const SourceCode& source, bool forceUsesArguments, FunctionParameters* parameters, int firstLine, int lastLine)
        {
            return adoptRef(new FunctionExecutable(exec, name, source, forceUsesArguments, parameters, firstLine, lastLine));
        }

        static PassRefPtr<FunctionExecutable> create(AJGlobalData* globalData, const Identifier& name, const SourceCode& source, bool forceUsesArguments, FunctionParameters* parameters, int firstLine, int lastLine)
        {
            return adoptRef(new FunctionExecutable(globalData, name, source, forceUsesArguments, parameters, firstLine, lastLine));
        }

        ~FunctionExecutable();

        AJFunction* make(ExecState* exec, ScopeChainNode* scopeChain)
        {
            return new (exec) AJFunction(exec, this, scopeChain);
        }

        CodeBlock& bytecode(ExecState* exec, ScopeChainNode* scopeChainNode) 
        {
            ASSERT(scopeChainNode);
            if (!m_codeBlock)
                compile(exec, scopeChainNode);
            return *m_codeBlock;
        }

        bool isGenerated() const
        {
            return m_codeBlock;
        }

        CodeBlock& generatedBytecode()
        {
            ASSERT(m_codeBlock);
            return *m_codeBlock;
        }

        const Identifier& name() { return m_name; }
        size_t parameterCount() const { return m_parameters->size(); }
        unsigned variableCount() const { return m_numVariables; }
        UString paramString() const;

        void recompile();
        ExceptionInfo* reparseExceptionInfo(AJGlobalData*, ScopeChainNode*, CodeBlock*);
        void markAggregate(MarkStack& markStack);
        static PassRefPtr<FunctionExecutable> fromGlobalCode(const Identifier&, ExecState*, Debugger*, const SourceCode&, int* errLine = 0, UString* errMsg = 0);

    private:
        FunctionExecutable(AJGlobalData* globalData, const Identifier& name, const SourceCode& source, bool forceUsesArguments, FunctionParameters* parameters, int firstLine, int lastLine)
            : ScriptExecutable(globalData, source)
            , m_numVariables(0)
            , m_forceUsesArguments(forceUsesArguments)
            , m_parameters(parameters)
            , m_codeBlock(0)
            , m_name(name)
        {
            m_firstLine = firstLine;
            m_lastLine = lastLine;
        }

        FunctionExecutable(ExecState* exec, const Identifier& name, const SourceCode& source, bool forceUsesArguments, FunctionParameters* parameters, int firstLine, int lastLine)
            : ScriptExecutable(exec, source)
            , m_numVariables(0)
            , m_forceUsesArguments(forceUsesArguments)
            , m_parameters(parameters)
            , m_codeBlock(0)
            , m_name(name)
        {
            m_firstLine = firstLine;
            m_lastLine = lastLine;
        }

        void compile(ExecState*, ScopeChainNode*);

        unsigned m_numVariables : 31;
        bool m_forceUsesArguments : 1;

        RefPtr<FunctionParameters> m_parameters;
        CodeBlock* m_codeBlock;
        Identifier m_name;

#if ENABLE(JIT)
    public:
        JITCode& jitCode(ExecState* exec, ScopeChainNode* scopeChainNode)
        {
            if (!m_jitCode)
                generateJITCode(exec, scopeChainNode);
            return m_jitCode;
        }

    private:
        void generateJITCode(ExecState*, ScopeChainNode*);
#endif
    };

    inline FunctionExecutable* AJFunction::jsExecutable() const
    {
        ASSERT(!isHostFunctionNonInline());
        return static_cast<FunctionExecutable*>(m_executable.get());
    }

    inline bool AJFunction::isHostFunction() const
    {
        ASSERT(m_executable);
        return m_executable->isHostFunction();
    }

}

#endif
