
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
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef JSVariableObject_h
#define JSVariableObject_h

#include "AJObject.h"
#include "Register.h"
#include "SymbolTable.h"
#include "UnusedParam.h"
#include <wtf/OwnArrayPtr.h>
#include <wtf/UnusedParam.h>

namespace AJ {

    class Register;

    class JSVariableObject : public AJObject {
        friend class JIT;

    public:
        SymbolTable& symbolTable() const { return *d->symbolTable; }

        virtual void putWithAttributes(ExecState*, const Identifier&, AJValue, unsigned attributes) = 0;

        virtual bool deleteProperty(ExecState*, const Identifier&);
        virtual void getOwnPropertyNames(ExecState*, PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);
        
        virtual bool isVariableObject() const;
        virtual bool isDynamicScope(bool& requiresDynamicChecks) const = 0;

        Register& registerAt(int index) const { return d->registers[index]; }

        static PassRefPtr<Structure> createStructure(AJValue prototype)
        {
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount);
        }
        
    protected:
        static const unsigned StructureFlags = OverridesGetPropertyNames | AJObject::StructureFlags;
        // Subclasses of JSVariableObject can subclass this struct to add data
        // without increasing their own size (since there's a hard limit on the
        // size of a AJCell).
        struct JSVariableObjectData {
            JSVariableObjectData(SymbolTable* symbolTable, Register* registers)
                : symbolTable(symbolTable)
                , registers(registers)
            {
                ASSERT(symbolTable);
            }

            SymbolTable* symbolTable; // Maps name -> offset from "r" in register file.
            Register* registers; // "r" in the register file.
            OwnArrayPtr<Register> registerArray; // Independent copy of registers, used when a variable object copies its registers out of the register file.

        private:
            JSVariableObjectData(const JSVariableObjectData&);
            JSVariableObjectData& operator=(const JSVariableObjectData&);
        };

        JSVariableObject(NonNullPassRefPtr<Structure> structure, JSVariableObjectData* data)
            : AJObject(structure)
            , d(data) // Subclass owns this pointer.
        {
        }

        Register* copyRegisterArray(Register* src, size_t count);
        void setRegisters(Register* r, Register* registerArray);

        bool symbolTableGet(const Identifier&, PropertySlot&);
        bool symbolTableGet(const Identifier&, PropertyDescriptor&);
        bool symbolTableGet(const Identifier&, PropertySlot&, bool& slotIsWriteable);
        bool symbolTablePut(const Identifier&, AJValue);
        bool symbolTablePutWithAttributes(const Identifier&, AJValue, unsigned attributes);

        JSVariableObjectData* d;
    };

    inline bool JSVariableObject::symbolTableGet(const Identifier& propertyName, PropertySlot& slot)
    {
        SymbolTableEntry entry = symbolTable().inlineGet(propertyName.ustring().rep());
        if (!entry.isNull()) {
            slot.setRegisterSlot(&registerAt(entry.getIndex()));
            return true;
        }
        return false;
    }

    inline bool JSVariableObject::symbolTableGet(const Identifier& propertyName, PropertySlot& slot, bool& slotIsWriteable)
    {
        SymbolTableEntry entry = symbolTable().inlineGet(propertyName.ustring().rep());
        if (!entry.isNull()) {
            slot.setRegisterSlot(&registerAt(entry.getIndex()));
            slotIsWriteable = !entry.isReadOnly();
            return true;
        }
        return false;
    }

    inline bool JSVariableObject::symbolTablePut(const Identifier& propertyName, AJValue value)
    {
        ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

        SymbolTableEntry entry = symbolTable().inlineGet(propertyName.ustring().rep());
        if (entry.isNull())
            return false;
        if (entry.isReadOnly())
            return true;
        registerAt(entry.getIndex()) = value;
        return true;
    }

    inline bool JSVariableObject::symbolTablePutWithAttributes(const Identifier& propertyName, AJValue value, unsigned attributes)
    {
        ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

        SymbolTable::iterator iter = symbolTable().find(propertyName.ustring().rep());
        if (iter == symbolTable().end())
            return false;
        SymbolTableEntry& entry = iter->second;
        ASSERT(!entry.isNull());
        entry.setAttributes(attributes);
        registerAt(entry.getIndex()) = value;
        return true;
    }

    inline Register* JSVariableObject::copyRegisterArray(Register* src, size_t count)
    {
        Register* registerArray = new Register[count];
        memcpy(registerArray, src, count * sizeof(Register));

        return registerArray;
    }

    inline void JSVariableObject::setRegisters(Register* registers, Register* registerArray)
    {
        ASSERT(registerArray != d->registerArray.get());
        d->registerArray.set(registerArray);
        d->registers = registers;
    }

} // namespace AJ

#endif // JSVariableObject_h
