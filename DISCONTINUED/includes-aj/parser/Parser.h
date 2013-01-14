
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

#ifndef Parser_h
#define Parser_h

#include "Debugger.h"
#include "Executable.h"
#include "AJGlobalObject.h"
#include "Lexer.h"
#include "Nodes.h"
#include "ParserArena.h"
#include "SourceProvider.h"
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/OwnPtr.h>
#include <wtf/RefPtr.h>

namespace AJ {

    class FunctionBodyNode;
    class ProgramNode;
    class UString;

    template <typename T> struct ParserArenaData : ParserArenaDeletable { T data; };

    class Parser : public Noncopyable {
    public:
        template <class ParsedNode>
        PassRefPtr<ParsedNode> parse(AJGlobalData* globalData, Debugger*, ExecState*, const SourceCode& source, int* errLine = 0, UString* errMsg = 0);

        void didFinishParsing(SourceElements*, ParserArenaData<DeclarationStacks::VarStack>*, 
                              ParserArenaData<DeclarationStacks::FunctionStack>*, CodeFeatures features, int lastLine, int numConstants);

        ParserArena& arena() { return m_arena; }

    private:
        void parse(AJGlobalData*, int* errLine, UString* errMsg);

        ParserArena m_arena;
        const SourceCode* m_source;
        SourceElements* m_sourceElements;
        ParserArenaData<DeclarationStacks::VarStack>* m_varDeclarations;
        ParserArenaData<DeclarationStacks::FunctionStack>* m_funcDeclarations;
        CodeFeatures m_features;
        int m_lastLine;
        int m_numConstants;
    };

    template <class ParsedNode>
    PassRefPtr<ParsedNode> Parser::parse(AJGlobalData* globalData, Debugger* debugger, ExecState* debuggerExecState, const SourceCode& source, int* errLine, UString* errMsg)
    {
        m_source = &source;
        if (ParsedNode::scopeIsFunction)
            globalData->lexer->setIsReparsing();
        parse(globalData, errLine, errMsg);

        RefPtr<ParsedNode> result;
        if (m_sourceElements) {
            result = ParsedNode::create(globalData,
            m_sourceElements,
            m_varDeclarations ? &m_varDeclarations->data : 0,
            m_funcDeclarations ? &m_funcDeclarations->data : 0,
            source,
            m_features,
            m_numConstants);
            result->setLoc(m_source->firstLine(), m_lastLine);
        }

        m_arena.reset();

        m_source = 0;
        m_sourceElements = 0;
        m_varDeclarations = 0;
        m_funcDeclarations = 0;

        if (debugger && !ParsedNode::scopeIsFunction)
            debugger->sourceParsed(debuggerExecState, source, *errLine, *errMsg);
        return result.release();
    }

} // namespace AJ

#endif // Parser_h
