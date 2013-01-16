
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
 *  Copyright (C) 2003, 2004, 2005, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef JSNumberCell_h
#define JSNumberCell_h

#include "CallFrame.h"
#include "AJCell.h"
#include "JSImmediate.h"
#include "Collector.h"
#include "UString.h"
#include <stddef.h> // for size_t

namespace AJ {

    extern const double NaN;
    extern const double Inf;

#if USE(JSVALUE32)
    AJValue jsNumberCell(ExecState*, double);

    class Identifier;
    class AJCell;
    class AJObject;
    class AJString;
    class PropertySlot;

    struct ClassInfo;
    struct Instruction;

    class JSNumberCell : public AJCell {
        friend class JIT;
        friend AJValue jsNumberCell(AJGlobalData*, double);
        friend AJValue jsNumberCell(ExecState*, double);

    public:
        double value() const { return m_value; }

        virtual AJValue toPrimitive(ExecState*, PreferredPrimitiveType) const;
        virtual bool getPrimitiveNumber(ExecState*, double& number, AJValue& value);
        virtual bool toBoolean(ExecState*) const;
        virtual double toNumber(ExecState*) const;
        virtual UString toString(ExecState*) const;
        virtual AJObject* toObject(ExecState*) const;

        virtual AJObject* toThisObject(ExecState*) const;
        virtual AJValue getJSNumber();

        void* operator new(size_t size, ExecState* exec)
        {
            return exec->heap()->allocateNumber(size);
        }

        void* operator new(size_t size, AJGlobalData* globalData)
        {
            return globalData->heap.allocateNumber(size);
        }

        static PassRefPtr<Structure> createStructure(AJValue proto) { return Structure::create(proto, TypeInfo(NumberType, OverridesGetOwnPropertySlot | NeedsThisConversion), AnonymousSlotCount); }

    private:
        JSNumberCell(AJGlobalData* globalData, double value)
            : AJCell(globalData->numberStructure.get())
            , m_value(value)
        {
        }

        JSNumberCell(ExecState* exec, double value)
            : AJCell(exec->globalData().numberStructure.get())
            , m_value(value)
        {
        }

        virtual bool getUInt32(uint32_t&) const;

        double m_value;
    };

    AJValue jsNumberCell(AJGlobalData*, double);

    inline bool isNumberCell(AJValue v)
    {
        return v.isCell() && v.asCell()->isNumber();
    }

    inline JSNumberCell* asNumberCell(AJValue v)
    {
        ASSERT(isNumberCell(v));
        return static_cast<JSNumberCell*>(v.asCell());
    }

    ALWAYS_INLINE AJValue::AJValue(EncodeAsDoubleTag, ExecState* exec, double d)
    {
        *this = jsNumberCell(exec, d);
    }

    inline AJValue::AJValue(ExecState* exec, double d)
    {
        AJValue v = JSImmediate::from(d);
        *this = v ? v : jsNumberCell(exec, d);
    }

    inline AJValue::AJValue(ExecState* exec, int i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline AJValue::AJValue(ExecState* exec, unsigned i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline AJValue::AJValue(ExecState* exec, long i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline AJValue::AJValue(ExecState* exec, unsigned long i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline AJValue::AJValue(ExecState* exec, long long i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, static_cast<double>(i));
    }

    inline AJValue::AJValue(ExecState* exec, unsigned long long i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, static_cast<double>(i));
    }

    inline AJValue::AJValue(AJGlobalData* globalData, double d)
    {
        AJValue v = JSImmediate::from(d);
        *this = v ? v : jsNumberCell(globalData, d);
    }

    inline AJValue::AJValue(AJGlobalData* globalData, int i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(globalData, i);
    }

    inline AJValue::AJValue(AJGlobalData* globalData, unsigned i)
    {
        AJValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(globalData, i);
    }

    inline bool AJValue::isDouble() const
    {
        return isNumberCell(asValue());
    }

    inline double AJValue::asDouble() const
    {
        return asNumberCell(asValue())->value();
    }

    inline bool AJValue::isNumber() const
    {
        return JSImmediate::isNumber(asValue()) || isDouble();
    }

    inline double AJValue::uncheckedGetNumber() const
    {
        ASSERT(isNumber());
        return JSImmediate::isImmediate(asValue()) ? JSImmediate::toDouble(asValue()) : asDouble();
    }

#endif // USE(JSVALUE32)

#if USE(JSVALUE64)
    ALWAYS_INLINE AJValue::AJValue(EncodeAsDoubleTag, ExecState*, double d)
    {
        *this = JSImmediate::fromNumberOutsideIntegerRange(d);
    }

    inline AJValue::AJValue(ExecState*, double d)
    {
        AJValue v = JSImmediate::from(d);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(ExecState*, int i)
    {
        AJValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(ExecState*, unsigned i)
    {
        AJValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(ExecState*, long i)
    {
        AJValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(ExecState*, unsigned long i)
    {
        AJValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(ExecState*, long long i)
    {
        AJValue v = JSImmediate::from(static_cast<double>(i));
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(ExecState*, unsigned long long i)
    {
        AJValue v = JSImmediate::from(static_cast<double>(i));
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(AJGlobalData*, double d)
    {
        AJValue v = JSImmediate::from(d);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(AJGlobalData*, int i)
    {
        AJValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline AJValue::AJValue(AJGlobalData*, unsigned i)
    {
        AJValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline bool AJValue::isDouble() const
    {
        return JSImmediate::isDouble(asValue());
    }

    inline double AJValue::asDouble() const
    {
        return JSImmediate::doubleValue(asValue());
    }

    inline bool AJValue::isNumber() const
    {
        return JSImmediate::isNumber(asValue());
    }

    inline double AJValue::uncheckedGetNumber() const
    {
        ASSERT(isNumber());
        return JSImmediate::toDouble(asValue());
    }

#endif // USE(JSVALUE64)

#if USE(JSVALUE32) || USE(JSVALUE64)

    inline AJValue::AJValue(ExecState*, char i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline AJValue::AJValue(ExecState*, unsigned char i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline AJValue::AJValue(ExecState*, short i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline AJValue::AJValue(ExecState*, unsigned short i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline AJValue jsNaN(ExecState* exec)
    {
        return jsNumber(exec, NaN);
    }

    inline AJValue jsNaN(AJGlobalData* globalData)
    {
        return jsNumber(globalData, NaN);
    }

    // --- AJValue inlines ----------------------------

    ALWAYS_INLINE AJValue AJValue::toJSNumber(ExecState* exec) const
    {
        return isNumber() ? asValue() : jsNumber(exec, this->toNumber(exec));
    }

    inline bool AJValue::getNumber(double &result) const
    {
        if (isInt32())
            result = asInt32();
        else if (LIKELY(isDouble()))
            result = asDouble();
        else {
            ASSERT(!isNumber());
            return false;
        }
        return true;
    }

#endif // USE(JSVALUE32) || USE(JSVALUE64)

} // namespace AJ

#endif // JSNumberCell_h
