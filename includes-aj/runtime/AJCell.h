
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
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef AJCell_h
#define AJCell_h

#include "Collector.h"
#include "JSImmediate.h"
#include "AJValue.h"
#include "MarkStack.h"
#include "Structure.h"
#include <wtf/Noncopyable.h>

namespace AJ {

    class AJCell : public NoncopyableCustomAllocated {
        friend class GetterSetter;
        friend class Heap;
        friend class JIT;
        friend class JSNumberCell;
        friend class AJObject;
        friend class AJPropertyNameIterator;
        friend class AJString;
        friend class AJValue;
        friend class AJAPIValueWrapper;
        friend class JSZombie;
        friend class AJGlobalData;

    private:
        explicit AJCell(Structure*);
        virtual ~AJCell();

    public:
        static PassRefPtr<Structure> createDummyStructure()
        {
            return Structure::create(jsNull(), TypeInfo(UnspecifiedType), AnonymousSlotCount);
        }

        // Querying the type.
#if USE(JSVALUE32)
        bool isNumber() const;
#endif
        bool isString() const;
        bool isObject() const;
        virtual bool isGetterSetter() const;
        bool inherits(const ClassInfo*) const;
        virtual bool isAPIValueWrapper() const { return false; }
        virtual bool isPropertyNameIterator() const { return false; }

        Structure* structure() const;

        // Extracting the value.
        bool getString(ExecState* exec, UString&) const;
        UString getString(ExecState* exec) const; // null string if not a string
        AJObject* getObject(); // NULL if not an object
        const AJObject* getObject() const; // NULL if not an object
        
        virtual CallType getCallData(CallData&);
        virtual ConstructType getConstructData(ConstructData&);

        // Extracting integer values.
        // FIXME: remove these methods, can check isNumberCell in AJValue && then call asNumberCell::*.
        virtual bool getUInt32(uint32_t&) const;

        // Basic conversions.
        virtual AJValue toPrimitive(ExecState*, PreferredPrimitiveType) const;
        virtual bool getPrimitiveNumber(ExecState*, double& number, AJValue&);
        virtual bool toBoolean(ExecState*) const;
        virtual double toNumber(ExecState*) const;
        virtual UString toString(ExecState*) const;
        virtual AJObject* toObject(ExecState*) const;

        // Garbage collection.
        void* operator new(size_t, ExecState*);
        void* operator new(size_t, AJGlobalData*);
        void* operator new(size_t, void* placementNewDestination) { return placementNewDestination; }

        virtual void markChildren(MarkStack&);
#if ENABLE(JSC_ZOMBIES)
        virtual bool isZombie() const { return false; }
#endif

        // Object operations, with the toObject operation included.
        virtual const ClassInfo* classInfo() const;
        virtual void put(ExecState*, const Identifier& propertyName, AJValue, PutPropertySlot&);
        virtual void put(ExecState*, unsigned propertyName, AJValue);
        virtual bool deleteProperty(ExecState*, const Identifier& propertyName);
        virtual bool deleteProperty(ExecState*, unsigned propertyName);

        virtual AJObject* toThisObject(ExecState*) const;
        virtual AJValue getJSNumber();
        void* vptr() { return *reinterpret_cast<void**>(this); }
        void setVPtr(void* vptr) { *reinterpret_cast<void**>(this) = vptr; }

        // FIXME: Rename getOwnPropertySlot to virtualGetOwnPropertySlot, and
        // fastGetOwnPropertySlot to getOwnPropertySlot. Callers should always
        // call this function, not its slower virtual counterpart. (For integer
        // property names, we want a similar interface with appropriate optimizations.)
        bool fastGetOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);

    protected:
        static const unsigned AnonymousSlotCount = 0;

    private:
        // Base implementation; for non-object classes implements getPropertySlot.
        virtual bool getOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        virtual bool getOwnPropertySlot(ExecState*, unsigned propertyName, PropertySlot&);
        
        Structure* m_structure;
    };

    inline AJCell::AJCell(Structure* structure)
        : m_structure(structure)
    {
    }

    inline AJCell::~AJCell()
    {
    }

#if USE(JSVALUE32)
    inline bool AJCell::isNumber() const
    {
        return m_structure->typeInfo().type() == NumberType;
    }
#endif

    inline bool AJCell::isObject() const
    {
        return m_structure->typeInfo().type() == ObjectType;
    }

    inline bool AJCell::isString() const
    {
        return m_structure->typeInfo().type() == StringType;
    }

    inline Structure* AJCell::structure() const
    {
        return m_structure;
    }

    inline void AJCell::markChildren(MarkStack&)
    {
    }

    inline void* AJCell::operator new(size_t size, AJGlobalData* globalData)
    {
        return globalData->heap.allocate(size);
    }

    inline void* AJCell::operator new(size_t size, ExecState* exec)
    {
        return exec->heap()->allocate(size);
    }

    // --- AJValue inlines ----------------------------

    inline bool AJValue::isString() const
    {
        return isCell() && asCell()->isString();
    }

    inline bool AJValue::isGetterSetter() const
    {
        return isCell() && asCell()->isGetterSetter();
    }

    inline bool AJValue::isObject() const
    {
        return isCell() && asCell()->isObject();
    }

    inline bool AJValue::getString(ExecState* exec, UString& s) const
    {
        return isCell() && asCell()->getString(exec, s);
    }

    inline UString AJValue::getString(ExecState* exec) const
    {
        return isCell() ? asCell()->getString(exec) : UString();
    }

    inline AJObject* AJValue::getObject() const
    {
        return isCell() ? asCell()->getObject() : 0;
    }

    inline CallType AJValue::getCallData(CallData& callData)
    {
        return isCell() ? asCell()->getCallData(callData) : CallTypeNone;
    }

    inline ConstructType AJValue::getConstructData(ConstructData& constructData)
    {
        return isCell() ? asCell()->getConstructData(constructData) : ConstructTypeNone;
    }

    ALWAYS_INLINE bool AJValue::getUInt32(uint32_t& v) const
    {
        if (isInt32()) {
            int32_t i = asInt32();
            v = static_cast<uint32_t>(i);
            return i >= 0;
        }
        if (isDouble()) {
            double d = asDouble();
            v = static_cast<uint32_t>(d);
            return v == d;
        }
        return false;
    }

#if !USE(JSVALUE32_64)
    ALWAYS_INLINE AJCell* AJValue::asCell() const
    {
        ASSERT(isCell());
        return m_ptr;
    }
#endif // !USE(JSVALUE32_64)

    inline AJValue AJValue::toPrimitive(ExecState* exec, PreferredPrimitiveType preferredType) const
    {
        return isCell() ? asCell()->toPrimitive(exec, preferredType) : asValue();
    }

    inline bool AJValue::getPrimitiveNumber(ExecState* exec, double& number, AJValue& value)
    {
        if (isInt32()) {
            number = asInt32();
            value = *this;
            return true;
        }
        if (isDouble()) {
            number = asDouble();
            value = *this;
            return true;
        }
        if (isCell())
            return asCell()->getPrimitiveNumber(exec, number, value);
        if (isTrue()) {
            number = 1.0;
            value = *this;
            return true;
        }
        if (isFalse() || isNull()) {
            number = 0.0;
            value = *this;
            return true;
        }
        ASSERT(isUndefined());
        number = nonInlineNaN();
        value = *this;
        return true;
    }

    inline bool AJValue::toBoolean(ExecState* exec) const
    {
        if (isInt32())
            return asInt32() != 0;
        if (isDouble())
            return asDouble() > 0.0 || asDouble() < 0.0; // false for NaN
        if (isCell())
            return asCell()->toBoolean(exec);
        return isTrue(); // false, null, and undefined all convert to false.
    }

    ALWAYS_INLINE double AJValue::toNumber(ExecState* exec) const
    {
        if (isInt32())
            return asInt32();
        if (isDouble())
            return asDouble();
        if (isCell())
            return asCell()->toNumber(exec);
        if (isTrue())
            return 1.0;
        return isUndefined() ? nonInlineNaN() : 0; // null and false both convert to 0.
    }

    inline bool AJValue::needsThisConversion() const
    {
        if (UNLIKELY(!isCell()))
            return true;
        return asCell()->structure()->typeInfo().needsThisConversion();
    }

    inline AJValue AJValue::getJSNumber()
    {
        if (isInt32() || isDouble())
            return *this;
        if (isCell())
            return asCell()->getJSNumber();
        return AJValue();
    }

    inline AJObject* AJValue::toObject(ExecState* exec) const
    {
        return isCell() ? asCell()->toObject(exec) : toObjectSlowCase(exec);
    }

    inline AJObject* AJValue::toThisObject(ExecState* exec) const
    {
        return isCell() ? asCell()->toThisObject(exec) : toThisObjectSlowCase(exec);
    }

    ALWAYS_INLINE void MarkStack::append(AJCell* cell)
    {
        ASSERT(!m_isCheckingForDefaultMarkViolation);
        ASSERT(cell);
        if (Heap::isCellMarked(cell))
            return;
        Heap::markCell(cell);
        if (cell->structure()->typeInfo().type() >= CompoundType)
            m_values.append(cell);
    }

    ALWAYS_INLINE void MarkStack::append(AJValue value)
    {
        ASSERT(value);
        if (value.isCell())
            append(value.asCell());
    }

    inline Heap* Heap::heap(AJValue v)
    {
        if (!v.isCell())
            return 0;
        return heap(v.asCell());
    }

    inline Heap* Heap::heap(AJCell* c)
    {
        return cellBlock(c)->heap;
    }
    
#if ENABLE(JSC_ZOMBIES)
    inline bool AJValue::isZombie() const
    {
        return isCell() && asCell() && asCell()->isZombie();
    }
#endif
} // namespace AJ

#endif // AJCell_h
