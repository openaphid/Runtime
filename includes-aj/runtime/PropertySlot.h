
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
 *  Copyright (C) 2005, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef PropertySlot_h
#define PropertySlot_h

#include "Identifier.h"
#include "AJValue.h"
#include "Register.h"
#include <wtf/Assertions.h>
#include <wtf/NotFound.h>

namespace AJ {

    class ExecState;
    class AJObject;

#define JSC_VALUE_SLOT_MARKER 0
#define JSC_REGISTER_SLOT_MARKER reinterpret_cast<GetValueFunc>(1)
#define INDEX_GETTER_MARKER reinterpret_cast<GetValueFunc>(2)
#define GETTER_FUNCTION_MARKER reinterpret_cast<GetValueFunc>(3)

    class PropertySlot {
    public:
        enum CachedPropertyType {
            Uncacheable,
            Getter,
            Custom,
            Value
        };

        PropertySlot()
            : m_cachedPropertyType(Uncacheable)
        {
            clearBase();
            clearOffset();
            clearValue();
        }

        explicit PropertySlot(const AJValue base)
            : m_slotBase(base)
            , m_cachedPropertyType(Uncacheable)
        {
            clearOffset();
            clearValue();
        }

        typedef AJValue (*GetValueFunc)(ExecState*, AJValue slotBase, const Identifier&);
        typedef AJValue (*GetIndexValueFunc)(ExecState*, AJValue slotBase, unsigned);

        AJValue getValue(ExecState* exec, const Identifier& propertyName) const
        {
            if (m_getValue == JSC_VALUE_SLOT_MARKER)
                return *m_data.valueSlot;
            if (m_getValue == JSC_REGISTER_SLOT_MARKER)
                return (*m_data.registerSlot).jsValue();
            if (m_getValue == INDEX_GETTER_MARKER)
                return m_getIndexValue(exec, slotBase(), index());
            if (m_getValue == GETTER_FUNCTION_MARKER)
                return functionGetter(exec);
            return m_getValue(exec, slotBase(), propertyName);
        }

        AJValue getValue(ExecState* exec, unsigned propertyName) const
        {
            if (m_getValue == JSC_VALUE_SLOT_MARKER)
                return *m_data.valueSlot;
            if (m_getValue == JSC_REGISTER_SLOT_MARKER)
                return (*m_data.registerSlot).jsValue();
            if (m_getValue == INDEX_GETTER_MARKER)
                return m_getIndexValue(exec, m_slotBase, m_data.index);
            if (m_getValue == GETTER_FUNCTION_MARKER)
                return functionGetter(exec);
            return m_getValue(exec, slotBase(), Identifier::from(exec, propertyName));
        }

        CachedPropertyType cachedPropertyType() const { return m_cachedPropertyType; }
        bool isCacheable() const { return m_cachedPropertyType != Uncacheable; }
        bool isCacheableValue() const { return m_cachedPropertyType == Value; }
        size_t cachedOffset() const
        {
            ASSERT(isCacheable());
            return m_offset;
        }

        void setValueSlot(AJValue* valueSlot) 
        {
            ASSERT(valueSlot);
            clearBase();
            clearOffset();
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_data.valueSlot = valueSlot;
        }
        
        void setValueSlot(AJValue slotBase, AJValue* valueSlot)
        {
            ASSERT(valueSlot);
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_slotBase = slotBase;
            m_data.valueSlot = valueSlot;
        }
        
        void setValueSlot(AJValue slotBase, AJValue* valueSlot, size_t offset)
        {
            ASSERT(valueSlot);
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_slotBase = slotBase;
            m_data.valueSlot = valueSlot;
            m_offset = offset;
            m_cachedPropertyType = Value;
        }
        
        void setValue(AJValue value)
        {
            ASSERT(value);
            clearBase();
            clearOffset();
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_value = value;
            m_data.valueSlot = &m_value;
        }

        void setRegisterSlot(Register* registerSlot)
        {
            ASSERT(registerSlot);
            clearBase();
            clearOffset();
            m_getValue = JSC_REGISTER_SLOT_MARKER;
            m_data.registerSlot = registerSlot;
        }

        void setCustom(AJValue slotBase, GetValueFunc getValue)
        {
            ASSERT(slotBase);
            ASSERT(getValue);
            m_getValue = getValue;
            m_getIndexValue = 0;
            m_slotBase = slotBase;
        }
        
        void setCacheableCustom(AJValue slotBase, GetValueFunc getValue)
        {
            ASSERT(slotBase);
            ASSERT(getValue);
            m_getValue = getValue;
            m_getIndexValue = 0;
            m_slotBase = slotBase;
            m_cachedPropertyType = Custom;
        }

        void setCustomIndex(AJValue slotBase, unsigned index, GetIndexValueFunc getIndexValue)
        {
            ASSERT(slotBase);
            ASSERT(getIndexValue);
            m_getValue = INDEX_GETTER_MARKER;
            m_getIndexValue = getIndexValue;
            m_slotBase = slotBase;
            m_data.index = index;
        }

        void setGetterSlot(AJObject* getterFunc)
        {
            ASSERT(getterFunc);
            m_thisValue = m_slotBase;
            m_getValue = GETTER_FUNCTION_MARKER;
            m_data.getterFunc = getterFunc;
        }

        void setCacheableGetterSlot(AJValue slotBase, AJObject* getterFunc, unsigned offset)
        {
            ASSERT(getterFunc);
            m_getValue = GETTER_FUNCTION_MARKER;
            m_thisValue = m_slotBase;
            m_slotBase = slotBase;
            m_data.getterFunc = getterFunc;
            m_offset = offset;
            m_cachedPropertyType = Getter;
        }

        void setUndefined()
        {
            setValue(jsUndefined());
        }

        AJValue slotBase() const
        {
            return m_slotBase;
        }

        void setBase(AJValue base)
        {
            ASSERT(m_slotBase);
            ASSERT(base);
            m_slotBase = base;
        }

        void clearBase()
        {
#ifndef NDEBUG
            m_slotBase = AJValue();
#endif
        }

        void clearValue()
        {
#ifndef NDEBUG
            m_value = AJValue();
#endif
        }

        void clearOffset()
        {
            // Clear offset even in release builds, in case this PropertySlot has been used before.
            // (For other data members, we don't need to clear anything because reuse would meaningfully overwrite them.)
            m_offset = 0;
            m_cachedPropertyType = Uncacheable;
        }

        unsigned index() const { return m_data.index; }

        AJValue thisValue() const { return m_thisValue; }

        GetValueFunc customGetter() const
        {
            ASSERT(m_cachedPropertyType == Custom);
            return m_getValue;
        }
    private:
        AJValue functionGetter(ExecState*) const;

        GetValueFunc m_getValue;
        GetIndexValueFunc m_getIndexValue;
        
        AJValue m_slotBase;
        union {
            AJObject* getterFunc;
            AJValue* valueSlot;
            Register* registerSlot;
            unsigned index;
        } m_data;

        AJValue m_value;
        AJValue m_thisValue;

        size_t m_offset;
        CachedPropertyType m_cachedPropertyType;
    };

} // namespace AJ

#endif // PropertySlot_h
