
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

#ifndef PropertyDescriptor_h
#define PropertyDescriptor_h

#include "AJValue.h"

namespace AJ {
    class PropertyDescriptor {
    public:
        PropertyDescriptor()
            : m_attributes(defaultAttributes)
            , m_seenAttributes(0)
        {
        }
        bool writable() const;
        bool enumerable() const;
        bool configurable() const;
        bool isDataDescriptor() const;
        bool isGenericDescriptor() const;
        bool isAccessorDescriptor() const;
        unsigned attributes() const { return m_attributes; }
        AJValue value() const { return m_value; }
        AJValue getter() const;
        AJValue setter() const;
        void setUndefined();
        void setDescriptor(AJValue value, unsigned attributes);
        void setAccessorDescriptor(AJValue getter, AJValue setter, unsigned attributes);
        void setWritable(bool);
        void setEnumerable(bool);
        void setConfigurable(bool);
        void setValue(AJValue value) { m_value = value; }
        void setSetter(AJValue);
        void setGetter(AJValue);
        bool isEmpty() const { return !(m_value || m_getter || m_setter || m_seenAttributes); }
        bool writablePresent() const { return m_seenAttributes & WritablePresent; }
        bool enumerablePresent() const { return m_seenAttributes & EnumerablePresent; }
        bool configurablePresent() const { return m_seenAttributes & ConfigurablePresent; }
        bool setterPresent() const { return m_setter; }
        bool getterPresent() const { return m_getter; }
        bool equalTo(ExecState* exec, const PropertyDescriptor& other) const;
        bool attributesEqual(const PropertyDescriptor& other) const;
        unsigned attributesWithOverride(const PropertyDescriptor& other) const;
    private:
        static unsigned defaultAttributes;
        bool operator==(const PropertyDescriptor&){ return false; }
        enum { WritablePresent = 1, EnumerablePresent = 2, ConfigurablePresent = 4};
        // May be a getter/setter
        AJValue m_value;
        AJValue m_getter;
        AJValue m_setter;
        unsigned m_attributes;
        unsigned m_seenAttributes;
    };
}

#endif
