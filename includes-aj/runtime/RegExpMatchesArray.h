
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
 *  Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef RegExpMatchesArray_h
#define RegExpMatchesArray_h

#include "AJArray.h"

namespace AJ {

    class RegExpMatchesArray : public AJArray {
    public:
        RegExpMatchesArray(ExecState*, RegExpConstructorPrivate*);
        virtual ~RegExpMatchesArray();

    private:
        virtual bool getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
        {
            if (subclassData())
                fillArrayInstance(exec);
            return AJArray::getOwnPropertySlot(exec, propertyName, slot);
        }

        virtual bool getOwnPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
        {
            if (subclassData())
                fillArrayInstance(exec);
            return AJArray::getOwnPropertySlot(exec, propertyName, slot);
        }

        virtual bool getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
        {
            if (subclassData())
                fillArrayInstance(exec);
            return AJArray::getOwnPropertyDescriptor(exec, propertyName, descriptor);
        }

        virtual void put(ExecState* exec, const Identifier& propertyName, AJValue v, PutPropertySlot& slot)
        {
            if (subclassData())
                fillArrayInstance(exec);
            AJArray::put(exec, propertyName, v, slot);
        }

        virtual void put(ExecState* exec, unsigned propertyName, AJValue v)
        {
            if (subclassData())
                fillArrayInstance(exec);
            AJArray::put(exec, propertyName, v);
        }

        virtual bool deleteProperty(ExecState* exec, const Identifier& propertyName)
        {
            if (subclassData())
                fillArrayInstance(exec);
            return AJArray::deleteProperty(exec, propertyName);
        }

        virtual bool deleteProperty(ExecState* exec, unsigned propertyName)
        {
            if (subclassData())
                fillArrayInstance(exec);
            return AJArray::deleteProperty(exec, propertyName);
        }

        virtual void getOwnPropertyNames(ExecState* exec, PropertyNameArray& arr, EnumerationMode mode = ExcludeDontEnumProperties)
        {
            if (subclassData())
                fillArrayInstance(exec);
            AJArray::getOwnPropertyNames(exec, arr, mode);
        }

        void fillArrayInstance(ExecState*);
};

}

#endif // RegExpMatchesArray_h
