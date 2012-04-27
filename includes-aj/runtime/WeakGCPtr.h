
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WeakGCPtr_h
#define WeakGCPtr_h

#include "Collector.h"
#include <wtf/Noncopyable.h>

namespace AJ {

// A smart pointer whose get() function returns 0 for cells awaiting destruction.
template <typename T> class WeakGCPtr : Noncopyable {
public:
    WeakGCPtr() : m_ptr(0) { }
    WeakGCPtr(T* ptr) { assign(ptr); }

    T* get() const
    {
        if (!m_ptr || !Heap::isCellMarked(m_ptr))
            return 0;
        return m_ptr;
    }
	
	T* forceGet() const {return m_ptr;} //OpenAphid
    
    bool clear(AJCell* ptr)
    {
        if (ptr == m_ptr) {
            m_ptr = 0;
            return true;
        }
        return false;
    }

    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }
    
    bool operator!() const { return !get(); }

    // This conversion operator allows implicit conversion to bool but not to other integer types.
#if COMPILER(WINSCW)
    operator bool() const { return m_ptr; }
#else
    typedef T* WeakGCPtr::*UnspecifiedBoolType;
    operator UnspecifiedBoolType() const { return get() ? &WeakGCPtr::m_ptr : 0; }
#endif

    WeakGCPtr& operator=(T*);

#if !ASSERT_DISABLED
    bool hasDeadObject() const { return !!m_ptr; }
#endif

private:
    void assign(T* ptr)
    {
        ASSERT(ptr);
        Heap::markCell(ptr);
        m_ptr = ptr;
    }

    T* m_ptr;
};

template <typename T> inline WeakGCPtr<T>& WeakGCPtr<T>::operator=(T* optr)
{
    assign(optr);
    return *this;
}

template <typename T, typename U> inline bool operator==(const WeakGCPtr<T>& a, const WeakGCPtr<U>& b)
{ 
    return a.get() == b.get(); 
}

template <typename T, typename U> inline bool operator==(const WeakGCPtr<T>& a, U* b)
{ 
    return a.get() == b; 
}

template <typename T, typename U> inline bool operator==(T* a, const WeakGCPtr<U>& b) 
{
    return a == b.get(); 
}

template <typename T, typename U> inline bool operator!=(const WeakGCPtr<T>& a, const WeakGCPtr<U>& b)
{ 
    return a.get() != b.get(); 
}

template <typename T, typename U> inline bool operator!=(const WeakGCPtr<T>& a, U* b)
{
    return a.get() != b; 
}

template <typename T, typename U> inline bool operator!=(T* a, const WeakGCPtr<U>& b)
{ 
    return a != b.get(); 
}

template <typename T, typename U> inline WeakGCPtr<T> static_pointer_cast(const WeakGCPtr<U>& p)
{ 
    return WeakGCPtr<T>(static_cast<T*>(p.get())); 
}

template <typename T, typename U> inline WeakGCPtr<T> const_pointer_cast(const WeakGCPtr<U>& p)
{ 
    return WeakGCPtr<T>(const_cast<T*>(p.get())); 
}

template <typename T> inline T* getPtr(const WeakGCPtr<T>& p)
{
    return p.get();
}

} // namespace AJ

#endif // WeakGCPtr_h
