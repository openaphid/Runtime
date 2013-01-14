
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
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef DebuggerActivation_h
#define DebuggerActivation_h

#include "AJObject.h"

namespace AJ {

    class JSActivation;

    class DebuggerActivation : public AJObject {
    public:
        DebuggerActivation(AJObject*);

        virtual void markChildren(MarkStack&);
        virtual UString className() const;
        virtual bool getOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        virtual void put(ExecState*, const Identifier& propertyName, AJValue, PutPropertySlot&);
        virtual void putWithAttributes(ExecState*, const Identifier& propertyName, AJValue, unsigned attributes);
        virtual bool deleteProperty(ExecState*, const Identifier& propertyName);
        virtual void getOwnPropertyNames(ExecState*, PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
        virtual void defineGetter(ExecState*, const Identifier& propertyName, AJObject* getterFunction, unsigned attributes);
        virtual void defineSetter(ExecState*, const Identifier& propertyName, AJObject* setterFunction, unsigned attributes);
        virtual AJValue lookupGetter(ExecState*, const Identifier& propertyName);
        virtual AJValue lookupSetter(ExecState*, const Identifier& propertyName);

        static PassRefPtr<Structure> createStructure(AJValue prototype) 
        {
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
        }

    protected:
        static const unsigned StructureFlags = OverridesGetOwnPropertySlot | OverridesMarkChildren | AJObject::StructureFlags;

    private:
        JSActivation* m_activation;
    };

} // namespace AJ

#endif // DebuggerActivation_h
