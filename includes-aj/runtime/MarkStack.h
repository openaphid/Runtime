
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

#ifndef MarkStack_h
#define MarkStack_h

#include "AJValue.h"
#include <wtf/Noncopyable.h>

namespace AJ {

    class AJGlobalData;
    class Register;
    
    enum MarkSetProperties { MayContainNullValues, NoNullValues };
    
    class MarkStack : Noncopyable {
    public:
        MarkStack(void* jsArrayVPtr)
            : m_jsArrayVPtr(jsArrayVPtr)
#ifndef NDEBUG
            , m_isCheckingForDefaultMarkViolation(false)
#endif
        {
        }

        ALWAYS_INLINE void append(AJValue);
        void append(AJCell*);
        
        ALWAYS_INLINE void appendValues(Register* values, size_t count, MarkSetProperties properties = NoNullValues)
        {
            appendValues(reinterpret_cast<AJValue*>(values), count, properties);
        }

        ALWAYS_INLINE void appendValues(AJValue* values, size_t count, MarkSetProperties properties = NoNullValues)
        {
            if (count)
                m_markSets.append(MarkSet(values, values + count, properties));
        }

        inline void drain();
        void compact();

        ~MarkStack()
        {
            ASSERT(m_markSets.isEmpty());
            ASSERT(m_values.isEmpty());
        }

    private:
        void markChildren(AJCell*);

        struct MarkSet {
            MarkSet(AJValue* values, AJValue* end, MarkSetProperties properties)
                : m_values(values)
                , m_end(end)
                , m_properties(properties)
            {
                ASSERT(values);
            }
            AJValue* m_values;
            AJValue* m_end;
            MarkSetProperties m_properties;
        };

        static void* allocateStack(size_t size);
        static void releaseStack(void* addr, size_t size);

        static void initializePagesize();
        static size_t pageSize()
        {
            if (!s_pageSize)
                initializePagesize();
            return s_pageSize;
        }

        template <typename T> struct MarkStackArray {
            MarkStackArray()
                : m_top(0)
                , m_allocated(MarkStack::pageSize())
                , m_capacity(m_allocated / sizeof(T))
            {
                m_data = reinterpret_cast<T*>(allocateStack(m_allocated));
            }

            ~MarkStackArray()
            {
                releaseStack(m_data, m_allocated);
            }

            void expand()
            {
                size_t oldAllocation = m_allocated;
                m_allocated *= 2;
                m_capacity = m_allocated / sizeof(T);
                void* newData = allocateStack(m_allocated);
                memcpy(newData, m_data, oldAllocation);
                releaseStack(m_data, oldAllocation);
                m_data = reinterpret_cast<T*>(newData);
            }

            inline void append(const T& v)
            {
                if (m_top == m_capacity)
                    expand();
                m_data[m_top++] = v;
            }

            inline T removeLast()
            {
                ASSERT(m_top);
                return m_data[--m_top];
            }
            
            inline T& last()
            {
                ASSERT(m_top);
                return m_data[m_top - 1];
            }

            inline bool isEmpty()
            {
                return m_top == 0;
            }

            inline size_t size() { return m_top; }

            inline void shrinkAllocation(size_t size)
            {
                ASSERT(size <= m_allocated);
                ASSERT(0 == (size % MarkStack::pageSize()));
                if (size == m_allocated)
                    return;
#if OS(WINDOWS) || OS(SYMBIAN) || PLATFORM(BREWMP)
                // We cannot release a part of a region with VirtualFree.  To get around this,
                // we'll release the entire region and reallocate the size that we want.
                releaseStack(m_data, m_allocated);
                m_data = reinterpret_cast<T*>(allocateStack(size));
#else
                releaseStack(reinterpret_cast<char*>(m_data) + size, m_allocated - size);
#endif
                m_allocated = size;
                m_capacity = m_allocated / sizeof(T);
            }

        private:
            size_t m_top;
            size_t m_allocated;
            size_t m_capacity;
            T* m_data;
        };

        void* m_jsArrayVPtr;
        MarkStackArray<MarkSet> m_markSets;
        MarkStackArray<AJCell*> m_values;
        static size_t s_pageSize;

#ifndef NDEBUG
    public:
        bool m_isCheckingForDefaultMarkViolation;
#endif
    };
}

#endif
