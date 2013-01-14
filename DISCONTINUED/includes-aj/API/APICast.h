
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

#ifndef APICast_h
#define APICast_h

#include "AJAPIValueWrapper.h"
#include "AJGlobalObject.h"
#include "AJValue.h"
#include <wtf/UnusedParam.h>

namespace AJ {
    class ExecState;
    class PropertyNameArray;
    class AJGlobalData;
    class AJObject;
    class AJValue;
}

typedef const struct OpaqueAJContextGroup* AJContextGroupRef;
typedef const struct OpaqueAJContext* AJContextRef;
typedef struct OpaqueAJContext* AJGlobalContextRef;
typedef struct OpaqueAJPropertyNameAccumulator* AJPropertyNameAccumulatorRef;
typedef const struct OpaqueAJValue* AJValueRef;
typedef struct OpaqueAJValue* AJObjectRef;

/* Opaque typing convenience methods */

inline AJ::ExecState* toJS(AJContextRef c)
{
    ASSERT(c);
    return reinterpret_cast<AJ::ExecState*>(const_cast<OpaqueAJContext*>(c));
}

inline AJ::ExecState* toJS(AJGlobalContextRef c)
{
    ASSERT(c);
    return reinterpret_cast<AJ::ExecState*>(c);
}

inline AJ::AJValue toJS(AJ::ExecState* exec, AJValueRef v)
{
    ASSERT_UNUSED(exec, exec);
    ASSERT(v);
#if USE(JSVALUE32_64)
    AJ::AJCell* jsCell = reinterpret_cast<AJ::AJCell*>(const_cast<OpaqueAJValue*>(v));
    if (!jsCell)
        return AJ::AJValue();
    if (jsCell->isAPIValueWrapper())
        return static_cast<AJ::AJAPIValueWrapper*>(jsCell)->value();
    return jsCell;
#else
    return AJ::AJValue::decode(reinterpret_cast<AJ::EncodedAJValue>(const_cast<OpaqueAJValue*>(v)));
#endif
}

inline AJ::AJValue toJSForGC(AJ::ExecState* exec, AJValueRef v)
{
    ASSERT_UNUSED(exec, exec);
    ASSERT(v);
#if USE(JSVALUE32_64)
    AJ::AJCell* jsCell = reinterpret_cast<AJ::AJCell*>(const_cast<OpaqueAJValue*>(v));
    if (!jsCell)
        return AJ::AJValue();
    return jsCell;
#else
    return AJ::AJValue::decode(reinterpret_cast<AJ::EncodedAJValue>(const_cast<OpaqueAJValue*>(v)));
#endif
}

inline AJ::AJObject* toJS(AJObjectRef o)
{
    return reinterpret_cast<AJ::AJObject*>(o);
}

inline AJ::PropertyNameArray* toJS(AJPropertyNameAccumulatorRef a)
{
    return reinterpret_cast<AJ::PropertyNameArray*>(a);
}

inline AJ::AJGlobalData* toJS(AJContextGroupRef g)
{
    return reinterpret_cast<AJ::AJGlobalData*>(const_cast<OpaqueAJContextGroup*>(g));
}

inline AJValueRef toRef(AJ::ExecState* exec, AJ::AJValue v)
{
#if USE(JSVALUE32_64)
    if (!v)
        return 0;
    if (!v.isCell())
        return reinterpret_cast<AJValueRef>(asCell(AJ::jsAPIValueWrapper(exec, v)));
    return reinterpret_cast<AJValueRef>(asCell(v));
#else
    UNUSED_PARAM(exec);
    return reinterpret_cast<AJValueRef>(AJ::AJValue::encode(v));
#endif
}

inline AJObjectRef toRef(AJ::AJObject* o)
{
    return reinterpret_cast<AJObjectRef>(o);
}

inline AJObjectRef toRef(const AJ::AJObject* o)
{
    return reinterpret_cast<AJObjectRef>(const_cast<AJ::AJObject*>(o));
}

inline AJContextRef toRef(AJ::ExecState* e)
{
    return reinterpret_cast<AJContextRef>(e);
}

inline AJGlobalContextRef toGlobalRef(AJ::ExecState* e)
{
    ASSERT(e == e->lexicalGlobalObject()->globalExec());
    return reinterpret_cast<AJGlobalContextRef>(e);
}

inline AJPropertyNameAccumulatorRef toRef(AJ::PropertyNameArray* l)
{
    return reinterpret_cast<AJPropertyNameAccumulatorRef>(l);
}

inline AJContextGroupRef toRef(AJ::AJGlobalData* g)
{
    return reinterpret_cast<AJContextGroupRef>(g);
}

#endif // APICast_h
