
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
 *  Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef PropertyNameArray_h
#define PropertyNameArray_h

#include "CallFrame.h"
#include "Identifier.h"
#include <wtf/HashSet.h>
#include <wtf/OwnArrayPtr.h>
#include <wtf/Vector.h>

namespace AJ {
    
    class Structure;
    class StructureChain;

    // FIXME: Rename to PropertyNameArray.
    class PropertyNameArrayData : public RefCounted<PropertyNameArrayData> {
    public:
        typedef Vector<Identifier, 20> PropertyNameVector;

        static PassRefPtr<PropertyNameArrayData> create() { return adoptRef(new PropertyNameArrayData); }

        PropertyNameVector& propertyNameVector() { return m_propertyNameVector; }

    private:
        PropertyNameArrayData()
        {
        }

        PropertyNameVector m_propertyNameVector;
    };

    // FIXME: Rename to PropertyNameArrayBuilder.
    class PropertyNameArray {
    public:
        PropertyNameArray(AJGlobalData* globalData)
            : m_data(PropertyNameArrayData::create())
            , m_globalData(globalData)
            , m_shouldCache(true)
        {
        }

        PropertyNameArray(ExecState* exec)
            : m_data(PropertyNameArrayData::create())
            , m_globalData(&exec->globalData())
            , m_shouldCache(true)
        {
        }

        AJGlobalData* globalData() { return m_globalData; }

        void add(const Identifier& identifier) { add(identifier.ustring().rep()); }
        void add(UString::Rep*);
        void addKnownUnique(UString::Rep* identifier) { m_data->propertyNameVector().append(Identifier(m_globalData, identifier)); }

        Identifier& operator[](unsigned i) { return m_data->propertyNameVector()[i]; }
        const Identifier& operator[](unsigned i) const { return m_data->propertyNameVector()[i]; }

        void setData(PassRefPtr<PropertyNameArrayData> data) { m_data = data; }
        PropertyNameArrayData* data() { return m_data.get(); }
        PassRefPtr<PropertyNameArrayData> releaseData() { return m_data.release(); }

        // FIXME: Remove these functions.
        typedef PropertyNameArrayData::PropertyNameVector::const_iterator const_iterator;
        size_t size() const { return m_data->propertyNameVector().size(); }
        const_iterator begin() const { return m_data->propertyNameVector().begin(); }
        const_iterator end() const { return m_data->propertyNameVector().end(); }

    private:
        typedef HashSet<UString::Rep*, PtrHash<UString::Rep*> > IdentifierSet;

        RefPtr<PropertyNameArrayData> m_data;
        IdentifierSet m_set;
        AJGlobalData* m_globalData;
        bool m_shouldCache;
    };

} // namespace AJ

#endif // PropertyNameArray_h
