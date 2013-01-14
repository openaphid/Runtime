
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

#ifndef JSZombie_h
#define JSZombie_h

#include "AJCell.h"

#if ENABLE(JSC_ZOMBIES)
namespace AJ {

class JSZombie : public AJCell {
public:
    JSZombie(const ClassInfo* oldInfo, Structure* structure)
        : AJCell(structure)
        , m_oldInfo(oldInfo)
    {
    }
    virtual bool isZombie() const { return true; }
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static Structure* leakedZombieStructure();

    virtual bool isGetterSetter() const { ASSERT_NOT_REACHED(); return false; }
    virtual bool isAPIValueWrapper() const { ASSERT_NOT_REACHED(); return false; }
    virtual bool isPropertyNameIterator() const { ASSERT_NOT_REACHED(); return false; }
    virtual CallType getCallData(CallData&) { ASSERT_NOT_REACHED(); return CallTypeNone; }
    virtual ConstructType getConstructData(ConstructData&) { ASSERT_NOT_REACHED(); return ConstructTypeNone; }
    virtual bool getUInt32(uint32_t&) const { ASSERT_NOT_REACHED(); return false; }
    virtual AJValue toPrimitive(ExecState*, PreferredPrimitiveType) const { ASSERT_NOT_REACHED(); return jsNull(); }
    virtual bool getPrimitiveNumber(ExecState*, double&, AJValue&) { ASSERT_NOT_REACHED(); return false; }
    virtual bool toBoolean(ExecState*) const { ASSERT_NOT_REACHED(); return false; }
    virtual double toNumber(ExecState*) const { ASSERT_NOT_REACHED(); return 0.0; }
    virtual UString toString(ExecState*) const { ASSERT_NOT_REACHED(); return ""; }
    virtual AJObject* toObject(ExecState*) const { ASSERT_NOT_REACHED(); return 0; }
    virtual void markChildren(MarkStack&) { ASSERT_NOT_REACHED(); }
    virtual void put(ExecState*, const Identifier&, AJValue, PutPropertySlot&) { ASSERT_NOT_REACHED(); }
    virtual void put(ExecState*, unsigned, AJValue) { ASSERT_NOT_REACHED(); }
    virtual bool deleteProperty(ExecState*, const Identifier&) { ASSERT_NOT_REACHED(); return false; }
    virtual bool deleteProperty(ExecState*, unsigned) { ASSERT_NOT_REACHED(); return false; }
    virtual AJObject* toThisObject(ExecState*) const { ASSERT_NOT_REACHED(); return 0; }
    virtual AJValue getJSNumber() { ASSERT_NOT_REACHED(); return jsNull(); }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&) { ASSERT_NOT_REACHED(); return false; }
    virtual bool getOwnPropertySlot(ExecState*, unsigned, PropertySlot&) { ASSERT_NOT_REACHED(); return false; }
    
    static const ClassInfo s_info;
private:
    const ClassInfo* m_oldInfo;
};

}

#endif // ENABLE(JSC_ZOMBIES)

#endif // JSZombie_h
