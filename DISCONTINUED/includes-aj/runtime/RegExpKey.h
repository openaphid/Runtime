
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
 * Copyright (C) 2010 University of Szeged
 * Copyright (C) 2010 Renata Hodovan (hodovan@inf.u-szeged.hu)
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY UNIVERSITY OF SZEGED ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL UNIVERSITY OF SZEGED OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "UString.h"

#ifndef RegExpKey_h
#define RegExpKey_h

namespace AJ {

struct RegExpKey {
    int flagsValue;
    RefPtr<UString::Rep> pattern;

    RegExpKey()
        : flagsValue(0)
    {
    }

    RegExpKey(int flags)
        : flagsValue(flags)
    {
    }

    RegExpKey(int flags, const UString& pattern)
        : flagsValue(flags)
        , pattern(pattern.rep())
    {
    }

    RegExpKey(int flags, const PassRefPtr<UString::Rep> pattern)
        : flagsValue(flags)
        , pattern(pattern)
    {
    }

    RegExpKey(const UString& flags, const UString& pattern)
        : pattern(pattern.rep())
    {
        flagsValue = getFlagsValue(flags);
    }

    int getFlagsValue(const UString flags) 
    {
        flagsValue = 0;
        if (flags.find('g') != UString::NotFound)
            flagsValue += 4;
        if (flags.find('i') != UString::NotFound)
            flagsValue += 2;
        if (flags.find('m') != UString::NotFound)
            flagsValue += 1;
        return flagsValue;
    }
};
	inline bool operator==(const AJ::RegExpKey& a, const AJ::RegExpKey& b) 
	{
    if (a.flagsValue != b.flagsValue)
			return false;
    if (!a.pattern)
			return !b.pattern;
    if (!b.pattern)
			return false;
    return equal(a.pattern.get(), b.pattern.get());
	}
} // namespace AJ

namespace ATF {
template<typename T> struct DefaultHash;
template<typename T> struct RegExpHash;

	/*
inline bool operator==(const AJ::RegExpKey& a, const AJ::RegExpKey& b) 
{
    if (a.flagsValue != b.flagsValue)
        return false;
    if (!a.pattern)
        return !b.pattern;
    if (!b.pattern)
        return false;
    return equal(a.pattern.get(), b.pattern.get());
}*/

template<> struct RegExpHash<AJ::RegExpKey> {
    static unsigned hash(const AJ::RegExpKey& key) { return key.pattern->hash(); }
    static bool equal(const AJ::RegExpKey& a, const AJ::RegExpKey& b) { return a == b; }
    static const bool safeToCompareToEmptyOrDeleted = false;
};

template<> struct DefaultHash<AJ::RegExpKey> {
    typedef RegExpHash<AJ::RegExpKey> Hash;
};

template<> struct HashTraits<AJ::RegExpKey> : GenericHashTraits<AJ::RegExpKey> {
    static void constructDeletedValue(AJ::RegExpKey& slot) { slot.flagsValue = -1; }
    static bool isDeletedValue(const AJ::RegExpKey& value) { return value.flagsValue == -1; }
};
} // namespace ATF

#endif // RegExpKey_h
