
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
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef AJClassRef_h
#define AJClassRef_h

#include "AJObjectRef.h"

#include <runtime/AJObject.h>
#include <runtime/Protect.h>
#include <runtime/UString.h>
#include <runtime/WeakGCPtr.h>
#include <wtf/HashMap.h>
#include <wtf/RefCounted.h>

struct StaticValueEntry : FastAllocBase {
    StaticValueEntry(AJObjectGetPropertyCallback _getProperty, AJObjectSetPropertyCallback _setProperty, AJPropertyAttributes _attributes)
        : getProperty(_getProperty), setProperty(_setProperty), attributes(_attributes)
    {
    }
    
    AJObjectGetPropertyCallback getProperty;
    AJObjectSetPropertyCallback setProperty;
    AJPropertyAttributes attributes;
};

struct StaticFunctionEntry : FastAllocBase {
    StaticFunctionEntry(AJObjectCallAsFunctionCallback _callAsFunction, AJPropertyAttributes _attributes)
        : callAsFunction(_callAsFunction), attributes(_attributes)
    {
    }

    AJObjectCallAsFunctionCallback callAsFunction;
    AJPropertyAttributes attributes;
};

typedef HashMap<RefPtr<AJ::UString::Rep>, StaticValueEntry*> OpaqueAJClassStaticValuesTable;
typedef HashMap<RefPtr<AJ::UString::Rep>, StaticFunctionEntry*> OpaqueAJClassStaticFunctionsTable;

struct OpaqueAJClass;

// An OpaqueAJClass (AJClass) is created without a context, so it can be used with any context, even across context groups.
// This structure holds data members that vary across context groups.
struct OpaqueAJClassContextData : Noncopyable {
    OpaqueAJClassContextData(OpaqueAJClass*);
    ~OpaqueAJClassContextData();

    // It is necessary to keep OpaqueAJClass alive because of the following rare scenario:
    // 1. A class is created and used, so its context data is stored in AJGlobalData hash map.
    // 2. The class is released, and when all JS objects that use it are collected, OpaqueAJClass
    // is deleted (that's the part prevented by this RefPtr).
    // 3. Another class is created at the same address.
    // 4. When it is used, the old context data is found in AJGlobalData and used.
    RefPtr<OpaqueAJClass> m_class;

    OpaqueAJClassStaticValuesTable* staticValues;
    OpaqueAJClassStaticFunctionsTable* staticFunctions;
    AJ::WeakGCPtr<AJ::AJObject> cachedPrototype;
};

struct OpaqueAJClass : public ThreadSafeShared<OpaqueAJClass> {
    static PassRefPtr<OpaqueAJClass> create(const AJClassDefinition*);
    static PassRefPtr<OpaqueAJClass> createNoAutomaticPrototype(const AJClassDefinition*);
    ~OpaqueAJClass();
    
    AJ::UString className();
    OpaqueAJClassStaticValuesTable* staticValues(AJ::ExecState*);
    OpaqueAJClassStaticFunctionsTable* staticFunctions(AJ::ExecState*);
    AJ::AJObject* prototype(AJ::ExecState*);

    OpaqueAJClass* parentClass;
    OpaqueAJClass* prototypeClass;
    
    AJObjectInitializeCallback initialize;
    AJObjectFinalizeCallback finalize;
    AJObjectHasPropertyCallback hasProperty;
    AJObjectGetPropertyCallback getProperty;
    AJObjectSetPropertyCallback setProperty;
    AJObjectDeletePropertyCallback deleteProperty;
    AJObjectGetPropertyNamesCallback getPropertyNames;
    AJObjectCallAsFunctionCallback callAsFunction;
    AJObjectCallAsConstructorCallback callAsConstructor;
    AJObjectHasInstanceCallback hasInstance;
    AJObjectConvertToTypeCallback convertToType;

private:
    friend struct OpaqueAJClassContextData;

    OpaqueAJClass();
    OpaqueAJClass(const OpaqueAJClass&);
    OpaqueAJClass(const AJClassDefinition*, OpaqueAJClass* protoClass);

    OpaqueAJClassContextData& contextData(AJ::ExecState*);

    // UStrings in these data members should not be put into any IdentifierTable.
    AJ::UString m_className;
    OpaqueAJClassStaticValuesTable* m_staticValues;
    OpaqueAJClassStaticFunctionsTable* m_staticFunctions;
};

#endif // AJClassRef_h
