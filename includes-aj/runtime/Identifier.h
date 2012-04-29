
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
 *  Copyright (C) 2003, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef Identifier_h
#define Identifier_h

#include "AJGlobalData.h"
#include "ThreadSpecific.h"
#include "UString.h"

namespace AJ {

    class ExecState;

    class Identifier {
        friend class Structure;
    public:
        Identifier() { }

        Identifier(ExecState* exec, const char* s) : _ustring(add(exec, s)) { } // Only to be used with string literals.
        Identifier(ExecState* exec, const UChar* s, int length) : _ustring(add(exec, s, length)) { }
        Identifier(ExecState* exec, UString::Rep* rep) : _ustring(add(exec, rep)) { } 
        Identifier(ExecState* exec, const UString& s) : _ustring(add(exec, s.rep())) { }

        Identifier(AJGlobalData* globalData, const char* s) : _ustring(add(globalData, s)) { } // Only to be used with string literals.
        Identifier(AJGlobalData* globalData, const UChar* s, int length) : _ustring(add(globalData, s, length)) { }
        Identifier(AJGlobalData* globalData, UString::Rep* rep) : _ustring(add(globalData, rep)) { } 
        Identifier(AJGlobalData* globalData, const UString& s) : _ustring(add(globalData, s.rep())) { }

        // Special constructor for cases where we overwrite an object in place.
        Identifier(PlacementNewAdoptType) : _ustring(PlacementNewAdopt) { }
        
        const UString& ustring() const { return _ustring; }
        
        const UChar* data() const { return _ustring.data(); }
        int size() const { return _ustring.size(); }
        
        const char* ascii() const { return _ustring.ascii(); }
        
        static Identifier from(ExecState* exec, unsigned y);
        static Identifier from(ExecState* exec, int y);
        static Identifier from(ExecState* exec, double y);
        
        bool isNull() const { return _ustring.isNull(); }
        bool isEmpty() const { return _ustring.isEmpty(); }
        
        uint32_t toUInt32(bool* ok) const { return _ustring.toUInt32(ok); }
        uint32_t toUInt32(bool* ok, bool tolerateEmptyString) const { return _ustring.toUInt32(ok, tolerateEmptyString); };
        uint32_t toStrictUInt32(bool* ok) const { return _ustring.toStrictUInt32(ok); }
        unsigned toArrayIndex(bool* ok) const { return _ustring.toArrayIndex(ok); }
        double toDouble() const { return _ustring.toDouble(); }
        
        friend bool operator==(const Identifier&, const Identifier&);
        friend bool operator!=(const Identifier&, const Identifier&);

        friend bool operator==(const Identifier&, const char*);
        friend bool operator!=(const Identifier&, const char*);
    
        static bool equal(const UString::Rep*, const char*);
        static bool equal(const UString::Rep*, const UChar*, unsigned length);
        static bool equal(const UString::Rep* a, const UString::Rep* b) { return ::equal(a, b); }

        static PassRefPtr<UString::Rep> add(ExecState*, const char*); // Only to be used with string literals.
        static PassRefPtr<UString::Rep> add(AJGlobalData*, const char*); // Only to be used with string literals.

    private:
        UString _ustring;
        
        static bool equal(const Identifier& a, const Identifier& b) { return a._ustring.rep() == b._ustring.rep(); }
        static bool equal(const Identifier& a, const char* b) { return equal(a._ustring.rep(), b); }

        static PassRefPtr<UString::Rep> add(ExecState*, const UChar*, int length);
        static PassRefPtr<UString::Rep> add(AJGlobalData*, const UChar*, int length);

        static PassRefPtr<UString::Rep> add(ExecState* exec, UString::Rep* r)
        {
#ifndef NDEBUG
            checkCurrentIdentifierTable(exec);
#endif
            if (r->isIdentifier())
                return r;
            return addSlowCase(exec, r);
        }
        static PassRefPtr<UString::Rep> add(AJGlobalData* globalData, UString::Rep* r)
        {
#ifndef NDEBUG
            checkCurrentIdentifierTable(globalData);
#endif
            if (r->isIdentifier())
                return r;
            return addSlowCase(globalData, r);
        }

        static PassRefPtr<UString::Rep> addSlowCase(ExecState*, UString::Rep* r);
        static PassRefPtr<UString::Rep> addSlowCase(AJGlobalData*, UString::Rep* r);

        static void checkCurrentIdentifierTable(ExecState*);
        static void checkCurrentIdentifierTable(AJGlobalData*);
    };
    
    inline bool operator==(const Identifier& a, const Identifier& b)
    {
        return Identifier::equal(a, b);
    }

    inline bool operator!=(const Identifier& a, const Identifier& b)
    {
        return !Identifier::equal(a, b);
    }

    inline bool operator==(const Identifier& a, const char* b)
    {
        return Identifier::equal(a, b);
    }

    inline bool operator!=(const Identifier& a, const char* b)
    {
        return !Identifier::equal(a, b);
    }

    IdentifierTable* createIdentifierTable();
    void deleteIdentifierTable(IdentifierTable*);

} // namespace AJ

#endif // Identifier_h
