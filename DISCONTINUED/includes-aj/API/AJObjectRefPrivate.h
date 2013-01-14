
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
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AJObjectRefPrivate_h
#define AJObjectRefPrivate_h

#include <AJCore/AJObjectRef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 @function
 @abstract Sets a private property on an object.  This private property cannot be accessed from within AJ.
 @param ctx The execution context to use.
 @param object The AJObject whose private property you want to set.
 @param propertyName A AJString containing the property's name.
 @param value A AJValue to use as the property's value.  This may be NULL.
 @result true if object can store private data, otherwise false.
 @discussion This API allows you to store JS values directly an object in a way that will be ensure that they are kept alive without exposing them to AJ code and without introducing the reference cycles that may occur when using AJValueProtect.

 The default object class does not allocate storage for private data. Only objects created with a non-NULL AJClass can store private properties.
 */
JS_EXPORT bool AJObjectSetPrivateProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef value);

/*!
 @function
 @abstract Gets a private property from an object.
 @param ctx The execution context to use.
 @param object The AJObject whose private property you want to get.
 @param propertyName A AJString containing the property's name.
 @result The property's value if object has the property, otherwise NULL.
 */
JS_EXPORT AJValueRef AJObjectGetPrivateProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName);

/*!
 @function
 @abstract Deletes a private property from an object.
 @param ctx The execution context to use.
 @param object The AJObject whose private property you want to delete.
 @param propertyName A AJString containing the property's name.
 @result true if object can store private data, otherwise false.
 @discussion The default object class does not allocate storage for private data. Only objects created with a non-NULL AJClass can store private data.
 */
JS_EXPORT bool AJObjectDeletePrivateProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName);

#ifdef __cplusplus
}
#endif

#endif // AJObjectRefPrivate_h
