
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
 *  Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef Lexer_h
#define Lexer_h

#include "Lookup.h"
#include "ParserArena.h"
#include "SourceCode.h"
#include <wtf/ASCIICType.h>
#include <wtf/SegmentedVector.h>
#include <wtf/Vector.h>
#include <wtf/unicode/Unicode.h>

namespace AJ {

    class RegExp;

    class Lexer : public Noncopyable {
    public:
        // Character manipulation functions.
        static bool isWhiteSpace(int character);
        static bool isLineTerminator(int character);
        static unsigned char convertHex(int c1, int c2);
        static UChar convertUnicode(int c1, int c2, int c3, int c4);

        // Functions to set up parsing.
        void setCode(const SourceCode&, ParserArena&);
        void setIsReparsing() { m_isReparsing = true; }

        // Functions for the parser itself.
        int lex(void* lvalp, void* llocp);
        int lineNumber() const { return m_lineNumber; }
        bool prevTerminator() const { return m_terminator; }
        SourceCode sourceCode(int openBrace, int closeBrace, int firstLine);
        bool scanRegExp(const Identifier*& pattern, const Identifier*& flags, UChar patternPrefix = 0);
        bool skipRegExp();

        // Functions for use after parsing.
        bool sawError() const { return m_error; }
        void clear();

    private:
        friend class AJGlobalData;

        Lexer(AJGlobalData*);
        ~Lexer();

        void shift1();
        void shift2();
        void shift3();
        void shift4();
        void shiftLineTerminator();

        void record8(int);
        void record16(int);
        void record16(UChar);

        void copyCodeWithoutBOMs();

        int currentOffset() const;
        const UChar* currentCharacter() const;

        const Identifier* makeIdentifier(const UChar* characters, size_t length);

        bool lastTokenWasRestrKeyword() const;

        static const size_t initialReadBufferCapacity = 32;

        int m_lineNumber;

        Vector<char> m_buffer8;
        Vector<UChar> m_buffer16;
        bool m_terminator;
        bool m_delimited; // encountered delimiter like "'" and "}" on last run
        int m_lastToken;

        const SourceCode* m_source;
        const UChar* m_code;
        const UChar* m_codeStart;
        const UChar* m_codeEnd;
        bool m_isReparsing;
        bool m_atLineStart;
        bool m_error;

        // current and following unicode characters (int to allow for -1 for end-of-file marker)
        int m_current;
        int m_next1;
        int m_next2;
        int m_next3;
        
        IdentifierArena* m_arena;

        AJGlobalData* m_globalData;

        const HashTable m_keywordTable;

        Vector<UChar> m_codeWithoutBOMs;
    };

    inline bool Lexer::isWhiteSpace(int ch)
    {
        return isASCII(ch) ? (ch == ' ' || ch == '\t' || ch == 0xB || ch == 0xC) : ATF::Unicode::isSeparatorSpace(ch);
    }

    inline bool Lexer::isLineTerminator(int ch)
    {
        return ch == '\r' || ch == '\n' || (ch & ~1) == 0x2028;
    }

    inline unsigned char Lexer::convertHex(int c1, int c2)
    {
        return (toASCIIHexValue(c1) << 4) | toASCIIHexValue(c2);
    }

    inline UChar Lexer::convertUnicode(int c1, int c2, int c3, int c4)
    {
        return (convertHex(c1, c2) << 8) | convertHex(c3, c4);
    }

    // A bridge for yacc from the C world to the C++ world.
    inline int jscyylex(void* lvalp, void* llocp, void* globalData)
    {
        return static_cast<AJGlobalData*>(globalData)->lexer->lex(lvalp, llocp);
    }

} // namespace AJ

#endif // Lexer_h
