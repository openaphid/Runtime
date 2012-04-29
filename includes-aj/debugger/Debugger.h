
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
 *  Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef Debugger_h
#define Debugger_h

#include <wtf/HashSet.h>

namespace AJ {

    class DebuggerCallFrame;
    class ExecState;
    class AJGlobalData;
    class AJGlobalObject;
    class AJValue;
    class SourceCode;
    class UString;

    class Debugger {
    public:
        virtual ~Debugger();

        void attach(AJGlobalObject*);
        virtual void detach(AJGlobalObject*);

        virtual void sourceParsed(ExecState*, const SourceCode&, int errorLineNumber, const UString& errorMessage) = 0;
        virtual void exception(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber, bool hasHandler) = 0;
        virtual void atStatement(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void callEvent(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void returnEvent(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;

        virtual void willExecuteProgram(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void didExecuteProgram(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void didReachBreakpoint(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;

        void recompileAllAJFunctions(AJGlobalData*);

    private:
        HashSet<AJGlobalObject*> m_globalObjects;
    };

    // This function exists only for backwards compatibility with existing WebScriptDebugger clients.
    AJValue evaluateInGlobalCallFrame(const UString&, AJValue& exception, AJGlobalObject*);

} // namespace AJ

#endif // Debugger_h
