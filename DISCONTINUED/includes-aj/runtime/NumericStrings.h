
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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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

#ifndef NumericStrings_h
#define NumericStrings_h

#include "UString.h"
#include <wtf/HashFunctions.h>

namespace AJ {

    class NumericStrings {
    public:
        UString add(double d)
        {
            CacheEntry<double>& entry = lookup(d);
            if (d == entry.key && !entry.value.isNull())
                return entry.value;
            entry.key = d;
            entry.value = UString::from(d);
            return entry.value;
        }

        UString add(int i)
        {
            if (static_cast<unsigned>(i) < cacheSize)
                return lookupSmallString(static_cast<unsigned>(i));
            CacheEntry<int>& entry = lookup(i);
            if (i == entry.key && !entry.value.isNull())
                return entry.value;
            entry.key = i;
            entry.value = UString::from(i);
            return entry.value;
        }

        UString add(unsigned i)
        {
            if (i < cacheSize)
                return lookupSmallString(static_cast<unsigned>(i));
            CacheEntry<unsigned>& entry = lookup(i);
            if (i == entry.key && !entry.value.isNull())
                return entry.value;
            entry.key = i;
            entry.value = UString::from(i);
            return entry.value;
        }
    private:
        static const size_t cacheSize = 64;

        template<typename T>
        struct CacheEntry {
            T key;
            UString value;
        };

        CacheEntry<double>& lookup(double d) { return doubleCache[ATF::FloatHash<double>::hash(d) & (cacheSize - 1)]; }
        CacheEntry<int>& lookup(int i) { return intCache[ATF::IntHash<int>::hash(i) & (cacheSize - 1)]; }
        CacheEntry<unsigned>& lookup(unsigned i) { return unsignedCache[ATF::IntHash<unsigned>::hash(i) & (cacheSize - 1)]; }
        const UString& lookupSmallString(unsigned i)
        {
            ASSERT(i < cacheSize);
            if (smallIntCache[i].isNull())
                smallIntCache[i] = UString::from(i);
            return smallIntCache[i];
        }

        CacheEntry<double> doubleCache[cacheSize];
        CacheEntry<int> intCache[cacheSize];
        CacheEntry<unsigned> unsignedCache[cacheSize];
        UString smallIntCache[cacheSize];
    };

} // namespace AJ

#endif // NumericStrings_h
