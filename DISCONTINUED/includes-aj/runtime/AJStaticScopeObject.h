
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
 * Copyright (C) 2008, 2009 Apple Inc. All Rights Reserved.
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

#ifndef AJStaticScopeObject_h
#define AJStaticScopeObject_h

#include "JSVariableObject.h"

namespace AJ{
    
    class AJStaticScopeObject : public JSVariableObject {
    protected:
        using JSVariableObject::JSVariableObjectData;
        struct AJStaticScopeObjectData : public JSVariableObjectData {
            AJStaticScopeObjectData()
                : JSVariableObjectData(&symbolTable, &registerStore + 1)
            {
            }
            SymbolTable symbolTable;
            Register registerStore;
        };
        
    public:
        AJStaticScopeObject(ExecState* exec, const Identifier& ident, AJValue value, unsigned attributes)
            : JSVariableObject(exec->globalData().staticScopeStructure, new AJStaticScopeObjectData())
        {
            d()->registerStore = value;
            symbolTable().add(ident.ustring().rep(), SymbolTableEntry(-1, attributes));
        }
        virtual ~AJStaticScopeObject();
        virtual void markChildren(MarkStack&);
        bool isDynamicScope(bool& requiresDynamicChecks) const;
        virtual AJObject* toThisObject(ExecState*) const;
        virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
        virtual void put(ExecState*, const Identifier&, AJValue, PutPropertySlot&);
        void putWithAttributes(ExecState*, const Identifier&, AJValue, unsigned attributes);

        static PassRefPtr<Structure> createStructure(AJValue proto) { return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); }

    protected:
        static const unsigned StructureFlags = OverridesGetOwnPropertySlot | NeedsThisConversion | OverridesMarkChildren | OverridesGetPropertyNames | JSVariableObject::StructureFlags;

    private:
        AJStaticScopeObjectData* d() { return static_cast<AJStaticScopeObjectData*>(JSVariableObject::d); }
    };

}

#endif // AJStaticScopeObject_h
