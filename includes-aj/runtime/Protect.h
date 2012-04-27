
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
 *  Copyright (C) 2004, 2008, 2009 Apple Inc. All rights reserved.
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


#ifndef Protect_h
#define Protect_h

#include "Collector.h"
#include "AJValue.h"

namespace AJ {

    inline void gcProtect(AJCell* val) 
    {
        Heap::heap(val)->protect(val);
    }

    inline void gcUnprotect(AJCell* val)
    {
        Heap::heap(val)->unprotect(val);
    }

    inline void gcProtectNullTolerant(AJCell* val) 
    {
        if (val) 
            gcProtect(val);
    }

    inline void gcUnprotectNullTolerant(AJCell* val) 
    {
        if (val) 
            gcUnprotect(val);
    }
    
    inline void gcProtect(AJValue value)
    {
        if (value && value.isCell())
            gcProtect(asCell(value));
    }

    inline void gcUnprotect(AJValue value)
    {
        if (value && value.isCell())
            gcUnprotect(asCell(value));
    }

    // FIXME: Share more code with RefPtr template? The only differences are the ref/deref operation
    // and the implicit conversion to raw pointer
    template <class T> class ProtectedPtr {
    public:
        ProtectedPtr() : m_ptr(0) {}
        ProtectedPtr(T* ptr);
        ProtectedPtr(const ProtectedPtr&);
        ~ProtectedPtr();

        template <class U> ProtectedPtr(const ProtectedPtr<U>&);
        
        T* get() const { return m_ptr; }
        operator T*() const { return m_ptr; }
        operator AJValue() const { return AJValue(m_ptr); }
        T* operator->() const { return m_ptr; }
        
        operator bool() const { return m_ptr; }
        bool operator!() const { return !m_ptr; }

        ProtectedPtr& operator=(const ProtectedPtr&);
        ProtectedPtr& operator=(T*);
        
    private:
        T* m_ptr;
    };

    class ProtectedAJValue {
    public:
        ProtectedAJValue() {}
        ProtectedAJValue(AJValue value);
        ProtectedAJValue(const ProtectedAJValue&);
        ~ProtectedAJValue();

        template <class U> ProtectedAJValue(const ProtectedPtr<U>&);
        
        AJValue get() const { return m_value; }
        operator AJValue() const { return m_value; }
        AJValue operator->() const { return m_value; }
        
        operator bool() const { return m_value; }
        bool operator!() const { return !m_value; }

        ProtectedAJValue& operator=(const ProtectedAJValue&);
        ProtectedAJValue& operator=(AJValue);
        
    private:
        AJValue m_value;
    };

    template <class T> inline ProtectedPtr<T>::ProtectedPtr(T* ptr)
        : m_ptr(ptr)
    {
        gcProtectNullTolerant(m_ptr);
    }

    template <class T> inline ProtectedPtr<T>::ProtectedPtr(const ProtectedPtr& o)
        : m_ptr(o.get())
    {
        gcProtectNullTolerant(m_ptr);
    }

    template <class T> inline ProtectedPtr<T>::~ProtectedPtr()
    {
        gcUnprotectNullTolerant(m_ptr);
    }

    template <class T> template <class U> inline ProtectedPtr<T>::ProtectedPtr(const ProtectedPtr<U>& o)
        : m_ptr(o.get())
    {
        gcProtectNullTolerant(m_ptr);
    }

    template <class T> inline ProtectedPtr<T>& ProtectedPtr<T>::operator=(const ProtectedPtr<T>& o) 
    {
        T* optr = o.m_ptr;
        gcProtectNullTolerant(optr);
        gcUnprotectNullTolerant(m_ptr);
        m_ptr = optr;
        return *this;
    }

    template <class T> inline ProtectedPtr<T>& ProtectedPtr<T>::operator=(T* optr)
    {
        gcProtectNullTolerant(optr);
        gcUnprotectNullTolerant(m_ptr);
        m_ptr = optr;
        return *this;
    }

    inline ProtectedAJValue::ProtectedAJValue(AJValue value)
        : m_value(value)
    {
        gcProtect(m_value);
    }

    inline ProtectedAJValue::ProtectedAJValue(const ProtectedAJValue& o)
        : m_value(o.get())
    {
        gcProtect(m_value);
    }

    inline ProtectedAJValue::~ProtectedAJValue()
    {
        gcUnprotect(m_value);
    }

    template <class U> ProtectedAJValue::ProtectedAJValue(const ProtectedPtr<U>& o)
        : m_value(o.get())
    {
        gcProtect(m_value);
    }

    inline ProtectedAJValue& ProtectedAJValue::operator=(const ProtectedAJValue& o) 
    {
        AJValue ovalue = o.m_value;
        gcProtect(ovalue);
        gcUnprotect(m_value);
        m_value = ovalue;
        return *this;
    }

    inline ProtectedAJValue& ProtectedAJValue::operator=(AJValue ovalue)
    {
        gcProtect(ovalue);
        gcUnprotect(m_value);
        m_value = ovalue;
        return *this;
    }

    template <class T> inline bool operator==(const ProtectedPtr<T>& a, const ProtectedPtr<T>& b) { return a.get() == b.get(); }
    template <class T> inline bool operator==(const ProtectedPtr<T>& a, const T* b) { return a.get() == b; }
    template <class T> inline bool operator==(const T* a, const ProtectedPtr<T>& b) { return a == b.get(); }

    template <class T> inline bool operator!=(const ProtectedPtr<T>& a, const ProtectedPtr<T>& b) { return a.get() != b.get(); }
    template <class T> inline bool operator!=(const ProtectedPtr<T>& a, const T* b) { return a.get() != b; }
    template <class T> inline bool operator!=(const T* a, const ProtectedPtr<T>& b) { return a != b.get(); }

    inline bool operator==(const ProtectedAJValue& a, const ProtectedAJValue& b) { return a.get() == b.get(); }
    inline bool operator==(const ProtectedAJValue& a, const AJValue b) { return a.get() == b; }
    template <class T> inline bool operator==(const ProtectedAJValue& a, const ProtectedPtr<T>& b) { return a.get() == AJValue(b.get()); }
    inline bool operator==(const AJValue a, const ProtectedAJValue& b) { return a == b.get(); }
    template <class T> inline bool operator==(const ProtectedPtr<T>& a, const ProtectedAJValue& b) { return AJValue(a.get()) == b.get(); }

    inline bool operator!=(const ProtectedAJValue& a, const ProtectedAJValue& b) { return a.get() != b.get(); }
    inline bool operator!=(const ProtectedAJValue& a, const AJValue b) { return a.get() != b; }
    template <class T> inline bool operator!=(const ProtectedAJValue& a, const ProtectedPtr<T>& b) { return a.get() != AJValue(b.get()); }
    inline bool operator!=(const AJValue a, const ProtectedAJValue& b) { return a != b.get(); }
    template <class T> inline bool operator!=(const ProtectedPtr<T>& a, const ProtectedAJValue& b) { return AJValue(a.get()) != b.get(); }
 
} // namespace AJ

#endif // Protect_h
