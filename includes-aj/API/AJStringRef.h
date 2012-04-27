
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
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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
 */

#ifndef AJStringRef_h
#define AJStringRef_h

#include <AJCore/AJValueRef.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WINSCW__) \
    && !((defined(__CC_ARM) || defined(__ARMCC__)) && defined(__SYMBIAN32__)) /* RVCT */
/*!
@typedef AJChar
@abstract A Unicode character.
*/
    typedef unsigned short AJChar;
#else
    typedef wchar_t AJChar;
#endif

/*!
@function
@abstract         Creates a AJ string from a buffer of Unicode characters.
@param chars      The buffer of Unicode characters to copy into the new AJString.
@param numChars   The number of characters to copy from the buffer pointed to by chars.
@result           A AJString containing chars. Ownership follows the Create Rule.
*/
JS_EXPORT AJStringRef AJStringCreateWithCharacters(const AJChar* chars, size_t numChars);
/*!
@function
@abstract         Creates a AJ string from a null-terminated UTF8 string.
@param string     The null-terminated UTF8 string to copy into the new AJString.
@result           A AJString containing string. Ownership follows the Create Rule.
*/
JS_EXPORT AJStringRef AJStringCreateWithUTF8CString(const char* string);

/*!
@function
@abstract         Retains a AJ string.
@param string     The AJString to retain.
@result           A AJString that is the same as string.
*/
JS_EXPORT AJStringRef AJStringRetain(AJStringRef string);
/*!
@function
@abstract         Releases a AJ string.
@param string     The AJString to release.
*/
JS_EXPORT void AJStringRelease(AJStringRef string);

/*!
@function
@abstract         Returns the number of Unicode characters in a AJ string.
@param string     The AJString whose length (in Unicode characters) you want to know.
@result           The number of Unicode characters stored in string.
*/
JS_EXPORT size_t AJStringGetLength(AJStringRef string);
/*!
@function
@abstract         Returns a pointer to the Unicode character buffer that 
 serves as the backing store for a AJ string.
@param string     The AJString whose backing store you want to access.
@result           A pointer to the Unicode character buffer that serves as string's 
 backing store, which will be deallocated when string is deallocated.
*/
JS_EXPORT const AJChar* AJStringGetCharactersPtr(AJStringRef string);

/*!
@function
@abstract Returns the maximum number of bytes a AJ string will 
 take up if converted into a null-terminated UTF8 string.
@param string The AJString whose maximum converted size (in bytes) you 
 want to know.
@result The maximum number of bytes that could be required to convert string into a 
 null-terminated UTF8 string. The number of bytes that the conversion actually ends 
 up requiring could be less than this, but never more.
*/
JS_EXPORT size_t AJStringGetMaximumUTF8CStringSize(AJStringRef string);
/*!
@function
@abstract Converts a AJ string into a null-terminated UTF8 string, 
 and copies the result into an external byte buffer.
@param string The source AJString.
@param buffer The destination byte buffer into which to copy a null-terminated 
 UTF8 representation of string. On return, buffer contains a UTF8 string 
 representation of string. If bufferSize is too small, buffer will contain only 
 partial results. If buffer is not at least bufferSize bytes in size, 
 behavior is undefined. 
@param bufferSize The size of the external buffer in bytes.
@result The number of bytes written into buffer (including the null-terminator byte).
*/
JS_EXPORT size_t AJStringGetUTF8CString(AJStringRef string, char* buffer, size_t bufferSize);

/*!
@function
@abstract     Tests whether two AJ strings match.
@param a      The first AJString to test.
@param b      The second AJString to test.
@result       true if the two strings match, otherwise false.
*/
JS_EXPORT bool AJStringIsEqual(AJStringRef a, AJStringRef b);
/*!
@function
@abstract     Tests whether a AJ string matches a null-terminated UTF8 string.
@param a      The AJString to test.
@param b      The null-terminated UTF8 string to test.
@result       true if the two strings match, otherwise false.
*/
JS_EXPORT bool AJStringIsEqualToUTF8CString(AJStringRef a, const char* b);

#ifdef __cplusplus
}
#endif

#endif /* AJStringRef_h */
