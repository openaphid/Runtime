
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

#ifndef AJValue_h
#define AJValue_h

#include "CallData.h"
#include "ConstructData.h"
#include <math.h>
#include <stddef.h> // for size_t
#include <stdint.h>
#include <wtf/AlwaysInline.h>
#include <wtf/Assertions.h>
#include <wtf/HashTraits.h>
#include <wtf/MathExtras.h>

namespace AJ {

    class Identifier;
    class AJCell;
    class AJGlobalData;
    class JSImmediate;
    class AJObject;
    class AJString;
    class PropertySlot;
    class PutPropertySlot;
    class UString;

    struct ClassInfo;
    struct Instruction;

    enum PreferredPrimitiveType { NoPreference, PreferNumber, PreferString };

#if USE(JSVALUE32_64)
    typedef int64_t EncodedAJValue;
#else
    typedef void* EncodedAJValue;
#endif

    double nonInlineNaN();
    int32_t toInt32SlowCase(double, bool& ok);
    uint32_t toUInt32SlowCase(double, bool& ok);

    class AJValue {
        friend class JSImmediate;
        friend struct EncodedAJValueHashTraits;
        friend class JIT;
        friend class JITStubs;
        friend class JITStubCall;
        friend class JSInterfaceJIT;
        friend class SpecializedThunkJIT;

    public:
        static EncodedAJValue encode(AJValue value);
        static AJValue decode(EncodedAJValue ptr);
#if !USE(JSVALUE32_64)
    private:
        static AJValue makeImmediate(intptr_t value);
        intptr_t immediateValue();
    public:
#endif
        enum JSNullTag { JSNull };
        enum JSUndefinedTag { JSUndefined };
        enum JSTrueTag { JSTrue };
        enum JSFalseTag { JSFalse };
        enum EncodeAsDoubleTag { EncodeAsDouble };

        AJValue();
        AJValue(JSNullTag);
        AJValue(JSUndefinedTag);
        AJValue(JSTrueTag);
        AJValue(JSFalseTag);
        AJValue(AJCell* ptr);
        AJValue(const AJCell* ptr);

        // Numbers
        AJValue(EncodeAsDoubleTag, ExecState*, double);
        AJValue(ExecState*, double);
        AJValue(ExecState*, char);
        AJValue(ExecState*, unsigned char);
        AJValue(ExecState*, short);
        AJValue(ExecState*, unsigned short);
        AJValue(ExecState*, int);
        AJValue(ExecState*, unsigned);
        AJValue(ExecState*, long);
        AJValue(ExecState*, unsigned long);
        AJValue(ExecState*, long long);
        AJValue(ExecState*, unsigned long long);
        AJValue(AJGlobalData*, double);
        AJValue(AJGlobalData*, int);
        AJValue(AJGlobalData*, unsigned);

        operator bool() const;
        bool operator==(const AJValue& other) const;
        bool operator!=(const AJValue& other) const;

        bool isInt32() const;
        bool isUInt32() const;
        bool isDouble() const;
        bool isTrue() const;
        bool isFalse() const;

        int32_t asInt32() const;
        uint32_t asUInt32() const;
        double asDouble() const;

        // Querying the type.
        bool isUndefined() const;
        bool isNull() const;
        bool isUndefinedOrNull() const;
        bool isBoolean() const;
        bool isNumber() const;
        bool isString() const;
        bool isGetterSetter() const;
        bool isObject() const;
        bool inherits(const ClassInfo*) const;
        
        // Extracting the value.
        bool getBoolean(bool&) const;
        bool getBoolean() const; // false if not a boolean
        bool getNumber(double&) const;
        double uncheckedGetNumber() const;
        bool getString(ExecState* exec, UString&) const;
        UString getString(ExecState* exec) const; // null string if not a string
        AJObject* getObject() const; // 0 if not an object

        CallType getCallData(CallData&);
        ConstructType getConstructData(ConstructData&);

        // Extracting integer values.
        bool getUInt32(uint32_t&) const;
        
        // Basic conversions.
        AJValue toPrimitive(ExecState*, PreferredPrimitiveType = NoPreference) const;
        bool getPrimitiveNumber(ExecState*, double& number, AJValue&);

        bool toBoolean(ExecState*) const;

        // toNumber conversion is expected to be side effect free if an exception has
        // been set in the ExecState already.
        double toNumber(ExecState*) const;
        AJValue toJSNumber(ExecState*) const; // Fast path for when you expect that the value is an immediate number.
        UString toString(ExecState*) const;
        UString toPrimitiveString(ExecState*) const;
        AJObject* toObject(ExecState*) const;

        // Integer conversions.
        double toInteger(ExecState*) const;
        double toIntegerPreserveNaN(ExecState*) const;
        int32_t toInt32(ExecState*) const;
        int32_t toInt32(ExecState*, bool& ok) const;
        uint32_t toUInt32(ExecState*) const;
        uint32_t toUInt32(ExecState*, bool& ok) const;

#if ENABLE(JSC_ZOMBIES)
        bool isZombie() const;
#endif

        // Floating point conversions (this is a convenience method for webcore;
        // signle precision float is not a representation used in JS or JSC).
        float toFloat(ExecState* exec) const { return static_cast<float>(toNumber(exec)); }

        // Object operations, with the toObject operation included.
        AJValue get(ExecState*, const Identifier& propertyName) const;
        AJValue get(ExecState*, const Identifier& propertyName, PropertySlot&) const;
        AJValue get(ExecState*, unsigned propertyName) const;
        AJValue get(ExecState*, unsigned propertyName, PropertySlot&) const;
        void put(ExecState*, const Identifier& propertyName, AJValue, PutPropertySlot&);
        void putDirect(ExecState*, const Identifier& propertyName, AJValue, PutPropertySlot&);
        void put(ExecState*, unsigned propertyName, AJValue);

        bool needsThisConversion() const;
        AJObject* toThisObject(ExecState*) const;
        UString toThisString(ExecState*) const;
        AJString* toThisAJString(ExecState*) const;

        static bool equal(ExecState* exec, AJValue v1, AJValue v2);
        static bool equalSlowCase(ExecState* exec, AJValue v1, AJValue v2);
        static bool equalSlowCaseInline(ExecState* exec, AJValue v1, AJValue v2);
        static bool strictEqual(ExecState* exec, AJValue v1, AJValue v2);
        static bool strictEqualSlowCase(ExecState* exec, AJValue v1, AJValue v2);
        static bool strictEqualSlowCaseInline(ExecState* exec, AJValue v1, AJValue v2);

        AJValue getJSNumber(); // AJValue() if this is not a JSNumber or number object

        bool isCell() const;
        AJCell* asCell() const;

#ifndef NDEBUG
        char* description();
#endif

    private:
        enum HashTableDeletedValueTag { HashTableDeletedValue };
        AJValue(HashTableDeletedValueTag);

        inline const AJValue asValue() const { return *this; }
        AJObject* toObjectSlowCase(ExecState*) const;
        AJObject* toThisObjectSlowCase(ExecState*) const;

        AJObject* synthesizePrototype(ExecState*) const;
        AJObject* synthesizeObject(ExecState*) const;

#if USE(JSVALUE32_64)
        enum { Int32Tag =        0xffffffff };
        enum { CellTag =         0xfffffffe };
        enum { TrueTag =         0xfffffffd };
        enum { FalseTag =        0xfffffffc };
        enum { NullTag =         0xfffffffb };
        enum { UndefinedTag =    0xfffffffa };
        enum { EmptyValueTag =   0xfffffff9 };
        enum { DeletedValueTag = 0xfffffff8 };
        
        enum { LowestTag =  DeletedValueTag };
        
        uint32_t tag() const;
        int32_t payload() const;

        union {
            EncodedAJValue asEncodedAJValue;
            double asDouble;
#if CPU(BIG_ENDIAN)
            struct {
                int32_t tag;
                int32_t payload;
            } asBits;
#else
            struct {
                int32_t payload;
                int32_t tag;
            } asBits;
#endif
        } u;
#else // USE(JSVALUE32_64)
        AJCell* m_ptr;
#endif // USE(JSVALUE32_64)
    };

#if USE(JSVALUE32_64)
    typedef IntHash<EncodedAJValue> EncodedAJValueHash;

    struct EncodedAJValueHashTraits : HashTraits<EncodedAJValue> {
        static const bool emptyValueIsZero = false;
        static EncodedAJValue emptyValue() { return AJValue::encode(AJValue()); }
        static void constructDeletedValue(EncodedAJValue& slot) { slot = AJValue::encode(AJValue(AJValue::HashTableDeletedValue)); }
        static bool isDeletedValue(EncodedAJValue value) { return value == AJValue::encode(AJValue(AJValue::HashTableDeletedValue)); }
    };
#else
    typedef PtrHash<EncodedAJValue> EncodedAJValueHash;

    struct EncodedAJValueHashTraits : HashTraits<EncodedAJValue> {
        static void constructDeletedValue(EncodedAJValue& slot) { slot = AJValue::encode(AJValue(AJValue::HashTableDeletedValue)); }
        static bool isDeletedValue(EncodedAJValue value) { return value == AJValue::encode(AJValue(AJValue::HashTableDeletedValue)); }
    };
#endif

    // Stand-alone helper functions.
    inline AJValue jsNull()
    {
        return AJValue(AJValue::JSNull);
    }

    inline AJValue jsUndefined()
    {
        return AJValue(AJValue::JSUndefined);
    }

    inline AJValue jsBoolean(bool b)
    {
        return b ? AJValue(AJValue::JSTrue) : AJValue(AJValue::JSFalse);
    }

    ALWAYS_INLINE AJValue jsDoubleNumber(ExecState* exec, double d)
    {
        return AJValue(AJValue::EncodeAsDouble, exec, d);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, double d)
    {
        return AJValue(exec, d);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, char i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, unsigned char i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, short i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, unsigned short i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, int i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, unsigned i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, long i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, unsigned long i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, long long i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(ExecState* exec, unsigned long long i)
    {
        return AJValue(exec, i);
    }

    ALWAYS_INLINE AJValue jsNumber(AJGlobalData* globalData, double d)
    {
        return AJValue(globalData, d);
    }

    ALWAYS_INLINE AJValue jsNumber(AJGlobalData* globalData, int i)
    {
        return AJValue(globalData, i);
    }

    ALWAYS_INLINE AJValue jsNumber(AJGlobalData* globalData, unsigned i)
    {
        return AJValue(globalData, i);
    }

    inline bool operator==(const AJValue a, const AJCell* b) { return a == AJValue(b); }
    inline bool operator==(const AJCell* a, const AJValue b) { return AJValue(a) == b; }

    inline bool operator!=(const AJValue a, const AJCell* b) { return a != AJValue(b); }
    inline bool operator!=(const AJCell* a, const AJValue b) { return AJValue(a) != b; }

    inline int32_t toInt32(double val)
    {
        if (!(val >= -2147483648.0 && val < 2147483648.0)) {
            bool ignored;
            return toInt32SlowCase(val, ignored);
        }
        return static_cast<int32_t>(val);
    }

    inline uint32_t toUInt32(double val)
    {
        if (!(val >= 0.0 && val < 4294967296.0)) {
            bool ignored;
            return toUInt32SlowCase(val, ignored);
        }
        return static_cast<uint32_t>(val);
    }

    // FIXME: We should deprecate this and just use AJValue::asCell() instead.
    AJCell* asCell(AJValue);

    inline AJCell* asCell(AJValue value)
    {
        return value.asCell();
    }

    ALWAYS_INLINE int32_t AJValue::toInt32(ExecState* exec) const
    {
        if (isInt32())
            return asInt32();
        bool ignored;
        return toInt32SlowCase(toNumber(exec), ignored);
    }

    inline uint32_t AJValue::toUInt32(ExecState* exec) const
    {
        if (isUInt32())
            return asInt32();
        bool ignored;
        return toUInt32SlowCase(toNumber(exec), ignored);
    }

    inline int32_t AJValue::toInt32(ExecState* exec, bool& ok) const
    {
        if (isInt32()) {
            ok = true;
            return asInt32();
        }
        return toInt32SlowCase(toNumber(exec), ok);
    }

    inline uint32_t AJValue::toUInt32(ExecState* exec, bool& ok) const
    {
        if (isUInt32()) {
            ok = true;
            return asInt32();
        }
        return toUInt32SlowCase(toNumber(exec), ok);
    }

#if USE(JSVALUE32_64)
    inline AJValue jsNaN(ExecState* exec)
    {
        return AJValue(exec, nonInlineNaN());
    }

    // AJValue member functions.
    inline EncodedAJValue AJValue::encode(AJValue value)
    {
        return value.u.asEncodedAJValue;
    }

    inline AJValue AJValue::decode(EncodedAJValue encodedAJValue)
    {
        AJValue v;
        v.u.asEncodedAJValue = encodedAJValue;
#if ENABLE(JSC_ZOMBIES)
        ASSERT(!v.isZombie());
#endif
        return v;
    }

    inline AJValue::AJValue()
    {
        u.asBits.tag = EmptyValueTag;
        u.asBits.payload = 0;
    }

    inline AJValue::AJValue(JSNullTag)
    {
        u.asBits.tag = NullTag;
        u.asBits.payload = 0;
    }
    
    inline AJValue::AJValue(JSUndefinedTag)
    {
        u.asBits.tag = UndefinedTag;
        u.asBits.payload = 0;
    }
    
    inline AJValue::AJValue(JSTrueTag)
    {
        u.asBits.tag = TrueTag;
        u.asBits.payload = 0;
    }
    
    inline AJValue::AJValue(JSFalseTag)
    {
        u.asBits.tag = FalseTag;
        u.asBits.payload = 0;
    }

    inline AJValue::AJValue(HashTableDeletedValueTag)
    {
        u.asBits.tag = DeletedValueTag;
        u.asBits.payload = 0;
    }

    inline AJValue::AJValue(AJCell* ptr)
    {
        if (ptr)
            u.asBits.tag = CellTag;
        else
            u.asBits.tag = EmptyValueTag;
        u.asBits.payload = reinterpret_cast<int32_t>(ptr);
#if ENABLE(JSC_ZOMBIES)
        ASSERT(!isZombie());
#endif
    }

    inline AJValue::AJValue(const AJCell* ptr)
    {
        if (ptr)
            u.asBits.tag = CellTag;
        else
            u.asBits.tag = EmptyValueTag;
        u.asBits.payload = reinterpret_cast<int32_t>(const_cast<AJCell*>(ptr));
#if ENABLE(JSC_ZOMBIES)
        ASSERT(!isZombie());
#endif
    }

    inline AJValue::operator bool() const
    {
        ASSERT(tag() != DeletedValueTag);
        return tag() != EmptyValueTag;
    }

    inline bool AJValue::operator==(const AJValue& other) const
    {
        return u.asEncodedAJValue == other.u.asEncodedAJValue;
    }

    inline bool AJValue::operator!=(const AJValue& other) const
    {
        return u.asEncodedAJValue != other.u.asEncodedAJValue;
    }

    inline bool AJValue::isUndefined() const
    {
        return tag() == UndefinedTag;
    }

    inline bool AJValue::isNull() const
    {
        return tag() == NullTag;
    }

    inline bool AJValue::isUndefinedOrNull() const
    {
        return isUndefined() || isNull();
    }

    inline bool AJValue::isCell() const
    {
        return tag() == CellTag;
    }

    inline bool AJValue::isInt32() const
    {
        return tag() == Int32Tag;
    }

    inline bool AJValue::isUInt32() const
    {
        return tag() == Int32Tag && asInt32() > -1;
    }

    inline bool AJValue::isDouble() const
    {
        return tag() < LowestTag;
    }

    inline bool AJValue::isTrue() const
    {
        return tag() == TrueTag;
    }

    inline bool AJValue::isFalse() const
    {
        return tag() == FalseTag;
    }

    inline uint32_t AJValue::tag() const
    {
        return u.asBits.tag;
    }
    
    inline int32_t AJValue::payload() const
    {
        return u.asBits.payload;
    }
    
    inline int32_t AJValue::asInt32() const
    {
        ASSERT(isInt32());
        return u.asBits.payload;
    }
    
    inline uint32_t AJValue::asUInt32() const
    {
        ASSERT(isUInt32());
        return u.asBits.payload;
    }
    
    inline double AJValue::asDouble() const
    {
        ASSERT(isDouble());
        return u.asDouble;
    }
    
    ALWAYS_INLINE AJCell* AJValue::asCell() const
    {
        ASSERT(isCell());
        return reinterpret_cast<AJCell*>(u.asBits.payload);
    }

    ALWAYS_INLINE AJValue::AJValue(EncodeAsDoubleTag, ExecState*, double d)
    {
        u.asDouble = d;
    }

    inline AJValue::AJValue(ExecState* exec, double d)
    {
        const int32_t asInt32 = static_cast<int32_t>(d);
        if (asInt32 != d || (!asInt32 && signbit(d))) { // true for -0.0
            u.asDouble = d;
            return;
        }
        *this = AJValue(exec, static_cast<int32_t>(d));
    }

    inline AJValue::AJValue(ExecState* exec, char i)
    {
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, unsigned char i)
    {
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, short i)
    {
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, unsigned short i)
    {
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState*, int i)
    {
        u.asBits.tag = Int32Tag;
        u.asBits.payload = i;
    }

    inline AJValue::AJValue(ExecState* exec, unsigned i)
    {
        if (static_cast<int32_t>(i) < 0) {
            *this = AJValue(exec, static_cast<double>(i));
            return;
        }
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, long i)
    {
        if (static_cast<int32_t>(i) != i) {
            *this = AJValue(exec, static_cast<double>(i));
            return;
        }
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, unsigned long i)
    {
        if (static_cast<uint32_t>(i) != i) {
            *this = AJValue(exec, static_cast<double>(i));
            return;
        }
        *this = AJValue(exec, static_cast<uint32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, long long i)
    {
        if (static_cast<int32_t>(i) != i) {
            *this = AJValue(exec, static_cast<double>(i));
            return;
        }
        *this = AJValue(exec, static_cast<int32_t>(i));
    }

    inline AJValue::AJValue(ExecState* exec, unsigned long long i)
    {
        if (static_cast<uint32_t>(i) != i) {
            *this = AJValue(exec, static_cast<double>(i));
            return;
        }
        *this = AJValue(exec, static_cast<uint32_t>(i));
    }

    inline AJValue::AJValue(AJGlobalData* globalData, double d)
    {
        const int32_t asInt32 = static_cast<int32_t>(d);
        if (asInt32 != d || (!asInt32 && signbit(d))) { // true for -0.0
            u.asDouble = d;
            return;
        }
        *this = AJValue(globalData, static_cast<int32_t>(d));
    }
    
    inline AJValue::AJValue(AJGlobalData*, int i)
    {
        u.asBits.tag = Int32Tag;
        u.asBits.payload = i;
    }
    
    inline AJValue::AJValue(AJGlobalData* globalData, unsigned i)
    {
        if (static_cast<int32_t>(i) < 0) {
            *this = AJValue(globalData, static_cast<double>(i));
            return;
        }
        *this = AJValue(globalData, static_cast<int32_t>(i));
    }

    inline bool AJValue::isNumber() const
    {
        return isInt32() || isDouble();
    }

    inline bool AJValue::isBoolean() const
    {
        return isTrue() || isFalse();
    }

    inline bool AJValue::getBoolean(bool& v) const
    {
        if (isTrue()) {
            v = true;
            return true;
        }
        if (isFalse()) {
            v = false;
            return true;
        }
        
        return false;
    }

    inline bool AJValue::getBoolean() const
    {
        ASSERT(isBoolean());
        return tag() == TrueTag;
    }

    inline double AJValue::uncheckedGetNumber() const
    {
        ASSERT(isNumber());
        return isInt32() ? asInt32() : asDouble();
    }

    ALWAYS_INLINE AJValue AJValue::toJSNumber(ExecState* exec) const
    {
        return isNumber() ? asValue() : jsNumber(exec, this->toNumber(exec));
    }

    inline bool AJValue::getNumber(double& result) const
    {
        if (isInt32()) {
            result = asInt32();
            return true;
        }
        if (isDouble()) {
            result = asDouble();
            return true;
        }
        return false;
    }

#else // USE(JSVALUE32_64)

    // AJValue member functions.
    inline EncodedAJValue AJValue::encode(AJValue value)
    {
        return reinterpret_cast<EncodedAJValue>(value.m_ptr);
    }

    inline AJValue AJValue::decode(EncodedAJValue ptr)
    {
        return AJValue(reinterpret_cast<AJCell*>(ptr));
    }

    inline AJValue AJValue::makeImmediate(intptr_t value)
    {
        return AJValue(reinterpret_cast<AJCell*>(value));
    }

    inline intptr_t AJValue::immediateValue()
    {
        return reinterpret_cast<intptr_t>(m_ptr);
    }
    
    // 0x0 can never occur naturally because it has a tag of 00, indicating a pointer value, but a payload of 0x0, which is in the (invalid) zero page.
    inline AJValue::AJValue()
        : m_ptr(0)
    {
    }

    // 0x4 can never occur naturally because it has a tag of 00, indicating a pointer value, but a payload of 0x4, which is in the (invalid) zero page.
    inline AJValue::AJValue(HashTableDeletedValueTag)
        : m_ptr(reinterpret_cast<AJCell*>(0x4))
    {
    }

    inline AJValue::AJValue(AJCell* ptr)
        : m_ptr(ptr)
    {
#if ENABLE(JSC_ZOMBIES)
        ASSERT(!isZombie());
#endif
    }

    inline AJValue::AJValue(const AJCell* ptr)
        : m_ptr(const_cast<AJCell*>(ptr))
    {
#if ENABLE(JSC_ZOMBIES)
        ASSERT(!isZombie());
#endif
    }

    inline AJValue::operator bool() const
    {
        return m_ptr;
    }

    inline bool AJValue::operator==(const AJValue& other) const
    {
        return m_ptr == other.m_ptr;
    }

    inline bool AJValue::operator!=(const AJValue& other) const
    {
        return m_ptr != other.m_ptr;
    }

    inline bool AJValue::isUndefined() const
    {
        return asValue() == jsUndefined();
    }

    inline bool AJValue::isNull() const
    {
        return asValue() == jsNull();
    }
#endif // USE(JSVALUE32_64)
    
    typedef std::pair<AJValue, UString> ValueStringPair;
} // namespace AJ

#endif // AJValue_h
