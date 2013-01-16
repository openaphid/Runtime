
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
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Kelvin W Sherlock (ksherlock@gmail.com)
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

#ifndef AJObjectRef_h
#define AJObjectRef_h

#include <AJCore/AJBase.h>
#include <AJCore/AJValueRef.h>
#include <AJCore/WebKitAvailability.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif

/*!
@enum AJPropertyAttribute
@constant kAJPropertyAttributeNone         Specifies that a property has no special attributes.
@constant kAJPropertyAttributeReadOnly     Specifies that a property is read-only.
@constant kAJPropertyAttributeDontEnum     Specifies that a property should not be enumerated by AJPropertyEnumerators and AJ for...in loops.
@constant kAJPropertyAttributeDontDelete   Specifies that the delete operation should fail on a property.
*/
enum { 
    kAJPropertyAttributeNone         = 0,
    kAJPropertyAttributeReadOnly     = 1 << 1,
    kAJPropertyAttributeDontEnum     = 1 << 2,
    kAJPropertyAttributeDontDelete   = 1 << 3
};

/*! 
@typedef AJPropertyAttributes
@abstract A set of AJPropertyAttributes. Combine multiple attributes by logically ORing them together.
*/
typedef unsigned AJPropertyAttributes;

/*!
@enum AJClassAttribute
@constant kAJClassAttributeNone Specifies that a class has no special attributes.
@constant kAJClassAttributeNoAutomaticPrototype Specifies that a class should not automatically generate a shared prototype for its instance objects. Use kAJClassAttributeNoAutomaticPrototype in combination with AJObjectSetPrototype to manage prototypes manually.
*/
enum { 
    kAJClassAttributeNone = 0,
    kAJClassAttributeNoAutomaticPrototype = 1 << 1
};

/*! 
@typedef AJClassAttributes
@abstract A set of AJClassAttributes. Combine multiple attributes by logically ORing them together.
*/
typedef unsigned AJClassAttributes;

/*! 
@typedef AJObjectInitializeCallback
@abstract The callback invoked when an object is first created.
@param ctx The execution context to use.
@param object The AJObject being created.
@discussion If you named your function Initialize, you would declare it like this:

void Initialize(AJContextRef ctx, AJObjectRef object);

Unlike the other object callbacks, the initialize callback is called on the least
derived class (the parent class) first, and the most derived class last.
*/
typedef void
(*AJObjectInitializeCallback) (AJContextRef ctx, AJObjectRef object);

/*! 
@typedef AJObjectFinalizeCallback
@abstract The callback invoked when an object is finalized (prepared for garbage collection). An object may be finalized on any thread.
@param object The AJObject being finalized.
@discussion If you named your function Finalize, you would declare it like this:

void Finalize(AJObjectRef object);

The finalize callback is called on the most derived class first, and the least 
derived class (the parent class) last.

You must not call any function that may cause a garbage collection or an allocation
of a garbage collected object from within a AJObjectFinalizeCallback. This includes
all functions that have a AJContextRef parameter.
*/
typedef void            
(*AJObjectFinalizeCallback) (AJObjectRef object);

/*! 
@typedef AJObjectHasPropertyCallback
@abstract The callback invoked when determining whether an object has a property.
@param ctx The execution context to use.
@param object The AJObject to search for the property.
@param propertyName A AJString containing the name of the property look up.
@result true if object has the property, otherwise false.
@discussion If you named your function HasProperty, you would declare it like this:

bool HasProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName);

If this function returns false, the hasProperty request forwards to object's statically declared properties, then its parent class chain (which includes the default object class), then its prototype chain.

This callback enables optimization in cases where only a property's existence needs to be known, not its value, and computing its value would be expensive.

If this callback is NULL, the getProperty callback will be used to service hasProperty requests.
*/
typedef bool
(*AJObjectHasPropertyCallback) (AJContextRef ctx, AJObjectRef object, AJStringRef propertyName);

/*! 
@typedef AJObjectGetPropertyCallback
@abstract The callback invoked when getting a property's value.
@param ctx The execution context to use.
@param object The AJObject to search for the property.
@param propertyName A AJString containing the name of the property to get.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result The property's value if object has the property, otherwise NULL.
@discussion If you named your function GetProperty, you would declare it like this:

AJValueRef GetProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef* exception);

If this function returns NULL, the get request forwards to object's statically declared properties, then its parent class chain (which includes the default object class), then its prototype chain.
*/
typedef AJValueRef
(*AJObjectGetPropertyCallback) (AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef* exception);

/*! 
@typedef AJObjectSetPropertyCallback
@abstract The callback invoked when setting a property's value.
@param ctx The execution context to use.
@param object The AJObject on which to set the property's value.
@param propertyName A AJString containing the name of the property to set.
@param value A AJValue to use as the property's value.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result true if the property was set, otherwise false.
@discussion If you named your function SetProperty, you would declare it like this:

bool SetProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef value, AJValueRef* exception);

If this function returns false, the set request forwards to object's statically declared properties, then its parent class chain (which includes the default object class).
*/
typedef bool
(*AJObjectSetPropertyCallback) (AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef value, AJValueRef* exception);

/*! 
@typedef AJObjectDeletePropertyCallback
@abstract The callback invoked when deleting a property.
@param ctx The execution context to use.
@param object The AJObject in which to delete the property.
@param propertyName A AJString containing the name of the property to delete.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result true if propertyName was successfully deleted, otherwise false.
@discussion If you named your function DeleteProperty, you would declare it like this:

bool DeleteProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef* exception);

If this function returns false, the delete request forwards to object's statically declared properties, then its parent class chain (which includes the default object class).
*/
typedef bool
(*AJObjectDeletePropertyCallback) (AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef* exception);

/*! 
@typedef AJObjectGetPropertyNamesCallback
@abstract The callback invoked when collecting the names of an object's properties.
@param ctx The execution context to use.
@param object The AJObject whose property names are being collected.
@param accumulator A AJ property name accumulator in which to accumulate the names of object's properties.
@discussion If you named your function GetPropertyNames, you would declare it like this:

void GetPropertyNames(AJContextRef ctx, AJObjectRef object, AJPropertyNameAccumulatorRef propertyNames);

Property name accumulators are used by AJObjectCopyPropertyNames and AJ for...in loops. 

Use AJPropertyNameAccumulatorAddName to add property names to accumulator. A class's getPropertyNames callback only needs to provide the names of properties that the class vends through a custom getProperty or setProperty callback. Other properties, including statically declared properties, properties vended by other classes, and properties belonging to object's prototype, are added independently.
*/
typedef void
(*AJObjectGetPropertyNamesCallback) (AJContextRef ctx, AJObjectRef object, AJPropertyNameAccumulatorRef propertyNames);

/*! 
@typedef AJObjectCallAsFunctionCallback
@abstract The callback invoked when an object is called as a function.
@param ctx The execution context to use.
@param function A AJObject that is the function being called.
@param thisObject A AJObject that is the 'this' variable in the function's scope.
@param argumentCount An integer count of the number of arguments in arguments.
@param arguments A AJValue array of the  arguments passed to the function.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result A AJValue that is the function's return value.
@discussion If you named your function CallAsFunction, you would declare it like this:

AJValueRef CallAsFunction(AJContextRef ctx, AJObjectRef function, AJObjectRef thisObject, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception);

If your callback were invoked by the AJ expression 'myObject.myFunction()', function would be set to myFunction, and thisObject would be set to myObject.

If this callback is NULL, calling your object as a function will throw an exception.
*/
typedef AJValueRef 
(*AJObjectCallAsFunctionCallback) (AJContextRef ctx, AJObjectRef function, AJObjectRef thisObject, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception);

/*! 
@typedef AJObjectCallAsConstructorCallback
@abstract The callback invoked when an object is used as a constructor in a 'new' expression.
@param ctx The execution context to use.
@param constructor A AJObject that is the constructor being called.
@param argumentCount An integer count of the number of arguments in arguments.
@param arguments A AJValue array of the  arguments passed to the function.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result A AJObject that is the constructor's return value.
@discussion If you named your function CallAsConstructor, you would declare it like this:

AJObjectRef CallAsConstructor(AJContextRef ctx, AJObjectRef constructor, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception);

If your callback were invoked by the AJ expression 'new myConstructor()', constructor would be set to myConstructor.

If this callback is NULL, using your object as a constructor in a 'new' expression will throw an exception.
*/
typedef AJObjectRef 
(*AJObjectCallAsConstructorCallback) (AJContextRef ctx, AJObjectRef constructor, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception);

/*! 
@typedef AJObjectHasInstanceCallback
@abstract hasInstance The callback invoked when an object is used as the target of an 'instanceof' expression.
@param ctx The execution context to use.
@param constructor The AJObject that is the target of the 'instanceof' expression.
@param possibleInstance The AJValue being tested to determine if it is an instance of constructor.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result true if possibleInstance is an instance of constructor, otherwise false.
@discussion If you named your function HasInstance, you would declare it like this:

bool HasInstance(AJContextRef ctx, AJObjectRef constructor, AJValueRef possibleInstance, AJValueRef* exception);

If your callback were invoked by the AJ expression 'someValue instanceof myObject', constructor would be set to myObject and possibleInstance would be set to someValue.

If this callback is NULL, 'instanceof' expressions that target your object will return false.

Standard AJ practice calls for objects that implement the callAsConstructor callback to implement the hasInstance callback as well.
*/
typedef bool 
(*AJObjectHasInstanceCallback)  (AJContextRef ctx, AJObjectRef constructor, AJValueRef possibleInstance, AJValueRef* exception);

/*! 
@typedef AJObjectConvertToTypeCallback
@abstract The callback invoked when converting an object to a particular AJ type.
@param ctx The execution context to use.
@param object The AJObject to convert.
@param type A AJType specifying the AJ type to convert to.
@param exception A pointer to a AJValueRef in which to return an exception, if any.
@result The objects's converted value, or NULL if the object was not converted.
@discussion If you named your function ConvertToType, you would declare it like this:

AJValueRef ConvertToType(AJContextRef ctx, AJObjectRef object, AJType type, AJValueRef* exception);

If this function returns false, the conversion request forwards to object's parent class chain (which includes the default object class).

This function is only invoked when converting an object to number or string. An object converted to boolean is 'true.' An object converted to object is itself.
*/
typedef AJValueRef
(*AJObjectConvertToTypeCallback) (AJContextRef ctx, AJObjectRef object, AJType type, AJValueRef* exception);

/*! 
@struct AJStaticValue
@abstract This structure describes a statically declared value property.
@field name A null-terminated UTF8 string containing the property's name.
@field getProperty A AJObjectGetPropertyCallback to invoke when getting the property's value.
@field setProperty A AJObjectSetPropertyCallback to invoke when setting the property's value. May be NULL if the ReadOnly attribute is set.
@field attributes A logically ORed set of AJPropertyAttributes to give to the property.
*/
typedef struct {
    const char* const name;
    AJObjectGetPropertyCallback getProperty;
    AJObjectSetPropertyCallback setProperty;
    AJPropertyAttributes attributes;
} AJStaticValue;

/*! 
@struct AJStaticFunction
@abstract This structure describes a statically declared function property.
@field name A null-terminated UTF8 string containing the property's name.
@field callAsFunction A AJObjectCallAsFunctionCallback to invoke when the property is called as a function.
@field attributes A logically ORed set of AJPropertyAttributes to give to the property.
*/
typedef struct {
    const char* const name;
    AJObjectCallAsFunctionCallback callAsFunction;
    AJPropertyAttributes attributes;
} AJStaticFunction;

/*!
@struct AJClassDefinition
@abstract This structure contains properties and callbacks that define a type of object. All fields other than the version field are optional. Any pointer may be NULL.
@field version The version number of this structure. The current version is 0.
@field attributes A logically ORed set of AJClassAttributes to give to the class.
@field className A null-terminated UTF8 string containing the class's name.
@field parentClass A AJClass to set as the class's parent class. Pass NULL use the default object class.
@field staticValues A AJStaticValue array containing the class's statically declared value properties. Pass NULL to specify no statically declared value properties. The array must be terminated by a AJStaticValue whose name field is NULL.
@field staticFunctions A AJStaticFunction array containing the class's statically declared function properties. Pass NULL to specify no statically declared function properties. The array must be terminated by a AJStaticFunction whose name field is NULL.
@field initialize The callback invoked when an object is first created. Use this callback to initialize the object.
@field finalize The callback invoked when an object is finalized (prepared for garbage collection). Use this callback to release resources allocated for the object, and perform other cleanup.
@field hasProperty The callback invoked when determining whether an object has a property. If this field is NULL, getProperty is called instead. The hasProperty callback enables optimization in cases where only a property's existence needs to be known, not its value, and computing its value is expensive. 
@field getProperty The callback invoked when getting a property's value.
@field setProperty The callback invoked when setting a property's value.
@field deleteProperty The callback invoked when deleting a property.
@field getPropertyNames The callback invoked when collecting the names of an object's properties.
@field callAsFunction The callback invoked when an object is called as a function.
@field hasInstance The callback invoked when an object is used as the target of an 'instanceof' expression.
@field callAsConstructor The callback invoked when an object is used as a constructor in a 'new' expression.
@field convertToType The callback invoked when converting an object to a particular AJ type.
@discussion The staticValues and staticFunctions arrays are the simplest and most efficient means for vending custom properties. Statically declared properties autmatically service requests like getProperty, setProperty, and getPropertyNames. Property access callbacks are required only to implement unusual properties, like array indexes, whose names are not known at compile-time.

If you named your getter function "GetX" and your setter function "SetX", you would declare a AJStaticValue array containing "X" like this:

AJStaticValue StaticValueArray[] = {
    { "X", GetX, SetX, kAJPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

Standard AJ practice calls for storing function objects in prototypes, so they can be shared. The default AJClass created by AJClassCreate follows this idiom, instantiating objects with a shared, automatically generating prototype containing the class's function objects. The kAJClassAttributeNoAutomaticPrototype attribute specifies that a AJClass should not automatically generate such a prototype. The resulting AJClass instantiates objects with the default object prototype, and gives each instance object its own copy of the class's function objects.

A NULL callback specifies that the default object callback should substitute, except in the case of hasProperty, where it specifies that getProperty should substitute.
*/
typedef struct {
    int                                 version; /* current (and only) version is 0 */
    AJClassAttributes                   attributes;

    const char*                         className;
    AJClassRef                          parentClass;
        
    const AJStaticValue*                staticValues;
    const AJStaticFunction*             staticFunctions;
    
    AJObjectInitializeCallback          initialize;
    AJObjectFinalizeCallback            finalize;
    AJObjectHasPropertyCallback         hasProperty;
    AJObjectGetPropertyCallback         getProperty;
    AJObjectSetPropertyCallback         setProperty;
    AJObjectDeletePropertyCallback      deleteProperty;
    AJObjectGetPropertyNamesCallback    getPropertyNames;
    AJObjectCallAsFunctionCallback      callAsFunction;
    AJObjectCallAsConstructorCallback   callAsConstructor;
    AJObjectHasInstanceCallback         hasInstance;
    AJObjectConvertToTypeCallback       convertToType;
} AJClassDefinition;

/*! 
@const kAJClassDefinitionEmpty 
@abstract A AJClassDefinition structure of the current version, filled with NULL pointers and having no attributes.
@discussion Use this constant as a convenience when creating class definitions. For example, to create a class definition with only a finalize method:

AJClassDefinition definition = kAJClassDefinitionEmpty;
definition.finalize = Finalize;
*/
JS_EXPORT extern const AJClassDefinition kAJClassDefinitionEmpty;

/*!
@function
@abstract Creates a AJ class suitable for use with AJObjectMake.
@param definition A AJClassDefinition that defines the class.
@result A AJClass with the given definition. Ownership follows the Create Rule.
*/
JS_EXPORT AJClassRef AJClassCreate(const AJClassDefinition* definition);

/*!
@function
@abstract Retains a AJ class.
@param jsClass The AJClass to retain.
@result A AJClass that is the same as jsClass.
*/
JS_EXPORT AJClassRef AJClassRetain(AJClassRef jsClass);

/*!
@function
@abstract Releases a AJ class.
@param jsClass The AJClass to release.
*/
JS_EXPORT void AJClassRelease(AJClassRef jsClass);

/*!
@function
@abstract Creates a AJ object.
@param ctx The execution context to use.
@param jsClass The AJClass to assign to the object. Pass NULL to use the default object class.
@param data A void* to set as the object's private data. Pass NULL to specify no private data.
@result A AJObject with the given class and private data.
@discussion The default object class does not allocate storage for private data, so you must provide a non-NULL jsClass to AJObjectMake if you want your object to be able to store private data.

data is set on the created object before the intialize methods in its class chain are called. This enables the initialize methods to retrieve and manipulate data through AJObjectGetPrivate.
*/
JS_EXPORT AJObjectRef AJObjectMake(AJContextRef ctx, AJClassRef jsClass, void* data);

/*!
@function
@abstract Convenience method for creating a AJ function with a given callback as its implementation.
@param ctx The execution context to use.
@param name A AJString containing the function's name. This will be used when converting the function to string. Pass NULL to create an anonymous function.
@param callAsFunction The AJObjectCallAsFunctionCallback to invoke when the function is called.
@result A AJObject that is a function. The object's prototype will be the default function prototype.
*/
JS_EXPORT AJObjectRef AJObjectMakeFunctionWithCallback(AJContextRef ctx, AJStringRef name, AJObjectCallAsFunctionCallback callAsFunction);

/*!
@function
@abstract Convenience method for creating a AJ constructor.
@param ctx The execution context to use.
@param jsClass A AJClass that is the class your constructor will assign to the objects its constructs. jsClass will be used to set the constructor's .prototype property, and to evaluate 'instanceof' expressions. Pass NULL to use the default object class.
@param callAsConstructor A AJObjectCallAsConstructorCallback to invoke when your constructor is used in a 'new' expression. Pass NULL to use the default object constructor.
@result A AJObject that is a constructor. The object's prototype will be the default object prototype.
@discussion The default object constructor takes no arguments and constructs an object of class jsClass with no private data.
*/
JS_EXPORT AJObjectRef AJObjectMakeConstructor(AJContextRef ctx, AJClassRef jsClass, AJObjectCallAsConstructorCallback callAsConstructor);

/*!
 @function
 @abstract Creates a AJ Array object.
 @param ctx The execution context to use.
 @param argumentCount An integer count of the number of arguments in arguments.
 @param arguments A AJValue array of data to populate the Array with. Pass NULL if argumentCount is 0.
 @param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
 @result A AJObject that is an Array.
 @discussion The behavior of this function does not exactly match the behavior of the built-in Array constructor. Specifically, if one argument 
 is supplied, this function returns an array with one element.
 */
JS_EXPORT AJObjectRef AJObjectMakeArray(AJContextRef ctx, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
 @function
 @abstract Creates a AJ Date object, as if by invoking the built-in Date constructor.
 @param ctx The execution context to use.
 @param argumentCount An integer count of the number of arguments in arguments.
 @param arguments A AJValue array of arguments to pass to the Date Constructor. Pass NULL if argumentCount is 0.
 @param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
 @result A AJObject that is a Date.
 */
JS_EXPORT AJObjectRef AJObjectMakeDate(AJContextRef ctx, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
 @function
 @abstract Creates a AJ Error object, as if by invoking the built-in Error constructor.
 @param ctx The execution context to use.
 @param argumentCount An integer count of the number of arguments in arguments.
 @param arguments A AJValue array of arguments to pass to the Error Constructor. Pass NULL if argumentCount is 0.
 @param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
 @result A AJObject that is a Error.
 */
JS_EXPORT AJObjectRef AJObjectMakeError(AJContextRef ctx, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
 @function
 @abstract Creates a AJ RegExp object, as if by invoking the built-in RegExp constructor.
 @param ctx The execution context to use.
 @param argumentCount An integer count of the number of arguments in arguments.
 @param arguments A AJValue array of arguments to pass to the RegExp Constructor. Pass NULL if argumentCount is 0.
 @param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
 @result A AJObject that is a RegExp.
 */
JS_EXPORT AJObjectRef AJObjectMakeRegExp(AJContextRef ctx, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception) AVAILABLE_IN_WEBKIT_VERSION_4_0;

/*!
@function
@abstract Creates a function with a given script as its body.
@param ctx The execution context to use.
@param name A AJString containing the function's name. This will be used when converting the function to string. Pass NULL to create an anonymous function.
@param parameterCount An integer count of the number of parameter names in parameterNames.
@param parameterNames A AJString array containing the names of the function's parameters. Pass NULL if parameterCount is 0.
@param body A AJString containing the script to use as the function's body.
@param sourceURL A AJString containing a URL for the script's source file. This is only used when reporting exceptions. Pass NULL if you do not care to include source file information in exceptions.
@param startingLineNumber An integer value specifying the script's starting line number in the file located at sourceURL. This is only used when reporting exceptions.
@param exception A pointer to a AJValueRef in which to store a syntax error exception, if any. Pass NULL if you do not care to store a syntax error exception.
@result A AJObject that is a function, or NULL if either body or parameterNames contains a syntax error. The object's prototype will be the default function prototype.
@discussion Use this method when you want to execute a script repeatedly, to avoid the cost of re-parsing the script before each execution.
*/
JS_EXPORT AJObjectRef AJObjectMakeFunction(AJContextRef ctx, AJStringRef name, unsigned parameterCount, const AJStringRef parameterNames[], AJStringRef body, AJStringRef sourceURL, int startingLineNumber, AJValueRef* exception);

/*!
@function
@abstract Gets an object's prototype.
@param ctx  The execution context to use.
@param object A AJObject whose prototype you want to get.
@result A AJValue that is the object's prototype.
*/
JS_EXPORT AJValueRef AJObjectGetPrototype(AJContextRef ctx, AJObjectRef object);

/*!
@function
@abstract Sets an object's prototype.
@param ctx  The execution context to use.
@param object The AJObject whose prototype you want to set.
@param value A AJValue to set as the object's prototype.
*/
JS_EXPORT void AJObjectSetPrototype(AJContextRef ctx, AJObjectRef object, AJValueRef value);

/*!
@function
@abstract Tests whether an object has a given property.
@param object The AJObject to test.
@param propertyName A AJString containing the property's name.
@result true if the object has a property whose name matches propertyName, otherwise false.
*/
JS_EXPORT bool AJObjectHasProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName);

/*!
@function
@abstract Gets a property from an object.
@param ctx The execution context to use.
@param object The AJObject whose property you want to get.
@param propertyName A AJString containing the property's name.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result The property's value if object has the property, otherwise the undefined value.
*/
JS_EXPORT AJValueRef AJObjectGetProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef* exception);

/*!
@function
@abstract Sets a property on an object.
@param ctx The execution context to use.
@param object The AJObject whose property you want to set.
@param propertyName A AJString containing the property's name.
@param value A AJValue to use as the property's value.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@param attributes A logically ORed set of AJPropertyAttributes to give to the property.
*/
JS_EXPORT void AJObjectSetProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef value, AJPropertyAttributes attributes, AJValueRef* exception);

/*!
@function
@abstract Deletes a property from an object.
@param ctx The execution context to use.
@param object The AJObject whose property you want to delete.
@param propertyName A AJString containing the property's name.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result true if the delete operation succeeds, otherwise false (for example, if the property has the kAJPropertyAttributeDontDelete attribute set).
*/
JS_EXPORT bool AJObjectDeleteProperty(AJContextRef ctx, AJObjectRef object, AJStringRef propertyName, AJValueRef* exception);

/*!
@function
@abstract Gets a property from an object by numeric index.
@param ctx The execution context to use.
@param object The AJObject whose property you want to get.
@param propertyIndex An integer value that is the property's name.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result The property's value if object has the property, otherwise the undefined value.
@discussion Calling AJObjectGetPropertyAtIndex is equivalent to calling AJObjectGetProperty with a string containing propertyIndex, but AJObjectGetPropertyAtIndex provides optimized access to numeric properties.
*/
JS_EXPORT AJValueRef AJObjectGetPropertyAtIndex(AJContextRef ctx, AJObjectRef object, unsigned propertyIndex, AJValueRef* exception);

/*!
@function
@abstract Sets a property on an object by numeric index.
@param ctx The execution context to use.
@param object The AJObject whose property you want to set.
@param propertyIndex The property's name as a number.
@param value A AJValue to use as the property's value.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@discussion Calling AJObjectSetPropertyAtIndex is equivalent to calling AJObjectSetProperty with a string containing propertyIndex, but AJObjectSetPropertyAtIndex provides optimized access to numeric properties.
*/
JS_EXPORT void AJObjectSetPropertyAtIndex(AJContextRef ctx, AJObjectRef object, unsigned propertyIndex, AJValueRef value, AJValueRef* exception);

/*!
@function
@abstract Gets an object's private data.
@param object A AJObject whose private data you want to get.
@result A void* that is the object's private data, if the object has private data, otherwise NULL.
*/
JS_EXPORT void* AJObjectGetPrivate(AJObjectRef object);

/*!
@function
@abstract Sets a pointer to private data on an object.
@param object The AJObject whose private data you want to set.
@param data A void* to set as the object's private data.
@result true if object can store private data, otherwise false.
@discussion The default object class does not allocate storage for private data. Only objects created with a non-NULL AJClass can store private data.
*/
JS_EXPORT bool AJObjectSetPrivate(AJObjectRef object, void* data);

/*!
@function
@abstract Tests whether an object can be called as a function.
@param ctx  The execution context to use.
@param object The AJObject to test.
@result true if the object can be called as a function, otherwise false.
*/
JS_EXPORT bool AJObjectIsFunction(AJContextRef ctx, AJObjectRef object);

/*!
@function
@abstract Calls an object as a function.
@param ctx The execution context to use.
@param object The AJObject to call as a function.
@param thisObject The object to use as "this," or NULL to use the global object as "this."
@param argumentCount An integer count of the number of arguments in arguments.
@param arguments A AJValue array of arguments to pass to the function. Pass NULL if argumentCount is 0.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result The AJValue that results from calling object as a function, or NULL if an exception is thrown or object is not a function.
*/
JS_EXPORT AJValueRef AJObjectCallAsFunction(AJContextRef ctx, AJObjectRef object, AJObjectRef thisObject, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception);

/*!
@function
@abstract Tests whether an object can be called as a constructor.
@param ctx  The execution context to use.
@param object The AJObject to test.
@result true if the object can be called as a constructor, otherwise false.
*/
JS_EXPORT bool AJObjectIsConstructor(AJContextRef ctx, AJObjectRef object);

/*!
@function
@abstract Calls an object as a constructor.
@param ctx The execution context to use.
@param object The AJObject to call as a constructor.
@param argumentCount An integer count of the number of arguments in arguments.
@param arguments A AJValue array of arguments to pass to the constructor. Pass NULL if argumentCount is 0.
@param exception A pointer to a AJValueRef in which to store an exception, if any. Pass NULL if you do not care to store an exception.
@result The AJObject that results from calling object as a constructor, or NULL if an exception is thrown or object is not a constructor.
*/
JS_EXPORT AJObjectRef AJObjectCallAsConstructor(AJContextRef ctx, AJObjectRef object, size_t argumentCount, const AJValueRef arguments[], AJValueRef* exception);

/*!
@function
@abstract Gets the names of an object's enumerable properties.
@param ctx The execution context to use.
@param object The object whose property names you want to get.
@result A AJPropertyNameArray containing the names object's enumerable properties. Ownership follows the Create Rule.
*/
JS_EXPORT AJPropertyNameArrayRef AJObjectCopyPropertyNames(AJContextRef ctx, AJObjectRef object);

/*!
@function
@abstract Retains a AJ property name array.
@param array The AJPropertyNameArray to retain.
@result A AJPropertyNameArray that is the same as array.
*/
JS_EXPORT AJPropertyNameArrayRef AJPropertyNameArrayRetain(AJPropertyNameArrayRef array);

/*!
@function
@abstract Releases a AJ property name array.
@param array The JSPropetyNameArray to release.
*/
JS_EXPORT void AJPropertyNameArrayRelease(AJPropertyNameArrayRef array);

/*!
@function
@abstract Gets a count of the number of items in a AJ property name array.
@param array The array from which to retrieve the count.
@result An integer count of the number of names in array.
*/
JS_EXPORT size_t AJPropertyNameArrayGetCount(AJPropertyNameArrayRef array);

/*!
@function
@abstract Gets a property name at a given index in a AJ property name array.
@param array The array from which to retrieve the property name.
@param index The index of the property name to retrieve.
@result A AJStringRef containing the property name.
*/
JS_EXPORT AJStringRef AJPropertyNameArrayGetNameAtIndex(AJPropertyNameArrayRef array, size_t index);

/*!
@function
@abstract Adds a property name to a AJ property name accumulator.
@param accumulator The accumulator object to which to add the property name.
@param propertyName The property name to add.
*/
JS_EXPORT void AJPropertyNameAccumulatorAddName(AJPropertyNameAccumulatorRef accumulator, AJStringRef propertyName);

#ifdef __cplusplus
}
#endif

#endif /* AJObjectRef_h */
