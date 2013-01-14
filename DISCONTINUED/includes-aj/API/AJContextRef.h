
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

#ifndef AJContextRef_h
#define AJContextRef_h

#include <AJCore/AJObjectRef.h>
#include <AJCore/AJValueRef.h>
#include <AJCore/WebKitAvailability.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
@function
@abstract Creates a AJ context group.
@discussion A AJContextGroup associates AJ contexts with one another.
 Contexts in the same group may share and exchange AJ objects. Sharing and/or exchanging
 AJ objects between contexts in different groups will produce undefined behavior.
 When objects from the same context group are used in multiple threads, explicit
 synchronization is required.
@result The created AJContextGroup.
*/
JS_EXPORT AJContextGroupRef AJContextGroupCreate() AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
@function
@abstract Retains a AJ context group.
@param group The AJContextGroup to retain.
@result A AJContextGroup that is the same as group.
*/
JS_EXPORT AJContextGroupRef AJContextGroupRetain(AJContextGroupRef group) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
@function
@abstract Releases a AJ context group.
@param group The AJContextGroup to release.
*/
JS_EXPORT void AJContextGroupRelease(AJContextGroupRef group) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
@function
@abstract Creates a global AJ execution context.
@discussion AJGlobalContextCreate allocates a global object and populates it with all the
 built-in AJ objects, such as Object, Function, String, and Array.

 In WebKit version 4.0 and later, the context is created in a unique context group.
 Therefore, scripts may execute in it concurrently with scripts executing in other contexts.
 However, you may not use values created in the context in other contexts.
@param globalObjectClass The class to use when creating the global object. Pass 
 NULL to use the default object class.
@result A AJGlobalContext with a global object of class globalObjectClass.
*/
JS_EXPORT AJGlobalContextRef AJGlobalContextCreate(AJClassRef globalObjectClass) AVAILABLE_WEBKIT_VERSION_3_0_AND_LATER;

/*!
@function
@abstract Creates a global AJ execution context in the context group provided.
@discussion AJGlobalContextCreateInGroup allocates a global object and populates it with
 all the built-in AJ objects, such as Object, Function, String, and Array.
@param globalObjectClass The class to use when creating the global object. Pass
 NULL to use the default object class.
@param group The context group to use. The created global context retains the group.
 Pass NULL to create a unique group for the context.
@result A AJGlobalContext with a global object of class globalObjectClass and a context
 group equal to group.
*/
JS_EXPORT AJGlobalContextRef AJGlobalContextCreateInGroup(AJContextGroupRef group, AJClassRef globalObjectClass) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
@function
@abstract Retains a global AJ execution context.
@param ctx The AJGlobalContext to retain.
@result A AJGlobalContext that is the same as ctx.
*/
JS_EXPORT AJGlobalContextRef AJGlobalContextRetain(AJGlobalContextRef ctx);

/*!
@function
@abstract Releases a global AJ execution context.
@param ctx The AJGlobalContext to release.
*/
JS_EXPORT void AJGlobalContextRelease(AJGlobalContextRef ctx);

/*!
@function
@abstract Gets the global object of a AJ execution context.
@param ctx The AJContext whose global object you want to get.
@result ctx's global object.
*/
JS_EXPORT AJObjectRef AJContextGetGlobalObject(AJContextRef ctx);

/*!
@function
@abstract Gets the context group to which a AJ execution context belongs.
@param ctx The AJContext whose group you want to get.
@result ctx's group.
*/
JS_EXPORT AJContextGroupRef AJContextGetGroup(AJContextRef ctx) AVAILABLE_IN_WEBKIT_VERSION_4_0;

#ifdef __cplusplus
}
#endif

#endif /* AJContextRef_h */
