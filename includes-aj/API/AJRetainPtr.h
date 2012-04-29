
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
 * Copyright (C) 2005, 2006, 2007 Apple Inc. All rights reserved.
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

#ifndef AJRetainPtr_h
#define AJRetainPtr_h

#include <AJCore/AJStringRef.h>
#include <algorithm>

inline void AJRetain(AJStringRef string) { AJStringRetain(string); }
inline void AJRelease(AJStringRef string) { AJStringRelease(string); }

enum AdoptTag { Adopt };

template <typename T> class AJRetainPtr {
public:
    AJRetainPtr() : m_ptr(0) {}
    AJRetainPtr(T ptr) : m_ptr(ptr) { if (ptr) AJRetain(ptr); }

    AJRetainPtr(AdoptTag, T ptr) : m_ptr(ptr) { }
    
    AJRetainPtr(const AJRetainPtr& o) : m_ptr(o.m_ptr) { if (T ptr = m_ptr) AJRetain(ptr); }

    ~AJRetainPtr() { if (T ptr = m_ptr) AJRelease(ptr); }
    
    template <typename U> AJRetainPtr(const AJRetainPtr<U>& o) : m_ptr(o.get()) { if (T ptr = m_ptr) AJRetain(ptr); }
    
    T get() const { return m_ptr; }
    
    T releaseRef() { T tmp = m_ptr; m_ptr = 0; return tmp; }
    
    T operator->() const { return m_ptr; }
    
    bool operator!() const { return !m_ptr; }

    // This conversion operator allows implicit conversion to bool but not to other integer types.
    typedef T AJRetainPtr::*UnspecifiedBoolType;
    operator UnspecifiedBoolType() const { return m_ptr ? &AJRetainPtr::m_ptr : 0; }
    
    AJRetainPtr& operator=(const AJRetainPtr&);
    template <typename U> AJRetainPtr& operator=(const AJRetainPtr<U>&);
    AJRetainPtr& operator=(T);
    template <typename U> AJRetainPtr& operator=(U*);

    void adopt(T);
    
    void swap(AJRetainPtr&);

private:
    T m_ptr;
};

template <typename T> inline AJRetainPtr<T>& AJRetainPtr<T>::operator=(const AJRetainPtr<T>& o)
{
    T optr = o.get();
    if (optr)
        AJRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        AJRelease(ptr);
    return *this;
}

template <typename T> template <typename U> inline AJRetainPtr<T>& AJRetainPtr<T>::operator=(const AJRetainPtr<U>& o)
{
    T optr = o.get();
    if (optr)
        AJRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        AJRelease(ptr);
    return *this;
}

template <typename T> inline AJRetainPtr<T>& AJRetainPtr<T>::operator=(T optr)
{
    if (optr)
        AJRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        AJRelease(ptr);
    return *this;
}

template <typename T> inline void AJRetainPtr<T>::adopt(T optr)
{
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        AJRelease(ptr);
}

template <typename T> template <typename U> inline AJRetainPtr<T>& AJRetainPtr<T>::operator=(U* optr)
{
    if (optr)
        AJRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        AJRelease(ptr);
    return *this;
}

template <class T> inline void AJRetainPtr<T>::swap(AJRetainPtr<T>& o)
{
    std::swap(m_ptr, o.m_ptr);
}

template <class T> inline void swap(AJRetainPtr<T>& a, AJRetainPtr<T>& b)
{
    a.swap(b);
}

template <typename T, typename U> inline bool operator==(const AJRetainPtr<T>& a, const AJRetainPtr<U>& b)
{ 
    return a.get() == b.get(); 
}

template <typename T, typename U> inline bool operator==(const AJRetainPtr<T>& a, U* b)
{ 
    return a.get() == b; 
}

template <typename T, typename U> inline bool operator==(T* a, const AJRetainPtr<U>& b) 
{
    return a == b.get(); 
}

template <typename T, typename U> inline bool operator!=(const AJRetainPtr<T>& a, const AJRetainPtr<U>& b)
{ 
    return a.get() != b.get(); 
}

template <typename T, typename U> inline bool operator!=(const AJRetainPtr<T>& a, U* b)
{
    return a.get() != b; 
}

template <typename T, typename U> inline bool operator!=(T* a, const AJRetainPtr<U>& b)
{ 
    return a != b.get(); 
}


#endif // AJRetainPtr_h
