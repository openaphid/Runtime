
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

#ifndef AJBase_h
#define AJBase_h

#ifndef __cplusplus
#include <stdbool.h>
#endif

/* AJ engine interface */

/*! @typedef AJContextGroupRef A group that associates AJ contexts with one another. Contexts in the same group may share and exchange AJ objects. */
typedef const struct OpaqueAJContextGroup* AJContextGroupRef;

/*! @typedef AJContextRef A AJ execution context. Holds the global object and other execution state. */
typedef const struct OpaqueAJContext* AJContextRef;

/*! @typedef AJGlobalContextRef A global AJ execution context. A AJGlobalContext is a AJContext. */
typedef struct OpaqueAJContext* AJGlobalContextRef;

/*! @typedef AJStringRef A UTF16 character buffer. The fundamental string representation in AJ. */
typedef struct OpaqueAJString* AJStringRef;

/*! @typedef AJClassRef A AJ class. Used with AJObjectMake to construct objects with custom behavior. */
typedef struct OpaqueAJClass* AJClassRef;

/*! @typedef AJPropertyNameArrayRef An array of AJ property names. */
typedef struct OpaqueAJPropertyNameArray* AJPropertyNameArrayRef;

/*! @typedef AJPropertyNameAccumulatorRef An ordered set used to collect the names of a AJ object's properties. */
typedef struct OpaqueAJPropertyNameAccumulator* AJPropertyNameAccumulatorRef;


/* AJ data types */

/*! @typedef AJValueRef A AJ value. The base type for all AJ values, and polymorphic functions on them. */
typedef const struct OpaqueAJValue* AJValueRef;

/*! @typedef AJObjectRef A AJ object. A AJObject is a AJValue. */
typedef struct OpaqueAJValue* AJObjectRef;

/* AJ symbol exports */

#undef JS_EXPORT
#if defined(JS_NO_EXPORT)
    #define JS_EXPORT
#elif defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC__)
    #define JS_EXPORT __attribute__((visibility("default")))
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
    #if defined(BUILDING_AJCore) || defined(BUILDING_WTF)
        #define JS_EXPORT __declspec(dllexport)
    #else
        #define JS_EXPORT __declspec(dllimport)
    #endif
#else
    #define JS_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Script Evaluation */

/*!
@function AJEvaluateScript
@abstract Evaluates a string of AJ.
@param ctx The execution context to use.
@param script A AJString containing the script to evaluate.
@param thisObject The object to use as "this," or NULL to use the global object as "this."
@param sourceURL A AJString containing a URL for the script's source file. This is only used when reporting exceptions. Pass NULL if you do not care to include source file information in exceptions.
@param startingLineNumber An integer value specifying the script's starting line number in the file located at sourceURL. This is only used when reporting exceptions.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result The AJValue that results from evaluating script, or NULL if an exception is thrown.
*/
JS_EXPORT AJValueRef AJEvaluateScript(AJContextRef ctx, AJStringRef script, AJObjectRef thisObject, AJStringRef sourceURL, int startingLineNumber, AJValueRef* exception);

/*!
@function AJCheckScriptSyntax
@abstract Checks for syntax errors in a string of AJ.
@param ctx The execution context to use.
@param script A AJString containing the script to check for syntax errors.
@param sourceURL A AJString containing a URL for the script's source file. This is only used when reporting exceptions. Pass NULL if you do not care to include source file information in exceptions.
@param startingLineNumber An integer value specifying the script's starting line number in the file located at sourceURL. This is only used when reporting exceptions.
@param exception A pointer to a AJValueRef in which to store a syntax error exception, if any. Pass NULL if you do not care to store a syntax error exception.
@result true if the script is syntactically correct, otherwise false.
*/
JS_EXPORT bool AJCheckScriptSyntax(AJContextRef ctx, AJStringRef script, AJStringRef sourceURL, int startingLineNumber, AJValueRef* exception);

/*!
@function AJGarbageCollect
@abstract Performs a AJ garbage collection. 
@param ctx The execution context to use.
@discussion AJ values that are on the machine stack, in a register, 
 protected by AJValueProtect, set as the global object of an execution context, 
 or reachable from any such value will not be collected.

 During AJ execution, you are not required to call this function; the 
 AJ engine will garbage collect as needed. AJ values created
 within a context group are automatically destroyed when the last reference
 to the context group is released.
*/
JS_EXPORT void AJGarbageCollect(AJContextRef ctx);

#ifdef __cplusplus
}
#endif

#endif /* AJBase_h */
