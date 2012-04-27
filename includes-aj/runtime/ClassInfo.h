
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

#ifndef ClassInfo_h
#define ClassInfo_h

#include "CallFrame.h"

namespace AJ {

    class HashEntry;
    struct HashTable;

    struct ClassInfo {
        /**
         * A string denoting the class name. Example: "Window".
         */
        const char* className;

        /**
         * Pointer to the class information of the base class.
         * 0L if there is none.
         */
        const ClassInfo* parentClass;
        /**
         * Static hash-table of properties.
         * For classes that can be used from multiple threads, it is accessed via a getter function that would typically return a pointer to thread-specific value.
         */
        const HashTable* propHashTable(ExecState* exec) const
        {
            if (classPropHashTableGetterFunction)
                return classPropHashTableGetterFunction(exec);
            return staticPropHashTable;
        }

        const HashTable* staticPropHashTable;
        typedef const HashTable* (*ClassPropHashTableGetterFunction)(ExecState*);
        const ClassPropHashTableGetterFunction classPropHashTableGetterFunction;
    };

} // namespace AJ

#endif // ClassInfo_h
