
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
 *  Copyright (C) 2003, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Cameron Zwarich (cwzwarich@uwaterloo.ca)
 *  Copyright (C) 2007 Maks Orlovich
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

#ifndef InternalFunction_h
#define InternalFunction_h

#include "AJObject.h"
#include "Identifier.h"

namespace AJ {

    class FunctionPrototype;

    class InternalFunction : public AJObject {
    public:
        virtual const ClassInfo* classInfo() const; 
        static JS_EXPORTDATA const ClassInfo info;

        const UString& name(ExecState*);
        const UString displayName(ExecState*);
        const UString calculatedDisplayName(ExecState*);

        static PassRefPtr<Structure> createStructure(AJValue proto) 
        { 
            return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
        }

    protected:
        static const unsigned StructureFlags = ImplementsHasInstance | AJObject::StructureFlags;

        InternalFunction(NonNullPassRefPtr<Structure> structure) : AJObject(structure) { }
        InternalFunction(AJGlobalData*, NonNullPassRefPtr<Structure>, const Identifier&);

    private:
        virtual CallType getCallData(CallData&) = 0;
    };

    InternalFunction* asInternalFunction(AJValue);

    inline InternalFunction* asInternalFunction(AJValue value)
    {
        ASSERT(asObject(value)->inherits(&InternalFunction::info));
        return static_cast<InternalFunction*>(asObject(value));
    }

} // namespace AJ

#endif // InternalFunction_h
