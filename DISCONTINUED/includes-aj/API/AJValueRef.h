
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

#ifndef AJValueRef_h
#define AJValueRef_h

#include <AJCore/AJBase.h>
#include <AJCore/WebKitAvailability.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

/*!
@enum AJType
@abstract     A constant identifying the type of a AJValue.
@constant     kAJTypeUndefined  The unique undefined value.
@constant     kAJTypeNull       The unique null value.
@constant     kAJTypeBoolean    A primitive boolean value, one of true or false.
@constant     kAJTypeNumber     A primitive number value.
@constant     kAJTypeString     A primitive string value.
@constant     kAJTypeObject     An object value (meaning that this AJValueRef is a AJObjectRef).
*/
typedef enum {
    kAJTypeUndefined,
    kAJTypeNull,
    kAJTypeBoolean,
    kAJTypeNumber,
    kAJTypeString,
    kAJTypeObject
} AJType;

#ifdef __cplusplus
extern "C" {
#endif

/*!
@function
@abstract       Returns a AJ value's type.
@param ctx  The execution context to use.
@param value    The AJValue whose type you want to obtain.
@result         A value of type AJType that identifies value's type.
*/
JS_EXPORT AJType AJValueGetType(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Tests whether a AJ value's type is the undefined type.
@param ctx  The execution context to use.
@param value    The AJValue to test.
@result         true if value's type is the undefined type, otherwise false.
*/
JS_EXPORT bool AJValueIsUndefined(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Tests whether a AJ value's type is the null type.
@param ctx  The execution context to use.
@param value    The AJValue to test.
@result         true if value's type is the null type, otherwise false.
*/
JS_EXPORT bool AJValueIsNull(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Tests whether a AJ value's type is the boolean type.
@param ctx  The execution context to use.
@param value    The AJValue to test.
@result         true if value's type is the boolean type, otherwise false.
*/
JS_EXPORT bool AJValueIsBoolean(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Tests whether a AJ value's type is the number type.
@param ctx  The execution context to use.
@param value    The AJValue to test.
@result         true if value's type is the number type, otherwise false.
*/
JS_EXPORT bool AJValueIsNumber(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Tests whether a AJ value's type is the string type.
@param ctx  The execution context to use.
@param value    The AJValue to test.
@result         true if value's type is the string type, otherwise false.
*/
JS_EXPORT bool AJValueIsString(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Tests whether a AJ value's type is the object type.
@param ctx  The execution context to use.
@param value    The AJValue to test.
@result         true if value's type is the object type, otherwise false.
*/
JS_EXPORT bool AJValueIsObject(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract Tests whether a AJ value is an object with a given class in its class chain.
@param ctx The execution context to use.
@param value The AJValue to test.
@param jsClass The AJClass to test against.
@result true if value is an object and has jsClass in its class chain, otherwise false.
*/
JS_EXPORT bool AJValueIsObjectOfClass(AJContextRef ctx, AJValueRef value, AJClassRef jsClass);

/* Comparing values */

/*!
@function
@abstract Tests whether two AJ values are equal, as compared by the JS == operator.
@param ctx The execution context to use.
@param a The first value to test.
@param b The second value to test.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result true if the two values are equal, false if they are not equal or an exception is thrown.
*/
JS_EXPORT bool AJValueIsEqual(AJContextRef ctx, AJValueRef a, AJValueRef b, AJValueRef* exception);

/*!
@function
@abstract       Tests whether two AJ values are strict equal, as compared by the JS === operator.
@param ctx  The execution context to use.
@param a        The first value to test.
@param b        The second value to test.
@result         true if the two values are strict equal, otherwise false.
*/
JS_EXPORT bool AJValueIsStrictEqual(AJContextRef ctx, AJValueRef a, AJValueRef b);

/*!
@function
@abstract Tests whether a AJ value is an object constructed by a given constructor, as compared by the JS instanceof operator.
@param ctx The execution context to use.
@param value The AJValue to test.
@param constructor The constructor to test against.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result true if value is an object constructed by constructor, as compared by the JS instanceof operator, otherwise false.
*/
JS_EXPORT bool AJValueIsInstanceOfConstructor(AJContextRef ctx, AJValueRef value, AJObjectRef constructor, AJValueRef* exception);

/* Creating values */

/*!
@function
@abstract       Creates a AJ value of the undefined type.
@param ctx  The execution context to use.
@result         The unique undefined value.
*/
JS_EXPORT AJValueRef AJValueMakeUndefined(AJContextRef ctx);

/*!
@function
@abstract       Creates a AJ value of the null type.
@param ctx  The execution context to use.
@result         The unique null value.
*/
JS_EXPORT AJValueRef AJValueMakeNull(AJContextRef ctx);

/*!
@function
@abstract       Creates a AJ value of the boolean type.
@param ctx  The execution context to use.
@param boolean  The bool to assign to the newly created AJValue.
@result         A AJValue of the boolean type, representing the value of boolean.
*/
JS_EXPORT AJValueRef AJValueMakeBoolean(AJContextRef ctx, bool boolean);

/*!
@function
@abstract       Creates a AJ value of the number type.
@param ctx  The execution context to use.
@param number   The double to assign to the newly created AJValue.
@result         A AJValue of the number type, representing the value of number.
*/
JS_EXPORT AJValueRef AJValueMakeNumber(AJContextRef ctx, double number);

/*!
@function
@abstract       Creates a AJ value of the string type.
@param ctx  The execution context to use.
@param string   The AJString to assign to the newly created AJValue. The
 newly created AJValue retains string, and releases it upon garbage collection.
@result         A AJValue of the string type, representing the value of string.
*/
JS_EXPORT AJValueRef AJValueMakeString(AJContextRef ctx, AJStringRef string);

/* Converting to and from JSON formatted strings */

/*!
 @function
 @abstract       Creates a AJ value from a JSON formatted string.
 @param ctx      The execution context to use.
 @param string   The AJString containing the JSON string to be parsed.
 @result         A AJValue containing the parsed value, or NULL if the input is invalid.
 */
JS_EXPORT AJValueRef AJValueMakeFromJSONString(AJContextRef ctx, AJStringRef string) AVAILABLE_AFTER_WEBKIT_VERSION_4_0;

/*!
 @function
 @abstract       Creates a AJ string containing the JSON serialized representation of a JS value.
 @param ctx      The execution context to use.
 @param value    The value to serialize.
 @param indent   The number of spaces to indent when nesting.  If 0, the resulting JSON will not contains newlines.  The size of the indent is clamped to 10 spaces.
 @param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
 @result         A AJString with the result of serialization, or NULL if an exception is thrown.
 */
JS_EXPORT AJStringRef AJValueCreateJSONString(AJContextRef ctx, AJValueRef value, unsigned indent, AJValueRef* exception) AVAILABLE_AFTER_WEBKIT_VERSION_4_0;

/* Converting to primitive values */

/*!
@function
@abstract       Converts a AJ value to boolean and returns the resulting boolean.
@param ctx  The execution context to use.
@param value    The AJValue to convert.
@result         The boolean result of conversion.
*/
JS_EXPORT bool AJValueToBoolean(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Converts a AJ value to number and returns the resulting number.
@param ctx  The execution context to use.
@param value    The AJValue to convert.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result         The numeric result of conversion, or NaN if an exception is thrown.
*/
JS_EXPORT double AJValueToNumber(AJContextRef ctx, AJValueRef value, AJValueRef* exception);

/*!
@function
@abstract       Converts a AJ value to string and copies the result into a AJ string.
@param ctx  The execution context to use.
@param value    The AJValue to convert.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result         A AJString with the result of conversion, or NULL if an exception is thrown. Ownership follows the Create Rule.
*/
JS_EXPORT AJStringRef AJValueToStringCopy(AJContextRef ctx, AJValueRef value, AJValueRef* exception);

/*!
@function
@abstract Converts a AJ value to object and returns the resulting object.
@param ctx  The execution context to use.
@param value    The AJValue to convert.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result         The AJObject result of conversion, or NULL if an exception is thrown.
*/
JS_EXPORT AJObjectRef AJValueToObject(AJContextRef ctx, AJValueRef value, AJValueRef* exception);

/* Garbage collection */
/*!
@function
@abstract Protects a AJ value from garbage collection.
@param ctx The execution context to use.
@param value The AJValue to protect.
@discussion Use this method when you want to store a AJValue in a global or on the heap, where the garbage collector will not be able to discover your reference to it.
 
A value may be protected multiple times and must be unprotected an equal number of times before becoming eligible for garbage collection.
*/
JS_EXPORT void AJValueProtect(AJContextRef ctx, AJValueRef value);

/*!
@function
@abstract       Unprotects a AJ value from garbage collection.
@param ctx      The execution context to use.
@param value    The AJValue to unprotect.
@discussion     A value may be protected multiple times and must be unprotected an 
 equal number of times before becoming eligible for garbage collection.
*/
JS_EXPORT void AJValueUnprotect(AJContextRef ctx, AJValueRef value);

#ifdef __cplusplus
}
#endif

#endif /* AJValueRef_h */
