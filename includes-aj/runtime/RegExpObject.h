
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
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2007, 2008 Apple Inc. All Rights Reserved.
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

#ifndef RegExpObject_h
#define RegExpObject_h

#include "AJObject.h"
#include "RegExp.h"

namespace AJ {

    class RegExpObject : public AJObject {
    public:
        RegExpObject(NonNullPassRefPtr<Structure>, NonNullPassRefPtr<RegExp>);
        virtual ~RegExpObject();

        void setRegExp(PassRefPtr<RegExp> r) { d->regExp = r; }
        RegExp* regExp() const { return d->regExp.get(); }

        void setLastIndex(double lastIndex) { d->lastIndex = lastIndex; }
        double lastIndex() const { return d->lastIndex; }

        AJValue test(ExecState*, const ArgList&);
        AJValue exec(ExecState*, const ArgList&);

        virtual bool getOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
        virtual void put(ExecState*, const Identifier& propertyName, AJValue, PutPropertySlot&);

        virtual const ClassInfo* classInfo() const { return &info; }
        static const ClassInfo info;

        static PassRefPtr<Structure> createStructure(AJValue prototype)
        {
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount);
        }

    protected:
        static const unsigned StructureFlags = OverridesGetOwnPropertySlot | AJObject::StructureFlags;

    private:
        bool match(ExecState*, const ArgList&);

        virtual CallType getCallData(CallData&);

        struct RegExpObjectData : FastAllocBase {
            RegExpObjectData(NonNullPassRefPtr<RegExp> regExp, double lastIndex)
                : regExp(regExp)
                , lastIndex(lastIndex)
            {
            }

            RefPtr<RegExp> regExp;
            double lastIndex;
        };

        OwnPtr<RegExpObjectData> d;
    };

    RegExpObject* asRegExpObject(AJValue);

    inline RegExpObject* asRegExpObject(AJValue value)
    {
        ASSERT(asObject(value)->inherits(&RegExpObject::info));
        return static_cast<RegExpObject*>(asObject(value));
    }

} // namespace AJ

#endif // RegExpObject_h
