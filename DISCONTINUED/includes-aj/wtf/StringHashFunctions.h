
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
 * Copyright (C) 2005, 2006, 2008, 2010 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */
#ifndef WTF_StringHashFunctions_h
#define WTF_StringHashFunctions_h

#include <wtf/unicode/Unicode.h>

namespace ATF {

// Golden ratio - arbitrary start value to avoid mapping all 0's to all 0's
static const unsigned stringHashingStartValue = 0x9e3779b9U;

// stringHash methods based on Paul Hsieh's SuperFastHash.
// http://www.azillionmonkeys.com/qed/hash.html
// char* data is interpreted as latin-encoded (zero extended to 16 bits).

inline unsigned stringHash(const UChar* data, unsigned length)
{
    unsigned hash = ATF::stringHashingStartValue;
    unsigned rem = length & 1;
    length >>= 1;

    // Main loop
    for (; length > 0; length--) {
        hash += data[0];
        unsigned tmp = (data[1] << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2;
        hash += hash >> 11;
    }

    // Handle end case
    if (rem) {
        hash += data[0];
        hash ^= hash << 11;
        hash += hash >> 17;
    }

    // Force "avalanching" of final 127 bits
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 2;
    hash += hash >> 15;
    hash ^= hash << 10;

    hash &= 0x7fffffff;

    // this avoids ever returning a hash code of 0, since that is used to
    // signal "hash not computed yet", using a value that is likely to be
    // effectively the same as 0 when the low bits are masked
    if (hash == 0)
        hash = 0x40000000;

    return hash;
}

inline unsigned stringHash(const char* data, unsigned length)
{
    unsigned hash = ATF::stringHashingStartValue;
    unsigned rem = length & 1;
    length >>= 1;

    // Main loop
    for (; length > 0; length--) {
        hash += static_cast<unsigned char>(data[0]);
        unsigned tmp = (static_cast<unsigned char>(data[1]) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2;
        hash += hash >> 11;
    }

    // Handle end case
    if (rem) {
        hash += static_cast<unsigned char>(data[0]);
        hash ^= hash << 11;
        hash += hash >> 17;
    }

    // Force "avalanching" of final 127 bits
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 2;
    hash += hash >> 15;
    hash ^= hash << 10;

    hash &= 0x7fffffff;

    // this avoids ever returning a hash code of 0, since that is used to
    // signal "hash not computed yet", using a value that is likely to be
    // effectively the same as 0 when the low bits are masked
    if (hash == 0)
        hash = 0x40000000;

    return hash;
}

inline unsigned stringHash(const char* data)
{
    unsigned hash = ATF::stringHashingStartValue;

    // Main loop
    for (;;) {
        unsigned char b0 = data[0];
        if (!b0)
            break;
        unsigned char b1 = data[1];
        if (!b1) {
            hash += b0;
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        }
        hash += b0;
        unsigned tmp = (b1 << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2;
        hash += hash >> 11;
    }

    // Force "avalanching" of final 127 bits.
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 2;
    hash += hash >> 15;
    hash ^= hash << 10;

    hash &= 0x7fffffff;

    // This avoids ever returning a hash code of 0, since that is used to
    // signal "hash not computed yet", using a value that is likely to be
    // effectively the same as 0 when the low bits are masked.
    if (hash == 0)
        hash = 0x40000000;

    return hash;
}

} // namespace ATF

#endif // WTF_StringHashFunctions_h
