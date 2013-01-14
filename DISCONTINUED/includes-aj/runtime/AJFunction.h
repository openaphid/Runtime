
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
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Cameron Zwarich (cwzwarich@uwaterloo.ca)
 *  Copyright (C) 2007 Maks Orlovich
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef AJFunction_h
#define AJFunction_h

#include "InternalFunction.h"

namespace AJ {

    class ExecutableBase;
    class FunctionExecutable;
    class FunctionPrototype;
    class JSActivation;
    class AJGlobalObject;
    class NativeExecutable;

    class AJFunction : public InternalFunction {
        friend class JIT;
        friend class AJGlobalData;

        typedef InternalFunction Base;

    public:
        AJFunction(ExecState*, NonNullPassRefPtr<Structure>, int length, const Identifier&, NativeFunction);
        AJFunction(ExecState*, NonNullPassRefPtr<Structure>, int length, const Identifier&, NativeExecutable*, NativeFunction);
        AJFunction(ExecState*, NonNullPassRefPtr<FunctionExecutable>, ScopeChainNode*);
        virtual ~AJFunction();

        AJObject* construct(ExecState*, const ArgList&);
        AJValue call(ExecState*, AJValue thisValue, const ArgList&);

        void setScope(const ScopeChain& scopeChain) { setScopeChain(scopeChain); }
        ScopeChain& scope() { return scopeChain(); }

        ExecutableBase* executable() const { return m_executable.get(); }

        // To call either of these methods include Executable.h
        inline bool isHostFunction() const;
        FunctionExecutable* jsExecutable() const;

        static JS_EXPORTDATA const ClassInfo info;

        static PassRefPtr<Structure> createStructure(AJValue prototype) 
        { 
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
        }

        NativeFunction nativeFunction()
        {
            return *ATF::bitwise_cast<NativeFunction*>(m_data);
        }

        virtual ConstructType getConstructData(ConstructData&);
        virtual CallType getCallData(CallData&);

    protected:
        const static unsigned StructureFlags = OverridesGetOwnPropertySlot | ImplementsHasInstance | OverridesMarkChildren | OverridesGetPropertyNames | InternalFunction::StructureFlags;

    private:
        AJFunction(NonNullPassRefPtr<Structure>);

        bool isHostFunctionNonInline() const;

        virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
        virtual void getOwnPropertyNames(ExecState*, PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);
        virtual void put(ExecState*, const Identifier& propertyName, AJValue, PutPropertySlot&);
        virtual bool deleteProperty(ExecState*, const Identifier& propertyName);

        virtual void markChildren(MarkStack&);

        virtual const ClassInfo* classInfo() const { return &info; }

        static AJValue argumentsGetter(ExecState*, AJValue, const Identifier&);
        static AJValue callerGetter(ExecState*, AJValue, const Identifier&);
        static AJValue lengthGetter(ExecState*, AJValue, const Identifier&);

        RefPtr<ExecutableBase> m_executable;
        ScopeChain& scopeChain()
        {
            ASSERT(!isHostFunctionNonInline());
            return *ATF::bitwise_cast<ScopeChain*>(m_data);
        }
        void clearScopeChain()
        {
            ASSERT(!isHostFunctionNonInline());
            new (m_data) ScopeChain(NoScopeChain());
        }
        void setScopeChain(ScopeChainNode* sc)
        {
            ASSERT(!isHostFunctionNonInline());
            new (m_data) ScopeChain(sc);
        }
        void setScopeChain(const ScopeChain& sc)
        {
            ASSERT(!isHostFunctionNonInline());
            *ATF::bitwise_cast<ScopeChain*>(m_data) = sc;
        }
        void setNativeFunction(NativeFunction func)
        {
            *ATF::bitwise_cast<NativeFunction*>(m_data) = func;
        }
        unsigned char m_data[sizeof(void*)];
    };

    AJFunction* asFunction(AJValue);

    inline AJFunction* asFunction(AJValue value)
    {
        ASSERT(asObject(value)->inherits(&AJFunction::info));
        return static_cast<AJFunction*>(asObject(value));
    }

} // namespace AJ

#endif // AJFunction_h
