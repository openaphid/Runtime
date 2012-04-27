
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
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef PropertyMapHashTable_h
#define PropertyMapHashTable_h

#include "UString.h"
#include <wtf/Vector.h>

namespace AJ {

    struct PropertyMapEntry {
        UString::Rep* key;
        unsigned offset;
        unsigned attributes;
        AJCell* specificValue;
        unsigned index;

        PropertyMapEntry(UString::Rep* key, unsigned attributes, AJCell* specificValue)
            : key(key)
            , offset(0)
            , attributes(attributes)
            , specificValue(specificValue)
            , index(0)
        {
        }

        PropertyMapEntry(UString::Rep* key, unsigned offset, unsigned attributes, AJCell* specificValue, unsigned index)
            : key(key)
            , offset(offset)
            , attributes(attributes)
            , specificValue(specificValue)
            , index(index)
        {
        }
    };

    // lastIndexUsed is an ever-increasing index used to identify the order items
    // were inserted into the property map. It's required that getEnumerablePropertyNames
    // return the properties in the order they were added for compatibility with other
    // browsers' AJ implementations.
    struct PropertyMapHashTable {
        unsigned sizeMask;
        unsigned size;
        unsigned keyCount;
        unsigned deletedSentinelCount;
        unsigned lastIndexUsed;
        Vector<unsigned>* deletedOffsets;
        unsigned entryIndices[1];

        PropertyMapEntry* entries()
        {
            // The entries vector comes after the indices vector.
            // The 0th item in the entries vector is not really used; it has to
            // have a 0 in its key to allow the hash table lookup to handle deleted
            // sentinels without any special-case code, but the other fields are unused.
            return reinterpret_cast<PropertyMapEntry*>(&entryIndices[size]);
        }

        static size_t allocationSize(unsigned size)
        {
            // We never let a hash table get more than half full,
            // So the number of indices we need is the size of the hash table.
            // But the number of entries is half that (plus one for the deleted sentinel).
            return sizeof(PropertyMapHashTable)
                + (size - 1) * sizeof(unsigned)
                + (1 + size / 2) * sizeof(PropertyMapEntry);
        }
    };

} // namespace AJ

#endif // PropertyMapHashTable_h
