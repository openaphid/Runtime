
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
 * Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Eric Seidel <eric@webkit.org>
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

#include "APIShims.h"
#include "APICast.h"
#include "Error.h"
#include "AJCallbackFunction.h"
#include "AJClassRef.h"
#include "AJGlobalObject.h"
#include "AJLock.h"
#include "AJObjectRef.h"
#include "AJString.h"
#include "AJStringRef.h"
#include "OpaqueAJString.h"
#include "PropertyNameArray.h"
#include <wtf/Vector.h>

namespace AJ {

template <class Base>
inline AJCallbackObject<Base>* AJCallbackObject<Base>::asCallbackObject(AJValue value)
{
    ASSERT(asObject(value)->inherits(&info));
    return static_cast<AJCallbackObject*>(asObject(value));
}

template <class Base>
AJCallbackObject<Base>::AJCallbackObject(ExecState* exec, NonNullPassRefPtr<Structure> structure, AJClassRef jsClass, void* data)
    : Base(structure)
    , m_callbackObjectData(new AJCallbackObjectData(data, jsClass))
{
    init(exec);
}

// Global object constructor.
// FIXME: Move this into a separate AJGlobalCallbackObject class derived from this one.
template <class Base>
AJCallbackObject<Base>::AJCallbackObject(AJClassRef jsClass)
    : Base()
    , m_callbackObjectData(new AJCallbackObjectData(0, jsClass))
{
    ASSERT(Base::isGlobalObject());
    init(static_cast<AJGlobalObject*>(this)->globalExec());
}

template <class Base>
void AJCallbackObject<Base>::init(ExecState* exec)
{
    ASSERT(exec);
    
    Vector<AJObjectInitializeCallback, 16> initRoutines;
    AJClassRef jsClass = classRef();
    do {
        if (AJObjectInitializeCallback initialize = jsClass->initialize)
            initRoutines.append(initialize);
    } while ((jsClass = jsClass->parentClass));
    
    // initialize from base to derived
    for (int i = static_cast<int>(initRoutines.size()) - 1; i >= 0; i--) {
        APICallbackShim callbackShim(exec);
        AJObjectInitializeCallback initialize = initRoutines[i];
        initialize(toRef(exec), toRef(this));
    }
}

template <class Base>
AJCallbackObject<Base>::~AJCallbackObject()
{
    AJObjectRef thisRef = toRef(this);
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass)
        if (AJObjectFinalizeCallback finalize = jsClass->finalize)
            finalize(thisRef);
}

template <class Base>
UString AJCallbackObject<Base>::className() const
{
    UString thisClassName = classRef()->className();
    if (!thisClassName.isEmpty())
        return thisClassName;
    
    return Base::className();
}

template <class Base>
bool AJCallbackObject<Base>::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    AJContextRef ctx = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    RefPtr<OpaqueAJString> propertyNameRef;
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        // optional optimization to bypass getProperty in cases when we only need to know if the property exists
        if (AJObjectHasPropertyCallback hasProperty = jsClass->hasProperty) {
            if (!propertyNameRef)
                propertyNameRef = OpaqueAJString::create(propertyName.ustring());
            APICallbackShim callbackShim(exec);
            if (hasProperty(ctx, thisRef, propertyNameRef.get())) {
                slot.setCustom(this, callbackGetter);
                return true;
            }
        } else if (AJObjectGetPropertyCallback getProperty = jsClass->getProperty) {
            if (!propertyNameRef)
                propertyNameRef = OpaqueAJString::create(propertyName.ustring());
            AJValueRef exception = 0;
            AJValueRef value;
            {
                APICallbackShim callbackShim(exec);
                value = getProperty(ctx, thisRef, propertyNameRef.get(), &exception);
            }
            if (exception) {
                exec->setException(toJS(exec, exception));
                slot.setValue(jsUndefined());
                return true;
            }
            if (value) {
                slot.setValue(toJS(exec, value));
                return true;
            }
        }
        
        if (OpaqueAJClassStaticValuesTable* staticValues = jsClass->staticValues(exec)) {
            if (staticValues->contains(propertyName.ustring().rep())) {
                slot.setCustom(this, staticValueGetter);
                return true;
            }
        }
        
        if (OpaqueAJClassStaticFunctionsTable* staticFunctions = jsClass->staticFunctions(exec)) {
            if (staticFunctions->contains(propertyName.ustring().rep())) {
                slot.setCustom(this, staticFunctionGetter);
                return true;
            }
        }
    }
    
    return Base::getOwnPropertySlot(exec, propertyName, slot);
}

template <class Base>
bool AJCallbackObject<Base>::getOwnPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
{
    return getOwnPropertySlot(exec, Identifier::from(exec, propertyName), slot);
}

template <class Base>
bool AJCallbackObject<Base>::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    PropertySlot slot;
    if (getOwnPropertySlot(exec, propertyName, slot)) {
        // Ideally we should return an access descriptor, but returning a value descriptor is better than nothing.
        AJValue value = slot.getValue(exec, propertyName);
        if (!exec->hadException())
            descriptor.setValue(value);
        // We don't know whether the property is configurable, but assume it is.
        descriptor.setConfigurable(true);
        // We don't know whether the property is enumerable (we could call getOwnPropertyNames() to find out), but assume it isn't.
        descriptor.setEnumerable(false);
        return true;
    }

    return Base::getOwnPropertyDescriptor(exec, propertyName, descriptor);
}

template <class Base>
void AJCallbackObject<Base>::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
{
    AJContextRef ctx = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    RefPtr<OpaqueAJString> propertyNameRef;
    AJValueRef valueRef = toRef(exec, value);
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (AJObjectSetPropertyCallback setProperty = jsClass->setProperty) {
            if (!propertyNameRef)
                propertyNameRef = OpaqueAJString::create(propertyName.ustring());
            AJValueRef exception = 0;
            bool result;
            {
                APICallbackShim callbackShim(exec);
                result = setProperty(ctx, thisRef, propertyNameRef.get(), valueRef, &exception);
            }
            if (exception)
                exec->setException(toJS(exec, exception));
            if (result || exception)
                return;
        }
        
        if (OpaqueAJClassStaticValuesTable* staticValues = jsClass->staticValues(exec)) {
            if (StaticValueEntry* entry = staticValues->get(propertyName.ustring().rep())) {
                if (entry->attributes & kAJPropertyAttributeReadOnly)
                    return;
                if (AJObjectSetPropertyCallback setProperty = entry->setProperty) {
                    if (!propertyNameRef)
                        propertyNameRef = OpaqueAJString::create(propertyName.ustring());
                    AJValueRef exception = 0;
                    bool result;
                    {
                        APICallbackShim callbackShim(exec);
                        result = setProperty(ctx, thisRef, propertyNameRef.get(), valueRef, &exception);
                    }
                    if (exception)
                        exec->setException(toJS(exec, exception));
                    if (result || exception)
                        return;
                } else
                    throwError(exec, ReferenceError, "Attempt to set a property that is not settable.");
            }
        }
        
        if (OpaqueAJClassStaticFunctionsTable* staticFunctions = jsClass->staticFunctions(exec)) {
            if (StaticFunctionEntry* entry = staticFunctions->get(propertyName.ustring().rep())) {
                if (entry->attributes & kAJPropertyAttributeReadOnly)
                    return;
                AJCallbackObject<Base>::putDirect(propertyName, value); // put as override property
                return;
            }
        }
    }
    
    return Base::put(exec, propertyName, value, slot);
}

template <class Base>
bool AJCallbackObject<Base>::deleteProperty(ExecState* exec, const Identifier& propertyName)
{
    AJContextRef ctx = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    RefPtr<OpaqueAJString> propertyNameRef;
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (AJObjectDeletePropertyCallback deleteProperty = jsClass->deleteProperty) {
            if (!propertyNameRef)
                propertyNameRef = OpaqueAJString::create(propertyName.ustring());
            AJValueRef exception = 0;
            bool result;
            {
                APICallbackShim callbackShim(exec);
                result = deleteProperty(ctx, thisRef, propertyNameRef.get(), &exception);
            }
            if (exception)
                exec->setException(toJS(exec, exception));
            if (result || exception)
                return true;
        }
        
        if (OpaqueAJClassStaticValuesTable* staticValues = jsClass->staticValues(exec)) {
            if (StaticValueEntry* entry = staticValues->get(propertyName.ustring().rep())) {
                if (entry->attributes & kAJPropertyAttributeDontDelete)
                    return false;
                return true;
            }
        }
        
        if (OpaqueAJClassStaticFunctionsTable* staticFunctions = jsClass->staticFunctions(exec)) {
            if (StaticFunctionEntry* entry = staticFunctions->get(propertyName.ustring().rep())) {
                if (entry->attributes & kAJPropertyAttributeDontDelete)
                    return false;
                return true;
            }
        }
    }
    
    return Base::deleteProperty(exec, propertyName);
}

template <class Base>
bool AJCallbackObject<Base>::deleteProperty(ExecState* exec, unsigned propertyName)
{
    return deleteProperty(exec, Identifier::from(exec, propertyName));
}

template <class Base>
ConstructType AJCallbackObject<Base>::getConstructData(ConstructData& constructData)
{
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (jsClass->callAsConstructor) {
            constructData.native.function = construct;
            return ConstructTypeHost;
        }
    }
    return ConstructTypeNone;
}

template <class Base>
AJObject* AJCallbackObject<Base>::construct(ExecState* exec, AJObject* constructor, const ArgList& args)
{
    AJContextRef execRef = toRef(exec);
    AJObjectRef constructorRef = toRef(constructor);
    
    for (AJClassRef jsClass = static_cast<AJCallbackObject<Base>*>(constructor)->classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (AJObjectCallAsConstructorCallback callAsConstructor = jsClass->callAsConstructor) {
            int argumentCount = static_cast<int>(args.size());
            Vector<AJValueRef, 16> arguments(argumentCount);
            for (int i = 0; i < argumentCount; i++)
                arguments[i] = toRef(exec, args.at(i));
            AJValueRef exception = 0;
            AJObject* result;
            {
                APICallbackShim callbackShim(exec);
                result = toJS(callAsConstructor(execRef, constructorRef, argumentCount, arguments.data(), &exception));
            }
            if (exception)
                exec->setException(toJS(exec, exception));
            return result;
        }
    }
    
    ASSERT_NOT_REACHED(); // getConstructData should prevent us from reaching here
    return 0;
}

template <class Base>
bool AJCallbackObject<Base>::hasInstance(ExecState* exec, AJValue value, AJValue)
{
    AJContextRef execRef = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (AJObjectHasInstanceCallback hasInstance = jsClass->hasInstance) {
            AJValueRef valueRef = toRef(exec, value);
            AJValueRef exception = 0;
            bool result;
            {
                APICallbackShim callbackShim(exec);
                result = hasInstance(execRef, thisRef, valueRef, &exception);
            }
            if (exception)
                exec->setException(toJS(exec, exception));
            return result;
        }
    }
    return false;
}

template <class Base>
CallType AJCallbackObject<Base>::getCallData(CallData& callData)
{
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (jsClass->callAsFunction) {
            callData.native.function = call;
            return CallTypeHost;
        }
    }
    return CallTypeNone;
}

template <class Base>
AJValue AJCallbackObject<Base>::call(ExecState* exec, AJObject* functionObject, AJValue thisValue, const ArgList& args)
{
    AJContextRef execRef = toRef(exec);
    AJObjectRef functionRef = toRef(functionObject);
    AJObjectRef thisObjRef = toRef(thisValue.toThisObject(exec));
    
    for (AJClassRef jsClass = static_cast<AJCallbackObject<Base>*>(functionObject)->classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (AJObjectCallAsFunctionCallback callAsFunction = jsClass->callAsFunction) {
            int argumentCount = static_cast<int>(args.size());
            Vector<AJValueRef, 16> arguments(argumentCount);
            for (int i = 0; i < argumentCount; i++)
                arguments[i] = toRef(exec, args.at(i));
            AJValueRef exception = 0;
            AJValue result;
            {
                APICallbackShim callbackShim(exec);
                result = toJS(exec, callAsFunction(execRef, functionRef, thisObjRef, argumentCount, arguments.data(), &exception));
            }
            if (exception)
                exec->setException(toJS(exec, exception));
            return result;
        }
    }
    
    ASSERT_NOT_REACHED(); // getCallData should prevent us from reaching here
    return AJValue();
}

template <class Base>
void AJCallbackObject<Base>::getOwnPropertyNames(ExecState* exec, PropertyNameArray& propertyNames, EnumerationMode mode)
{
    AJContextRef execRef = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (AJObjectGetPropertyNamesCallback getPropertyNames = jsClass->getPropertyNames) {
            APICallbackShim callbackShim(exec);
            getPropertyNames(execRef, thisRef, toRef(&propertyNames));
        }
        
        if (OpaqueAJClassStaticValuesTable* staticValues = jsClass->staticValues(exec)) {
            typedef OpaqueAJClassStaticValuesTable::const_iterator iterator;
            iterator end = staticValues->end();
            for (iterator it = staticValues->begin(); it != end; ++it) {
                UString::Rep* name = it->first.get();
                StaticValueEntry* entry = it->second;
                if (entry->getProperty && (!(entry->attributes & kAJPropertyAttributeDontEnum) || (mode == IncludeDontEnumProperties)))
                    propertyNames.add(Identifier(exec, name));
            }
        }
        
        if (OpaqueAJClassStaticFunctionsTable* staticFunctions = jsClass->staticFunctions(exec)) {
            typedef OpaqueAJClassStaticFunctionsTable::const_iterator iterator;
            iterator end = staticFunctions->end();
            for (iterator it = staticFunctions->begin(); it != end; ++it) {
                UString::Rep* name = it->first.get();
                StaticFunctionEntry* entry = it->second;
                if (!(entry->attributes & kAJPropertyAttributeDontEnum) || (mode == IncludeDontEnumProperties))
                    propertyNames.add(Identifier(exec, name));
            }
        }
    }
    
    Base::getOwnPropertyNames(exec, propertyNames, mode);
}

template <class Base>
double AJCallbackObject<Base>::toNumber(ExecState* exec) const
{
    // We need this check to guard against the case where this object is rhs of
    // a binary expression where lhs threw an exception in its conversion to
    // primitive
    if (exec->hadException())
        return NaN;
    AJContextRef ctx = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass)
        if (AJObjectConvertToTypeCallback convertToType = jsClass->convertToType) {
            AJValueRef exception = 0;
            AJValueRef value;
            {
                APICallbackShim callbackShim(exec);
                value = convertToType(ctx, thisRef, kAJTypeNumber, &exception);
            }
            if (exception) {
                exec->setException(toJS(exec, exception));
                return 0;
            }

            double dValue;
            if (value)
                return toJS(exec, value).getNumber(dValue) ? dValue : NaN;
        }
            
    return Base::toNumber(exec);
}

template <class Base>
UString AJCallbackObject<Base>::toString(ExecState* exec) const
{
    AJContextRef ctx = toRef(exec);
    AJObjectRef thisRef = toRef(this);
    
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass)
        if (AJObjectConvertToTypeCallback convertToType = jsClass->convertToType) {
            AJValueRef exception = 0;
            AJValueRef value;
            {
                APICallbackShim callbackShim(exec);
                value = convertToType(ctx, thisRef, kAJTypeString, &exception);
            }
            if (exception) {
                exec->setException(toJS(exec, exception));
                return "";
            }
            if (value)
                return toJS(exec, value).getString(exec);
        }
            
    return Base::toString(exec);
}

template <class Base>
void AJCallbackObject<Base>::setPrivate(void* data)
{
    m_callbackObjectData->privateData = data;
}

template <class Base>
void* AJCallbackObject<Base>::getPrivate()
{
    return m_callbackObjectData->privateData;
}

template <class Base>
bool AJCallbackObject<Base>::inherits(AJClassRef c) const
{
    for (AJClassRef jsClass = classRef(); jsClass; jsClass = jsClass->parentClass)
        if (jsClass == c)
            return true;
    
    return false;
}

template <class Base>
AJValue AJCallbackObject<Base>::staticValueGetter(ExecState* exec, AJValue slotBase, const Identifier& propertyName)
{
    AJCallbackObject* thisObj = asCallbackObject(slotBase);
    
    AJObjectRef thisRef = toRef(thisObj);
    RefPtr<OpaqueAJString> propertyNameRef;
    
    for (AJClassRef jsClass = thisObj->classRef(); jsClass; jsClass = jsClass->parentClass)
        if (OpaqueAJClassStaticValuesTable* staticValues = jsClass->staticValues(exec))
            if (StaticValueEntry* entry = staticValues->get(propertyName.ustring().rep()))
                if (AJObjectGetPropertyCallback getProperty = entry->getProperty) {
                    if (!propertyNameRef)
                        propertyNameRef = OpaqueAJString::create(propertyName.ustring());
                    AJValueRef exception = 0;
                    AJValueRef value;
                    {
                        APICallbackShim callbackShim(exec);
                        value = getProperty(toRef(exec), thisRef, propertyNameRef.get(), &exception);
                    }
                    if (exception) {
                        exec->setException(toJS(exec, exception));
                        return jsUndefined();
                    }
                    if (value)
                        return toJS(exec, value);
                }

    return throwError(exec, ReferenceError, "Static value property defined with NULL getProperty callback.");
}

template <class Base>
AJValue AJCallbackObject<Base>::staticFunctionGetter(ExecState* exec, AJValue slotBase, const Identifier& propertyName)
{
    AJCallbackObject* thisObj = asCallbackObject(slotBase);
    
    // Check for cached or override property.
    PropertySlot slot2(thisObj);
    if (thisObj->Base::getOwnPropertySlot(exec, propertyName, slot2))
        return slot2.getValue(exec, propertyName);
    
    for (AJClassRef jsClass = thisObj->classRef(); jsClass; jsClass = jsClass->parentClass) {
        if (OpaqueAJClassStaticFunctionsTable* staticFunctions = jsClass->staticFunctions(exec)) {
            if (StaticFunctionEntry* entry = staticFunctions->get(propertyName.ustring().rep())) {
                if (AJObjectCallAsFunctionCallback callAsFunction = entry->callAsFunction) {
                    AJObject* o = new (exec) AJCallbackFunction(exec, callAsFunction, propertyName);
                    thisObj->putDirect(propertyName, o, entry->attributes);
                    return o;
                }
            }
        }
    }
    
    return throwError(exec, ReferenceError, "Static function property defined with NULL callAsFunction callback.");
}

template <class Base>
AJValue AJCallbackObject<Base>::callbackGetter(ExecState* exec, AJValue slotBase, const Identifier& propertyName)
{
    AJCallbackObject* thisObj = asCallbackObject(slotBase);
    
    AJObjectRef thisRef = toRef(thisObj);
    RefPtr<OpaqueAJString> propertyNameRef;
    
    for (AJClassRef jsClass = thisObj->classRef(); jsClass; jsClass = jsClass->parentClass)
        if (AJObjectGetPropertyCallback getProperty = jsClass->getProperty) {
            if (!propertyNameRef)
                propertyNameRef = OpaqueAJString::create(propertyName.ustring());
            AJValueRef exception = 0;
            AJValueRef value;
            {
                APICallbackShim callbackShim(exec);
                value = getProperty(toRef(exec), thisRef, propertyNameRef.get(), &exception);
            }
            if (exception) {
                exec->setException(toJS(exec, exception));
                return jsUndefined();
            }
            if (value)
                return toJS(exec, value);
        }
            
    return throwError(exec, ReferenceError, "hasProperty callback returned true for a property that doesn't exist.");
}

} // namespace AJ
