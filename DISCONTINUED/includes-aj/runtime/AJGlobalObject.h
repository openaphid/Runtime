
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
 *  Copyright (C) 2007 Eric Seidel <eric@webkit.org>
 *  Copyright (C) 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef AJGlobalObject_h
#define AJGlobalObject_h

#include "AJArray.h"
#include "AJGlobalData.h"
#include "JSVariableObject.h"
#include "JSWeakObjectMapRefInternal.h"
#include "NativeFunctionWrapper.h"
#include "NumberPrototype.h"
#include "StringPrototype.h"
#include <wtf/HashSet.h>
#include <wtf/OwnPtr.h>
#include <wtf/RandomNumber.h>

namespace AJ {

    class ArrayPrototype;
    class BooleanPrototype;
    class DatePrototype;
    class Debugger;
    class ErrorConstructor;
    class FunctionPrototype;
    class GlobalCodeBlock;
    class GlobalEvalFunction;
    class NativeErrorConstructor;
    class ProgramCodeBlock;
    class PrototypeFunction;
    class RegExpConstructor;
    class RegExpPrototype;
    class RegisterFile;

    struct ActivationStackNode;
    struct HashTable;

    typedef Vector<ExecState*, 16> ExecStateStack;
    
    class AJGlobalObject : public JSVariableObject {
    protected:
        using JSVariableObject::JSVariableObjectData;
        typedef HashSet<RefPtr<OpaqueJSWeakObjectMap> > WeakMapSet;

        struct AJGlobalObjectData : public JSVariableObjectData {
            // We use an explicit destructor function pointer instead of a
            // virtual destructor because we want to avoid adding a vtable
            // pointer to this struct. Adding a vtable pointer would force the
            // compiler to emit costly pointer fixup code when casting from
            // JSVariableObjectData* to AJGlobalObjectData*.
            typedef void (*Destructor)(void*);

            AJGlobalObjectData(Destructor destructor)
                : JSVariableObjectData(&symbolTable, 0)
                , destructor(destructor)
                , registerArraySize(0)
                , globalScopeChain(NoScopeChain())
                , regExpConstructor(0)
                , errorConstructor(0)
                , evalErrorConstructor(0)
                , rangeErrorConstructor(0)
                , referenceErrorConstructor(0)
                , syntaxErrorConstructor(0)
                , typeErrorConstructor(0)
                , URIErrorConstructor(0)
                , evalFunction(0)
                , callFunction(0)
                , applyFunction(0)
                , objectPrototype(0)
                , functionPrototype(0)
                , arrayPrototype(0)
                , booleanPrototype(0)
                , stringPrototype(0)
                , numberPrototype(0)
                , datePrototype(0)
                , regExpPrototype(0)
                , methodCallDummy(0)
                , weakRandom(static_cast<unsigned>(randomNumber() * (std::numeric_limits<unsigned>::max() + 1.0)))
            {
            }
            
            Destructor destructor;
            
            size_t registerArraySize;

            AJGlobalObject* next;
            AJGlobalObject* prev;

            Debugger* debugger;
            
            ScopeChain globalScopeChain;
            Register globalCallFrame[RegisterFile::CallFrameHeaderSize];

            int recursion;

            RegExpConstructor* regExpConstructor;
            ErrorConstructor* errorConstructor;
            NativeErrorConstructor* evalErrorConstructor;
            NativeErrorConstructor* rangeErrorConstructor;
            NativeErrorConstructor* referenceErrorConstructor;
            NativeErrorConstructor* syntaxErrorConstructor;
            NativeErrorConstructor* typeErrorConstructor;
            NativeErrorConstructor* URIErrorConstructor;

            GlobalEvalFunction* evalFunction;
            NativeFunctionWrapper* callFunction;
            NativeFunctionWrapper* applyFunction;

            ObjectPrototype* objectPrototype;
            FunctionPrototype* functionPrototype;
            ArrayPrototype* arrayPrototype;
            BooleanPrototype* booleanPrototype;
            StringPrototype* stringPrototype;
            NumberPrototype* numberPrototype;
            DatePrototype* datePrototype;
            RegExpPrototype* regExpPrototype;

            AJObject* methodCallDummy;

            RefPtr<Structure> argumentsStructure;
            RefPtr<Structure> arrayStructure;
            RefPtr<Structure> booleanObjectStructure;
            RefPtr<Structure> callbackConstructorStructure;
            RefPtr<Structure> callbackFunctionStructure;
            RefPtr<Structure> callbackObjectStructure;
            RefPtr<Structure> dateStructure;
            RefPtr<Structure> emptyObjectStructure;
            RefPtr<Structure> errorStructure;
            RefPtr<Structure> functionStructure;
            RefPtr<Structure> numberObjectStructure;
            RefPtr<Structure> prototypeFunctionStructure;
            RefPtr<Structure> regExpMatchesArrayStructure;
            RefPtr<Structure> regExpStructure;
            RefPtr<Structure> stringObjectStructure;

            SymbolTable symbolTable;
            unsigned profileGroup;

            RefPtr<AJGlobalData> globalData;

            HashSet<GlobalCodeBlock*> codeBlocks;
            WeakMapSet weakMaps;
            WeakRandom weakRandom;
        };

    public:
        void* operator new(size_t, AJGlobalData*);

        explicit AJGlobalObject()
            : JSVariableObject(AJGlobalObject::createStructure(jsNull()), new AJGlobalObjectData(destroyAJGlobalObjectData))
        {
            init(this);
        }

    protected:
        AJGlobalObject(NonNullPassRefPtr<Structure> structure, AJGlobalObjectData* data, AJObject* thisValue)
            : JSVariableObject(structure, data)
        {
            init(thisValue);
        }

    public:
        virtual ~AJGlobalObject();

        virtual void markChildren(MarkStack&);

        virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
        virtual bool hasOwnPropertyForWrite(ExecState*, const Identifier&);
        virtual void put(ExecState*, const Identifier&, AJValue, PutPropertySlot&);
        virtual void putWithAttributes(ExecState*, const Identifier& propertyName, AJValue value, unsigned attributes);

        virtual void defineGetter(ExecState*, const Identifier& propertyName, AJObject* getterFunc, unsigned attributes);
        virtual void defineSetter(ExecState*, const Identifier& propertyName, AJObject* setterFunc, unsigned attributes);

        // Linked list of all global objects that use the same AJGlobalData.
        AJGlobalObject*& head() { return d()->globalData->head; }
        AJGlobalObject* next() { return d()->next; }

        // The following accessors return pristine values, even if a script 
        // replaces the global object's associated property.

        RegExpConstructor* regExpConstructor() const { return d()->regExpConstructor; }

        ErrorConstructor* errorConstructor() const { return d()->errorConstructor; }
        NativeErrorConstructor* evalErrorConstructor() const { return d()->evalErrorConstructor; }
        NativeErrorConstructor* rangeErrorConstructor() const { return d()->rangeErrorConstructor; }
        NativeErrorConstructor* referenceErrorConstructor() const { return d()->referenceErrorConstructor; }
        NativeErrorConstructor* syntaxErrorConstructor() const { return d()->syntaxErrorConstructor; }
        NativeErrorConstructor* typeErrorConstructor() const { return d()->typeErrorConstructor; }
        NativeErrorConstructor* URIErrorConstructor() const { return d()->URIErrorConstructor; }

        GlobalEvalFunction* evalFunction() const { return d()->evalFunction; }

        ObjectPrototype* objectPrototype() const { return d()->objectPrototype; }
        FunctionPrototype* functionPrototype() const { return d()->functionPrototype; }
        ArrayPrototype* arrayPrototype() const { return d()->arrayPrototype; }
        BooleanPrototype* booleanPrototype() const { return d()->booleanPrototype; }
        StringPrototype* stringPrototype() const { return d()->stringPrototype; }
        NumberPrototype* numberPrototype() const { return d()->numberPrototype; }
        DatePrototype* datePrototype() const { return d()->datePrototype; }
        RegExpPrototype* regExpPrototype() const { return d()->regExpPrototype; }

        AJObject* methodCallDummy() const { return d()->methodCallDummy; }

        Structure* argumentsStructure() const { return d()->argumentsStructure.get(); }
        Structure* arrayStructure() const { return d()->arrayStructure.get(); }
        Structure* booleanObjectStructure() const { return d()->booleanObjectStructure.get(); }
        Structure* callbackConstructorStructure() const { return d()->callbackConstructorStructure.get(); }
        Structure* callbackFunctionStructure() const { return d()->callbackFunctionStructure.get(); }
        Structure* callbackObjectStructure() const { return d()->callbackObjectStructure.get(); }
        Structure* dateStructure() const { return d()->dateStructure.get(); }
        Structure* emptyObjectStructure() const { return d()->emptyObjectStructure.get(); }
        Structure* errorStructure() const { return d()->errorStructure.get(); }
        Structure* functionStructure() const { return d()->functionStructure.get(); }
        Structure* numberObjectStructure() const { return d()->numberObjectStructure.get(); }
        Structure* prototypeFunctionStructure() const { return d()->prototypeFunctionStructure.get(); }
        Structure* regExpMatchesArrayStructure() const { return d()->regExpMatchesArrayStructure.get(); }
        Structure* regExpStructure() const { return d()->regExpStructure.get(); }
        Structure* stringObjectStructure() const { return d()->stringObjectStructure.get(); }

        void setProfileGroup(unsigned value) { d()->profileGroup = value; }
        unsigned profileGroup() const { return d()->profileGroup; }

        Debugger* debugger() const { return d()->debugger; }
        void setDebugger(Debugger* debugger) { d()->debugger = debugger; }
        
        virtual bool supportsProfiling() const { return false; }
        
        int recursion() { return d()->recursion; }
        void incRecursion() { ++d()->recursion; }
        void decRecursion() { --d()->recursion; }
        
        ScopeChain& globalScopeChain() { return d()->globalScopeChain; }

        virtual bool isGlobalObject() const { return true; }

        virtual ExecState* globalExec();

        virtual bool shouldInterruptScriptBeforeTimeout() const { return false; }
        virtual bool shouldInterruptScript() const { return true; }

        virtual bool allowsAccessFrom(const AJGlobalObject*) const { return true; }

        virtual bool isDynamicScope(bool& requiresDynamicChecks) const;

        HashSet<GlobalCodeBlock*>& codeBlocks() { return d()->codeBlocks; }

        void copyGlobalsFrom(RegisterFile&);
        void copyGlobalsTo(RegisterFile&);
        
        void resetPrototype(AJValue prototype);

        AJGlobalData* globalData() { return d()->globalData.get(); }
        AJGlobalObjectData* d() const { return static_cast<AJGlobalObjectData*>(JSVariableObject::d); }

        static PassRefPtr<Structure> createStructure(AJValue prototype)
        {
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount);
        }

        void registerWeakMap(OpaqueJSWeakObjectMap* map)
        {
            d()->weakMaps.add(map);
        }

        void deregisterWeakMap(OpaqueJSWeakObjectMap* map)
        {
            d()->weakMaps.remove(map);
        }

        double weakRandomNumber() { return d()->weakRandom.get(); }
    protected:

        static const unsigned StructureFlags = OverridesGetOwnPropertySlot | OverridesMarkChildren | OverridesGetPropertyNames | JSVariableObject::StructureFlags;

        struct GlobalPropertyInfo {
            GlobalPropertyInfo(const Identifier& i, AJValue v, unsigned a)
                : identifier(i)
                , value(v)
                , attributes(a)
            {
            }

            const Identifier identifier;
            AJValue value;
            unsigned attributes;
        };
        void addStaticGlobals(GlobalPropertyInfo*, int count);

    private:
        static void destroyAJGlobalObjectData(void*);

        // FIXME: Fold reset into init.
        void init(AJObject* thisValue);
        void reset(AJValue prototype);

        void setRegisters(Register* registers, Register* registerArray, size_t count);

        void* operator new(size_t); // can only be allocated with AJGlobalData
    };

    AJGlobalObject* asGlobalObject(AJValue);

    inline AJGlobalObject* asGlobalObject(AJValue value)
    {
        ASSERT(asObject(value)->isGlobalObject());
        return static_cast<AJGlobalObject*>(asObject(value));
    }

    inline void AJGlobalObject::setRegisters(Register* registers, Register* registerArray, size_t count)
    {
        JSVariableObject::setRegisters(registers, registerArray);
        d()->registerArraySize = count;
    }

    inline void AJGlobalObject::addStaticGlobals(GlobalPropertyInfo* globals, int count)
    {
        size_t oldSize = d()->registerArraySize;
        size_t newSize = oldSize + count;
        Register* registerArray = new Register[newSize];
        if (d()->registerArray)
            memcpy(registerArray + count, d()->registerArray.get(), oldSize * sizeof(Register));
        setRegisters(registerArray + newSize, registerArray, newSize);

        for (int i = 0, index = -static_cast<int>(oldSize) - 1; i < count; ++i, --index) {
            GlobalPropertyInfo& global = globals[i];
            ASSERT(global.attributes & DontDelete);
            SymbolTableEntry newEntry(index, global.attributes);
            symbolTable().add(global.identifier.ustring().rep(), newEntry);
            registerAt(index) = global.value;
        }
    }

    inline bool AJGlobalObject::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
    {
        if (JSVariableObject::getOwnPropertySlot(exec, propertyName, slot))
            return true;
        return symbolTableGet(propertyName, slot);
    }

    inline bool AJGlobalObject::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
    {
        if (symbolTableGet(propertyName, descriptor))
            return true;
        return JSVariableObject::getOwnPropertyDescriptor(exec, propertyName, descriptor);
    }

    inline bool AJGlobalObject::hasOwnPropertyForWrite(ExecState* exec, const Identifier& propertyName)
    {
        PropertySlot slot;
        if (JSVariableObject::getOwnPropertySlot(exec, propertyName, slot))
            return true;
        bool slotIsWriteable;
        return symbolTableGet(propertyName, slot, slotIsWriteable);
    }

    inline AJValue Structure::prototypeForLookup(ExecState* exec) const
    {
        if (typeInfo().type() == ObjectType)
            return m_prototype;

#if USE(JSVALUE32)
        if (typeInfo().type() == StringType)
            return exec->lexicalGlobalObject()->stringPrototype();

        ASSERT(typeInfo().type() == NumberType);
        return exec->lexicalGlobalObject()->numberPrototype();
#else
        ASSERT(typeInfo().type() == StringType);
        return exec->lexicalGlobalObject()->stringPrototype();
#endif
    }

    inline StructureChain* Structure::prototypeChain(ExecState* exec) const
    {
        // We cache our prototype chain so our clients can share it.
        if (!isValid(exec, m_cachedPrototypeChain.get())) {
            AJValue prototype = prototypeForLookup(exec);
            m_cachedPrototypeChain = StructureChain::create(prototype.isNull() ? 0 : asObject(prototype)->structure());
        }
        return m_cachedPrototypeChain.get();
    }

    inline bool Structure::isValid(ExecState* exec, StructureChain* cachedPrototypeChain) const
    {
        if (!cachedPrototypeChain)
            return false;

        AJValue prototype = prototypeForLookup(exec);
        RefPtr<Structure>* cachedStructure = cachedPrototypeChain->head();
        while(*cachedStructure && !prototype.isNull()) {
            if (asObject(prototype)->structure() != *cachedStructure)
                return false;
            ++cachedStructure;
            prototype = asObject(prototype)->prototype();
        }
        return prototype.isNull() && !*cachedStructure;
    }

    inline AJGlobalObject* ExecState::dynamicGlobalObject()
    {
        if (this == lexicalGlobalObject()->globalExec())
            return lexicalGlobalObject();

        // For any ExecState that's not a globalExec, the 
        // dynamic global object must be set since code is running
        ASSERT(globalData().dynamicGlobalObject);
        return globalData().dynamicGlobalObject;
    }

    inline AJObject* constructEmptyObject(ExecState* exec)
    {
        return new (exec) AJObject(exec->lexicalGlobalObject()->emptyObjectStructure());
    }
    
    inline AJObject* constructEmptyObject(ExecState* exec, AJGlobalObject* globalObject)
    {
        return new (exec) AJObject(globalObject->emptyObjectStructure());
    }

    inline AJArray* constructEmptyArray(ExecState* exec)
    {
        return new (exec) AJArray(exec->lexicalGlobalObject()->arrayStructure());
    }
    
    inline AJArray* constructEmptyArray(ExecState* exec, AJGlobalObject* globalObject)
    {
        return new (exec) AJArray(globalObject->arrayStructure());
    }

    inline AJArray* constructEmptyArray(ExecState* exec, unsigned initialLength)
    {
        return new (exec) AJArray(exec->lexicalGlobalObject()->arrayStructure(), initialLength);
    }

    inline AJArray* constructArray(ExecState* exec, AJValue singleItemValue)
    {
        MarkedArgumentBuffer values;
        values.append(singleItemValue);
        return new (exec) AJArray(exec->lexicalGlobalObject()->arrayStructure(), values);
    }

    inline AJArray* constructArray(ExecState* exec, const ArgList& values)
    {
        return new (exec) AJArray(exec->lexicalGlobalObject()->arrayStructure(), values);
    }

    class DynamicGlobalObjectScope : public Noncopyable {
    public:
        DynamicGlobalObjectScope(CallFrame* callFrame, AJGlobalObject* dynamicGlobalObject);

        ~DynamicGlobalObjectScope()
        {
            m_dynamicGlobalObjectSlot = m_savedDynamicGlobalObject;
        }

    private:
        AJGlobalObject*& m_dynamicGlobalObjectSlot;
        AJGlobalObject* m_savedDynamicGlobalObject;
    };

} // namespace AJ

#endif // AJGlobalObject_h
