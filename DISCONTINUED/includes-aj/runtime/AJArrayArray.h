
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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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

#ifndef AJArrayArray_h
#define AJArrayArray_h

#include "AJObject.h"

#include <wtf/ByteArray.h>

namespace AJ {

    class AJArrayArray : public AJObject {
        friend class AJGlobalData;
    public:
        bool canAccessIndex(unsigned i) { return i < m_storage->length(); }
        AJValue getIndex(ExecState* exec, unsigned i)
        {
            ASSERT(canAccessIndex(i));
            return jsNumber(exec, m_storage->data()[i]);
        }

        void setIndex(unsigned i, int value)
        {
            ASSERT(canAccessIndex(i));
            if (value & ~0xFF) {
                if (value < 0)
                    value = 0;
                else
                    value = 255;
            }
            m_storage->data()[i] = static_cast<unsigned char>(value);
        }
        
        void setIndex(unsigned i, double value)
        {
            ASSERT(canAccessIndex(i));
            if (!(value > 0)) // Clamp NaN to 0
                value = 0;
            else if (value > 255)
                value = 255;
            m_storage->data()[i] = static_cast<unsigned char>(value + 0.5);
        }
        
        void setIndex(ExecState* exec, unsigned i, AJValue value)
        {
            double byteValue = value.toNumber(exec);
            if (exec->hadException())
                return;
            if (canAccessIndex(i))
                setIndex(i, byteValue);
        }

        AJArrayArray(ExecState* exec, NonNullPassRefPtr<Structure>, ATF::ByteArray* storage, const AJ::ClassInfo* = &s_defaultInfo);
        static PassRefPtr<Structure> createStructure(AJValue prototype);
        
        virtual bool getOwnPropertySlot(AJ::ExecState*, const AJ::Identifier& propertyName, AJ::PropertySlot&);
        virtual bool getOwnPropertySlot(AJ::ExecState*, unsigned propertyName, AJ::PropertySlot&);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
        virtual void put(AJ::ExecState*, const AJ::Identifier& propertyName, AJ::AJValue, AJ::PutPropertySlot&);
        virtual void put(AJ::ExecState*, unsigned propertyName, AJ::AJValue);

        virtual void getOwnPropertyNames(AJ::ExecState*, AJ::PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);

        virtual const ClassInfo* classInfo() const { return m_classInfo; }
        static const ClassInfo s_defaultInfo;
        
        size_t length() const { return m_storage->length(); }

        ATF::ByteArray* storage() const { return m_storage.get(); }

#if !ASSERT_DISABLED
        virtual ~AJArrayArray();
#endif

    protected:
        static const unsigned StructureFlags = OverridesGetOwnPropertySlot | OverridesGetPropertyNames | AJObject::StructureFlags;

    private:
        enum VPtrStealingHackType { VPtrStealingHack };
        AJArrayArray(VPtrStealingHackType) 
            : AJObject(createStructure(jsNull()))
            , m_classInfo(0)
        {
        }

        RefPtr<ATF::ByteArray> m_storage;
        const ClassInfo* m_classInfo;
    };
    
    AJArrayArray* asByteArray(AJValue value);
    inline AJArrayArray* asByteArray(AJValue value)
    {
        return static_cast<AJArrayArray*>(asCell(value));
    }

    inline bool isAJArrayArray(AJGlobalData* globalData, AJValue v) { return v.isCell() && v.asCell()->vptr() == globalData->jsByteArrayVPtr; }

} // namespace AJ

#endif // AJArrayArray_h
