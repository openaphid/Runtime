
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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 */

#ifndef ATFThreadData_h
#define ATFThreadData_h

#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/Noncopyable.h>
#include <wtf/text/StringHash.h>

// This was ENABLE(WORKERS) in WebCore, but this is not defined when compiling JSC.
// However this check was not correct anyway, re this comment:
//    // FIXME: Workers are not necessarily the only feature that make per-thread global data necessary.
//    // We need to check for e.g. database objects manipulating strings on secondary threads.
// Always enabling this is safe, and should be a better option until we can come up
// with a better define.
#define WTFTHREADDATA_MULTITHREADED 1

#if WTFTHREADDATA_MULTITHREADED
#include <wtf/ThreadSpecific.h>
#include <wtf/Threading.h>
#endif

// FIXME: This is a temporary layering violation while we move more string code to WTF.
namespace Aphid {
class AtomicStringTable;
class StringImpl;
}
using Aphid::StringImpl;

typedef void (*AtomicStringTableDestructor)(Aphid::AtomicStringTable*);

#if USE(JSC)
// FIXME: This is a temporary layering violation while we move more string code to WTF.
namespace AJ {

typedef HashMap<const char*, RefPtr<StringImpl>, PtrHash<const char*> > LiteralIdentifierTable;

class IdentifierTable : public FastAllocBase {
public:
    ~IdentifierTable();

    std::pair<HashSet<StringImpl*>::iterator, bool> add(StringImpl* value);
    template<typename U, typename V>
    std::pair<HashSet<StringImpl*>::iterator, bool> add(U value);

    void remove(StringImpl* r) { m_table.remove(r); }

    LiteralIdentifierTable& literalTable() { return m_literalTable; }

private:
    HashSet<StringImpl*> m_table;
    LiteralIdentifierTable m_literalTable;
};

}
#endif

namespace ATF {

class ATFThreadData : public Noncopyable {
public:
    ATFThreadData();
    ~ATFThreadData();

    Aphid::AtomicStringTable* atomicStringTable()
    {
        return m_atomicStringTable;
    }

#if USE(JSC)

    AJ::IdentifierTable* currentIdentifierTable()
    {
        return m_currentIdentifierTable;
    }

    AJ::IdentifierTable* setCurrentIdentifierTable(AJ::IdentifierTable* identifierTable)
    {
        AJ::IdentifierTable* oldIdentifierTable = m_currentIdentifierTable;
        m_currentIdentifierTable = identifierTable;
        return oldIdentifierTable;
    }

    void resetCurrentIdentifierTable()
    {
        m_currentIdentifierTable = m_defaultIdentifierTable;
    }
#endif

private:
    Aphid::AtomicStringTable* m_atomicStringTable;
    AtomicStringTableDestructor m_atomicStringTableDestructor;

#if USE(JSC)
    AJ::IdentifierTable* m_defaultIdentifierTable;
    AJ::IdentifierTable* m_currentIdentifierTable;
#endif

#if WTFTHREADDATA_MULTITHREADED
    static JS_EXPORTDATA ThreadSpecific<ATFThreadData>* staticData;
#else
    static JS_EXPORTDATA ATFThreadData* staticData;
#endif
    friend ATFThreadData& wtfThreadData();
    friend class Aphid::AtomicStringTable;
};

inline ATFThreadData& wtfThreadData()
{
#if WTFTHREADDATA_MULTITHREADED
    // WRT WebCore:
    //    ATFThreadData is used on main thread before it could possibly be used
    //    on secondary ones, so there is no need for synchronization here.
    // WRT AJCore:
    //    wtfThreadData() is initially called from initializeThreading(), ensuring
    //    this is initially called in a pthread_once locked context.
    if (!ATFThreadData::staticData)
        ATFThreadData::staticData = new ThreadSpecific<ATFThreadData>;
    return **ATFThreadData::staticData;
#else
    if (!ATFThreadData::staticData) {
        ATFThreadData::staticData = static_cast<ATFThreadData*>(fastMalloc(sizeof(ATFThreadData)));
        // ATFThreadData constructor indirectly uses staticData, so we need to set up the memory before invoking it.
        new (ATFThreadData::staticData) ATFThreadData;
    }
    return *ATFThreadData::staticData;
#endif
}

} // namespace ATF

using ATF::ATFThreadData;
using ATF::wtfThreadData;

#endif // ATFThreadData_h
