
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
 * Copyright (C) 2008, 2009 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SmallStrings_h
#define SmallStrings_h

#include "UString.h"
#include <wtf/OwnPtr.h>

namespace AJ {

    class AJGlobalData;
    class AJString;
    class MarkStack;
    class SmallStringsStorage;

    class SmallStrings : public Noncopyable {
    public:
        SmallStrings();
        ~SmallStrings();

        AJString* emptyString(AJGlobalData* globalData)
        {
            if (!m_emptyString)
                createEmptyString(globalData);
            return m_emptyString;
        }
        AJString* singleCharacterString(AJGlobalData* globalData, unsigned char character)
        {
            if (!m_singleCharacterStrings[character])
                createSingleCharacterString(globalData, character);
            return m_singleCharacterStrings[character];
        }

        UString::Rep* singleCharacterStringRep(unsigned char character);

        void markChildren(MarkStack&);
        void clear();

        unsigned count() const;
#if ENABLE(JIT)
        AJString** singleCharacterStrings() { return m_singleCharacterStrings; }
#endif
    private:
        void createEmptyString(AJGlobalData*);
        void createSingleCharacterString(AJGlobalData*, unsigned char);

        AJString* m_emptyString;
        AJString* m_singleCharacterStrings[0x100];
        OwnPtr<SmallStringsStorage> m_storage;
    };

} // namespace AJ

#endif // SmallStrings_h
