
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
 *  Copyright (C) 2006 Maks Orlovich
 *  Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef JSWrapperObject_h
#define JSWrapperObject_h

#include "AJObject.h"

namespace AJ {

    // This class is used as a base for classes such as String,
    // Number, Boolean and Date which are wrappers for primitive types.
    class JSWrapperObject : public AJObject {
    protected:
        explicit JSWrapperObject(NonNullPassRefPtr<Structure>);

    public:
        AJValue internalValue() const { return m_internalValue; }
        void setInternalValue(AJValue);

        static PassRefPtr<Structure> createStructure(AJValue prototype) 
        { 
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount);
        }

    protected:
        static const unsigned AnonymousSlotCount = 1 + AJObject::AnonymousSlotCount;

    private:
        virtual void markChildren(MarkStack&);
        
        AJValue m_internalValue;
    };

    inline JSWrapperObject::JSWrapperObject(NonNullPassRefPtr<Structure> structure)
        : AJObject(structure)
    {
        putAnonymousValue(0, jsNull());
    }

    inline void JSWrapperObject::setInternalValue(AJValue value)
    {
        ASSERT(value);
        ASSERT(!value.isObject());
        m_internalValue = value;
        putAnonymousValue(0, value);
    }

} // namespace AJ

#endif // JSWrapperObject_h
