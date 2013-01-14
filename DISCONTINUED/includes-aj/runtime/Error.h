
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

#ifndef Error_h
#define Error_h

#include <stdint.h>

namespace AJ {

    class ExecState;
    class AJObject;
    class UString;

    /**
     * Types of Native Errors available. For custom errors, GeneralError
     * should be used.
     */
    enum ErrorType {
        GeneralError   = 0,
        EvalError      = 1,
        RangeError     = 2,
        ReferenceError = 3,
        SyntaxError    = 4,
        TypeError      = 5,
        URIError       = 6
    };
    
    extern const char* expressionBeginOffsetPropertyName;
    extern const char* expressionCaretOffsetPropertyName;
    extern const char* expressionEndOffsetPropertyName;
    
    class Error {
    public:
        static AJObject* create(ExecState*, ErrorType, const UString& message, int lineNumber, intptr_t sourceID, const UString& sourceURL);
        static AJObject* create(ExecState*, ErrorType, const char* message);
    };

    AJObject* throwError(ExecState*, ErrorType, const UString& message, int lineNumber, intptr_t sourceID, const UString& sourceURL);
    AJObject* throwError(ExecState*, ErrorType, const UString& message);
    AJObject* throwError(ExecState*, ErrorType, const char* message);
    AJObject* throwError(ExecState*, ErrorType);
    AJObject* throwError(ExecState*, AJObject*);

} // namespace AJ

#endif // Error_h
