
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
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef AJString_h
#define AJString_h

#include "CallFrame.h"
#include "CommonIdentifiers.h"
#include "Identifier.h"
#include "JSNumberCell.h"
#include "PropertyDescriptor.h"
#include "PropertySlot.h"
#include "RopeImpl.h"

namespace AJ {

    class AJString;

    AJString* jsEmptyString(AJGlobalData*);
    AJString* jsEmptyString(ExecState*);
    AJString* jsString(AJGlobalData*, const UString&); // returns empty string if passed null string
    AJString* jsString(ExecState*, const UString&); // returns empty string if passed null string

    AJString* jsSingleCharacterString(AJGlobalData*, UChar);
    AJString* jsSingleCharacterString(ExecState*, UChar);
    AJString* jsSingleCharacterSubstring(ExecState*, const UString&, unsigned offset);
    AJString* jsSubstring(AJGlobalData*, const UString&, unsigned offset, unsigned length);
    AJString* jsSubstring(ExecState*, const UString&, unsigned offset, unsigned length);

    // Non-trivial strings are two or more characters long.
    // These functions are faster than just calling jsString.
    AJString* jsNontrivialString(AJGlobalData*, const UString&);
    AJString* jsNontrivialString(ExecState*, const UString&);
    AJString* jsNontrivialString(AJGlobalData*, const char*);
    AJString* jsNontrivialString(ExecState*, const char*);

    // Should be used for strings that are owned by an object that will
    // likely outlive the AJValue this makes, such as the parse tree or a
    // DOM object that contains a UString
    AJString* jsOwnedString(AJGlobalData*, const UString&); 
    AJString* jsOwnedString(ExecState*, const UString&); 

    typedef void (*AJStringFinalizerCallback)(AJString*, void* context);
    AJString* jsStringWithFinalizer(ExecState*, const UString&, AJStringFinalizerCallback callback, void* context);

    class JS_EXPORTCLASS AJString : public AJCell {
    public:
        friend class JIT;
        friend class AJGlobalData;
        friend class SpecializedThunkJIT;
        friend struct ThunkHelpers;

        class RopeBuilder {
        public:
            RopeBuilder(unsigned fiberCount)
                : m_index(0)
                , m_rope(RopeImpl::tryCreateUninitialized(fiberCount))
            {
            }

            bool isOutOfMemory() { return !m_rope; }

            void append(RopeImpl::Fiber& fiber)
            {
                ASSERT(m_rope);
                m_rope->initializeFiber(m_index, fiber);
            }
            void append(const UString& string)
            {
                ASSERT(m_rope);
                m_rope->initializeFiber(m_index, string.rep());
            }
            void append(AJString* jsString)
            {
                if (jsString->isRope()) {
                    for (unsigned i = 0; i < jsString->m_fiberCount; ++i)
                        append(jsString->m_other.m_fibers[i]);
                } else
                    append(jsString->string());
            }

            PassRefPtr<RopeImpl> release()
            {
                ASSERT(m_index == m_rope->fiberCount());
                return m_rope.release();
            }

            unsigned length() { return m_rope->length(); }

        private:
            unsigned m_index;
            RefPtr<RopeImpl> m_rope;
        };

        class RopeIterator {
            public:
                RopeIterator() { }

                RopeIterator(RopeImpl::Fiber* fibers, size_t fiberCount)
                {
                    ASSERT(fiberCount);
                    m_workQueue.append(WorkItem(fibers, fiberCount));
                    skipRopes();
                }

                RopeIterator& operator++()
                {
                    WorkItem& item = m_workQueue.last();
                    ASSERT(!RopeImpl::isRope(item.fibers[item.i]));
                    if (++item.i == item.fiberCount)
                        m_workQueue.removeLast();
                    skipRopes();
                    return *this;
                }

                UStringImpl* operator*()
                {
                    WorkItem& item = m_workQueue.last();
                    RopeImpl::Fiber fiber = item.fibers[item.i];
                    ASSERT(!RopeImpl::isRope(fiber));
                    return static_cast<UStringImpl*>(fiber);
                }

                bool operator!=(const RopeIterator& other) const
                {
                    return m_workQueue != other.m_workQueue;
                }

            private:
                struct WorkItem {
                    WorkItem(RopeImpl::Fiber* fibers, size_t fiberCount)
                        : fibers(fibers)
                        , fiberCount(fiberCount)
                        , i(0)
                    {
                    }

                    bool operator!=(const WorkItem& other) const
                    {
                        return fibers != other.fibers || fiberCount != other.fiberCount || i != other.i;
                    }

                    RopeImpl::Fiber* fibers;
                    size_t fiberCount;
                    size_t i;
                };

                void skipRopes()
                {
                    if (m_workQueue.isEmpty())
                        return;

                    while (1) {
                        WorkItem& item = m_workQueue.last();
                        RopeImpl::Fiber fiber = item.fibers[item.i];
                        if (!RopeImpl::isRope(fiber))
                            break;
                        RopeImpl* rope = static_cast<RopeImpl*>(fiber);
                        if (++item.i == item.fiberCount)
                            m_workQueue.removeLast();
                        m_workQueue.append(WorkItem(rope->fibers(), rope->fiberCount()));
                    }
                }

                Vector<WorkItem, 16> m_workQueue;
        };

        ALWAYS_INLINE AJString(AJGlobalData* globalData, const UString& value)
            : AJCell(globalData->stringStructure.get())
            , m_length(value.size())
            , m_value(value)
            , m_fiberCount(0)
        {
            ASSERT(!m_value.isNull());
            Heap::heap(this)->reportExtraMemoryCost(value.cost());
        }

        enum HasOtherOwnerType { HasOtherOwner };
        AJString(AJGlobalData* globalData, const UString& value, HasOtherOwnerType)
            : AJCell(globalData->stringStructure.get())
            , m_length(value.size())
            , m_value(value)
            , m_fiberCount(0)
        {
            ASSERT(!m_value.isNull());
        }
        AJString(AJGlobalData* globalData, PassRefPtr<UStringImpl> value, HasOtherOwnerType)
            : AJCell(globalData->stringStructure.get())
            , m_length(value->length())
            , m_value(value)
            , m_fiberCount(0)
        {
            ASSERT(!m_value.isNull());
        }
        AJString(AJGlobalData* globalData, PassRefPtr<RopeImpl> rope)
            : AJCell(globalData->stringStructure.get())
            , m_length(rope->length())
            , m_fiberCount(1)
        {
            m_other.m_fibers[0] = rope.releaseRef();
        }
        // This constructor constructs a new string by concatenating s1 & s2.
        // This should only be called with fiberCount <= 3.
        AJString(AJGlobalData* globalData, unsigned fiberCount, AJString* s1, AJString* s2)
            : AJCell(globalData->stringStructure.get())
            , m_length(s1->length() + s2->length())
            , m_fiberCount(fiberCount)
        {
            ASSERT(fiberCount <= s_maxInternalRopeLength);
            unsigned index = 0;
            appendStringInConstruct(index, s1);
            appendStringInConstruct(index, s2);
            ASSERT(fiberCount == index);
        }
        // This constructor constructs a new string by concatenating s1 & s2.
        // This should only be called with fiberCount <= 3.
        AJString(AJGlobalData* globalData, unsigned fiberCount, AJString* s1, const UString& u2)
            : AJCell(globalData->stringStructure.get())
            , m_length(s1->length() + u2.size())
            , m_fiberCount(fiberCount)
        {
            ASSERT(fiberCount <= s_maxInternalRopeLength);
            unsigned index = 0;
            appendStringInConstruct(index, s1);
            appendStringInConstruct(index, u2);
            ASSERT(fiberCount == index);
        }
        // This constructor constructs a new string by concatenating s1 & s2.
        // This should only be called with fiberCount <= 3.
        AJString(AJGlobalData* globalData, unsigned fiberCount, const UString& u1, AJString* s2)
            : AJCell(globalData->stringStructure.get())
            , m_length(u1.size() + s2->length())
            , m_fiberCount(fiberCount)
        {
            ASSERT(fiberCount <= s_maxInternalRopeLength);
            unsigned index = 0;
            appendStringInConstruct(index, u1);
            appendStringInConstruct(index, s2);
            ASSERT(fiberCount == index);
        }
        // This constructor constructs a new string by concatenating v1, v2 & v3.
        // This should only be called with fiberCount <= 3 ... which since every
        // value must require a fiberCount of at least one implies that the length
        // for each value must be exactly 1!
        AJString(ExecState* exec, AJValue v1, AJValue v2, AJValue v3)
            : AJCell(exec->globalData().stringStructure.get())
            , m_length(0)
            , m_fiberCount(s_maxInternalRopeLength)
        {
            unsigned index = 0;
            appendValueInConstructAndIncrementLength(exec, index, v1);
            appendValueInConstructAndIncrementLength(exec, index, v2);
            appendValueInConstructAndIncrementLength(exec, index, v3);
            ASSERT(index == s_maxInternalRopeLength);
        }

        // This constructor constructs a new string by concatenating u1 & u2.
        AJString(AJGlobalData* globalData, const UString& u1, const UString& u2)
            : AJCell(globalData->stringStructure.get())
            , m_length(u1.size() + u2.size())
            , m_fiberCount(2)
        {
            unsigned index = 0;
            appendStringInConstruct(index, u1);
            appendStringInConstruct(index, u2);
            ASSERT(index <= s_maxInternalRopeLength);
        }

        // This constructor constructs a new string by concatenating u1, u2 & u3.
        AJString(AJGlobalData* globalData, const UString& u1, const UString& u2, const UString& u3)
            : AJCell(globalData->stringStructure.get())
            , m_length(u1.size() + u2.size() + u3.size())
            , m_fiberCount(s_maxInternalRopeLength)
        {
            unsigned index = 0;
            appendStringInConstruct(index, u1);
            appendStringInConstruct(index, u2);
            appendStringInConstruct(index, u3);
            ASSERT(index <= s_maxInternalRopeLength);
        }

        AJString(AJGlobalData* globalData, const UString& value, AJStringFinalizerCallback finalizer, void* context)
            : AJCell(globalData->stringStructure.get())
            , m_length(value.size())
            , m_value(value)
            , m_fiberCount(0)
        {
            ASSERT(!m_value.isNull());
            // nasty hack because we can't union non-POD types
            m_other.m_finalizerCallback = finalizer;
            m_other.m_finalizerContext = context;
            Heap::heap(this)->reportExtraMemoryCost(value.cost());
        }

        ~AJString()
        {
            ASSERT(vptr() == AJGlobalData::jsStringVPtr);
            for (unsigned i = 0; i < m_fiberCount; ++i)
                RopeImpl::deref(m_other.m_fibers[i]);

            if (!m_fiberCount && m_other.m_finalizerCallback)
                m_other.m_finalizerCallback(this, m_other.m_finalizerContext);
        }

        const UString& value(ExecState* exec) const
        {
            if (isRope())
                resolveRope(exec);
            return m_value;
        }
        const UString& tryGetValue() const
        {
            if (isRope())
                resolveRope(0);
            return m_value;
        }
        unsigned length() { return m_length; }

        bool getStringPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        bool getStringPropertySlot(ExecState*, unsigned propertyName, PropertySlot&);
        bool getStringPropertyDescriptor(ExecState*, const Identifier& propertyName, PropertyDescriptor&);

        bool canGetIndex(unsigned i) { return i < m_length; }
        AJString* getIndex(ExecState*, unsigned);
        AJString* getIndexSlowCase(ExecState*, unsigned);

        AJValue replaceCharacter(ExecState*, UChar, const UString& replacement);

        static PassRefPtr<Structure> createStructure(AJValue proto) { return Structure::create(proto, TypeInfo(StringType, OverridesGetOwnPropertySlot | NeedsThisConversion), AnonymousSlotCount); }

    private:
        enum VPtrStealingHackType { VPtrStealingHack };
        AJString(VPtrStealingHackType) 
            : AJCell(0)
            , m_fiberCount(0)
        {
        }

        void resolveRope(ExecState*) const;
        AJString* substringFromRope(ExecState*, unsigned offset, unsigned length);

        void appendStringInConstruct(unsigned& index, const UString& string)
        {
            UStringImpl* impl = string.rep();
            impl->ref();
            m_other.m_fibers[index++] = impl;
        }

        void appendStringInConstruct(unsigned& index, AJString* jsString)
        {
            if (jsString->isRope()) {
                for (unsigned i = 0; i < jsString->m_fiberCount; ++i) {
                    RopeImpl::Fiber fiber = jsString->m_other.m_fibers[i];
                    fiber->ref();
                    m_other.m_fibers[index++] = fiber;
                }
            } else
                appendStringInConstruct(index, jsString->string());
        }

        void appendValueInConstructAndIncrementLength(ExecState* exec, unsigned& index, AJValue v)
        {
            if (v.isString()) {
                ASSERT(asCell(v)->isString());
                AJString* s = static_cast<AJString*>(asCell(v));
                ASSERT(s->size() == 1);
                appendStringInConstruct(index, s);
                m_length += s->length();
            } else {
                UString u(v.toString(exec));
                UStringImpl* impl = u.rep();
                impl->ref();
                m_other.m_fibers[index++] = impl;
                m_length += u.size();
            }
        }

        virtual AJValue toPrimitive(ExecState*, PreferredPrimitiveType) const;
        virtual bool getPrimitiveNumber(ExecState*, double& number, AJValue& value);
        virtual bool toBoolean(ExecState*) const;
        virtual double toNumber(ExecState*) const;
        virtual AJObject* toObject(ExecState*) const;
        virtual UString toString(ExecState*) const;

        virtual AJObject* toThisObject(ExecState*) const;

        // Actually getPropertySlot, not getOwnPropertySlot (see AJCell).
        virtual bool getOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        virtual bool getOwnPropertySlot(ExecState*, unsigned propertyName, PropertySlot&);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);

        static const unsigned s_maxInternalRopeLength = 3;

        // A string is represented either by a UString or a RopeImpl.
        unsigned m_length;
        mutable UString m_value;
        mutable unsigned m_fiberCount;
        // This structure exists to support a temporary workaround for a GC issue.
        struct AJStringFinalizerStruct {
            AJStringFinalizerStruct() : m_finalizerCallback(0) {}
            union {
                mutable RopeImpl::Fiber m_fibers[s_maxInternalRopeLength];
                struct {
                    AJStringFinalizerCallback m_finalizerCallback;
                    void* m_finalizerContext;
                };
            };
        } m_other;

        bool isRope() const { return m_fiberCount; }
        UString& string() { ASSERT(!isRope()); return m_value; }
        unsigned size() { return m_fiberCount ? m_fiberCount : 1; }

        friend AJValue jsString(ExecState* exec, AJString* s1, AJString* s2);
        friend AJValue jsString(ExecState* exec, const UString& u1, AJString* s2);
        friend AJValue jsString(ExecState* exec, AJString* s1, const UString& u2);
        friend AJValue jsString(ExecState* exec, Register* strings, unsigned count);
        friend AJValue jsString(ExecState* exec, AJValue thisValue, const ArgList& args);
        friend AJString* jsStringWithFinalizer(ExecState*, const UString&, AJStringFinalizerCallback callback, void* context);
        friend AJString* jsSubstring(ExecState* exec, AJString* s, unsigned offset, unsigned length);
    };

    AJString* asString(AJValue);

    // When an object is created from a different DLL, MSVC changes vptr to a "local" one right after invoking a constructor,
    // see <http://groups.google.com/group/microsoft.public.vc.language/msg/55cdcefeaf770212>.
    // This breaks isAJString(), and we don't need that hack anyway, so we change vptr back to primary one.
    // The below function must be called by any inline function that invokes a AJString constructor.
#if COMPILER(MSVC) && !defined(BUILDING_AJCore)
    inline AJString* fixupVPtr(AJGlobalData* globalData, AJString* string) { string->setVPtr(globalData->jsStringVPtr); return string; }
#else
    inline AJString* fixupVPtr(AJGlobalData*, AJString* string) { return string; }
#endif

    inline AJString* asString(AJValue value)
    {
        ASSERT(asCell(value)->isString());
        return static_cast<AJString*>(asCell(value));
    }

    inline AJString* jsEmptyString(AJGlobalData* globalData)
    {
        return globalData->smallStrings.emptyString(globalData);
    }

    inline AJString* jsSingleCharacterString(AJGlobalData* globalData, UChar c)
    {
        if (c <= 0xFF)
            return globalData->smallStrings.singleCharacterString(globalData, c);
        return fixupVPtr(globalData, new (globalData) AJString(globalData, UString(&c, 1)));
    }

    inline AJString* jsSingleCharacterSubstring(ExecState* exec, const UString& s, unsigned offset)
    {
        AJGlobalData* globalData = &exec->globalData();
        ASSERT(offset < static_cast<unsigned>(s.size()));
        UChar c = s.data()[offset];
        if (c <= 0xFF)
            return globalData->smallStrings.singleCharacterString(globalData, c);
        return fixupVPtr(globalData, new (globalData) AJString(globalData, UString(UStringImpl::create(s.rep(), offset, 1))));
    }

    inline AJString* jsNontrivialString(AJGlobalData* globalData, const char* s)
    {
        ASSERT(s);
        ASSERT(s[0]);
        ASSERT(s[1]);
        return fixupVPtr(globalData, new (globalData) AJString(globalData, s));
    }

    inline AJString* jsNontrivialString(AJGlobalData* globalData, const UString& s)
    {
        ASSERT(s.size() > 1);
        return fixupVPtr(globalData, new (globalData) AJString(globalData, s));
    }

    inline AJString* AJString::getIndex(ExecState* exec, unsigned i)
    {
        ASSERT(canGetIndex(i));
        if (isRope())
            return getIndexSlowCase(exec, i);
        ASSERT(i < m_value.size());
        return jsSingleCharacterSubstring(exec, m_value, i);
    }

    inline AJString* jsString(AJGlobalData* globalData, const UString& s)
    {
        int size = s.size();
        if (!size)
            return globalData->smallStrings.emptyString(globalData);
        if (size == 1) {
            UChar c = s.data()[0];
            if (c <= 0xFF)
                return globalData->smallStrings.singleCharacterString(globalData, c);
        }
        return fixupVPtr(globalData, new (globalData) AJString(globalData, s));
    }

    inline AJString* jsStringWithFinalizer(ExecState* exec, const UString& s, AJStringFinalizerCallback callback, void* context)
    {
        ASSERT(s.size() && (s.size() > 1 || s.data()[0] > 0xFF));
        AJGlobalData* globalData = &exec->globalData();
        return fixupVPtr(globalData, new (globalData) AJString(globalData, s, callback, context));
    }
    
    inline AJString* jsSubstring(ExecState* exec, AJString* s, unsigned offset, unsigned length)
    {
        ASSERT(offset <= static_cast<unsigned>(s->length()));
        ASSERT(length <= static_cast<unsigned>(s->length()));
        ASSERT(offset + length <= static_cast<unsigned>(s->length()));
        AJGlobalData* globalData = &exec->globalData();
        if (!length)
            return globalData->smallStrings.emptyString(globalData);
        if (s->isRope())
            return s->substringFromRope(exec, offset, length);
        return jsSubstring(globalData, s->m_value, offset, length);
    }

    inline AJString* jsSubstring(AJGlobalData* globalData, const UString& s, unsigned offset, unsigned length)
    {
        ASSERT(offset <= static_cast<unsigned>(s.size()));
        ASSERT(length <= static_cast<unsigned>(s.size()));
        ASSERT(offset + length <= static_cast<unsigned>(s.size()));
        if (!length)
            return globalData->smallStrings.emptyString(globalData);
        if (length == 1) {
            UChar c = s.data()[offset];
            if (c <= 0xFF)
                return globalData->smallStrings.singleCharacterString(globalData, c);
        }
        return fixupVPtr(globalData, new (globalData) AJString(globalData, UString(UStringImpl::create(s.rep(), offset, length)), AJString::HasOtherOwner));
    }

    inline AJString* jsOwnedString(AJGlobalData* globalData, const UString& s)
    {
        int size = s.size();
        if (!size)
            return globalData->smallStrings.emptyString(globalData);
        if (size == 1) {
            UChar c = s.data()[0];
            if (c <= 0xFF)
                return globalData->smallStrings.singleCharacterString(globalData, c);
        }
        return fixupVPtr(globalData, new (globalData) AJString(globalData, s, AJString::HasOtherOwner));
    }

    inline AJString* jsEmptyString(ExecState* exec) { return jsEmptyString(&exec->globalData()); }
    inline AJString* jsString(ExecState* exec, const UString& s) { return jsString(&exec->globalData(), s); }
    inline AJString* jsSingleCharacterString(ExecState* exec, UChar c) { return jsSingleCharacterString(&exec->globalData(), c); }
    inline AJString* jsSubstring(ExecState* exec, const UString& s, unsigned offset, unsigned length) { return jsSubstring(&exec->globalData(), s, offset, length); }
    inline AJString* jsNontrivialString(ExecState* exec, const UString& s) { return jsNontrivialString(&exec->globalData(), s); }
    inline AJString* jsNontrivialString(ExecState* exec, const char* s) { return jsNontrivialString(&exec->globalData(), s); }
    inline AJString* jsOwnedString(ExecState* exec, const UString& s) { return jsOwnedString(&exec->globalData(), s); } 

    ALWAYS_INLINE bool AJString::getStringPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
    {
        if (propertyName == exec->propertyNames().length) {
            slot.setValue(jsNumber(exec, m_length));
            return true;
        }

        bool isStrictUInt32;
        unsigned i = propertyName.toStrictUInt32(&isStrictUInt32);
        if (isStrictUInt32 && i < m_length) {
            slot.setValue(getIndex(exec, i));
            return true;
        }

        return false;
    }
        
    ALWAYS_INLINE bool AJString::getStringPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
    {
        if (propertyName < m_length) {
            slot.setValue(getIndex(exec, propertyName));
            return true;
        }

        return false;
    }

    inline bool isAJString(AJGlobalData* globalData, AJValue v) { return v.isCell() && v.asCell()->vptr() == globalData->jsStringVPtr; }

    // --- AJValue inlines ----------------------------

    inline UString AJValue::toString(ExecState* exec) const
    {
        if (isString())
            return static_cast<AJString*>(asCell())->value(exec);
        if (isInt32())
            return exec->globalData().numericStrings.add(asInt32());
        if (isDouble())
            return exec->globalData().numericStrings.add(asDouble());
        if (isTrue())
            return "true";
        if (isFalse())
            return "false";
        if (isNull())
            return "null";
        if (isUndefined())
            return "undefined";
        ASSERT(isCell());
        return asCell()->toString(exec);
    }

    inline UString AJValue::toPrimitiveString(ExecState* exec) const
    {
        if (isString())
            return static_cast<AJString*>(asCell())->value(exec);
        if (isInt32())
            return exec->globalData().numericStrings.add(asInt32());
        if (isDouble())
            return exec->globalData().numericStrings.add(asDouble());
        if (isTrue())
            return "true";
        if (isFalse())
            return "false";
        if (isNull())
            return "null";
        if (isUndefined())
            return "undefined";
        ASSERT(isCell());
        return asCell()->toPrimitive(exec, NoPreference).toString(exec);
    }

} // namespace AJ

#endif // AJString_h
