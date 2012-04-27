
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
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef AJCallbackObject_h
#define AJCallbackObject_h

#include "AJObjectRef.h"
#include "AJValueRef.h"
#include "AJObject.h"

namespace AJ {

struct AJCallbackObjectData {
    AJCallbackObjectData(void* privateData, AJClassRef jsClass)
        : privateData(privateData)
        , jsClass(jsClass)
    {
        AJClassRetain(jsClass);
    }
    
    ~AJCallbackObjectData()
    {
        AJClassRelease(jsClass);
    }
    
    AJValue getPrivateProperty(const Identifier& propertyName) const
    {
        if (!m_privateProperties)
            return AJValue();
        return m_privateProperties->getPrivateProperty(propertyName);
    }
    
    void setPrivateProperty(const Identifier& propertyName, AJValue value)
    {
        if (!m_privateProperties)
            m_privateProperties.set(new JSPrivatePropertyMap);
        m_privateProperties->setPrivateProperty(propertyName, value);
    }
    
    void deletePrivateProperty(const Identifier& propertyName)
    {
        if (!m_privateProperties)
            return;
        m_privateProperties->deletePrivateProperty(propertyName);
    }

    void markChildren(MarkStack& markStack)
    {
        if (!m_privateProperties)
            return;
        m_privateProperties->markChildren(markStack);
    }

    void* privateData;
    AJClassRef jsClass;
    struct JSPrivatePropertyMap {
        AJValue getPrivateProperty(const Identifier& propertyName) const
        {
            PrivatePropertyMap::const_iterator location = m_propertyMap.find(propertyName.ustring().rep());
            if (location == m_propertyMap.end())
                return AJValue();
            return location->second;
        }
        
        void setPrivateProperty(const Identifier& propertyName, AJValue value)
        {
            m_propertyMap.set(propertyName.ustring().rep(), value);
        }
        
        void deletePrivateProperty(const Identifier& propertyName)
        {
            m_propertyMap.remove(propertyName.ustring().rep());
        }

        void markChildren(MarkStack& markStack)
        {
            for (PrivatePropertyMap::iterator ptr = m_propertyMap.begin(); ptr != m_propertyMap.end(); ++ptr) {
                if (ptr->second)
                    markStack.append(ptr->second);
            }
        }

    private:
        typedef HashMap<RefPtr<UString::Rep>, AJValue, IdentifierRepHash> PrivatePropertyMap;
        PrivatePropertyMap m_propertyMap;
    };
    OwnPtr<JSPrivatePropertyMap> m_privateProperties;
};

    
template <class Base>
class AJCallbackObject : public Base {
public:
    AJCallbackObject(ExecState*, NonNullPassRefPtr<Structure>, AJClassRef, void* data);
    AJCallbackObject(AJClassRef);
    virtual ~AJCallbackObject();

    void setPrivate(void* data);
    void* getPrivate();

    static const ClassInfo info;

    AJClassRef classRef() const { return m_callbackObjectData->jsClass; }
    bool inherits(AJClassRef) const;

    static PassRefPtr<Structure> createStructure(AJValue proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), Base::AnonymousSlotCount); 
    }
    
    AJValue getPrivateProperty(const Identifier& propertyName) const
    {
        return m_callbackObjectData->getPrivateProperty(propertyName);
    }
    
    void setPrivateProperty(const Identifier& propertyName, AJValue value)
    {
        m_callbackObjectData->setPrivateProperty(propertyName, value);
    }
    
    void deletePrivateProperty(const Identifier& propertyName)
    {
        m_callbackObjectData->deletePrivateProperty(propertyName);
    }

protected:
    static const unsigned StructureFlags = OverridesGetOwnPropertySlot | ImplementsHasInstance | OverridesHasInstance | OverridesMarkChildren | OverridesGetPropertyNames | Base::StructureFlags;

private:
    virtual UString className() const;

    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual bool getOwnPropertySlot(ExecState*, unsigned, PropertySlot&);
    virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
    
    virtual void put(ExecState*, const Identifier&, AJValue, PutPropertySlot&);

    virtual bool deleteProperty(ExecState*, const Identifier&);
    virtual bool deleteProperty(ExecState*, unsigned);

    virtual bool hasInstance(ExecState* exec, AJValue value, AJValue proto);

    virtual void getOwnPropertyNames(ExecState*, PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);

    virtual double toNumber(ExecState*) const;
    virtual UString toString(ExecState*) const;

    virtual ConstructType getConstructData(ConstructData&);
    virtual CallType getCallData(CallData&);
    virtual const ClassInfo* classInfo() const { return &info; }

    virtual void markChildren(MarkStack& markStack)
    {
        Base::markChildren(markStack);
        m_callbackObjectData->markChildren(markStack);
    }

    void init(ExecState*);
 
    static AJCallbackObject* asCallbackObject(AJValue);
 
    static AJValue JSC_HOST_CALL call(ExecState*, AJObject* functionObject, AJValue thisValue, const ArgList&);
    static AJObject* construct(ExecState*, AJObject* constructor, const ArgList&);
   
    static AJValue staticValueGetter(ExecState*, AJValue, const Identifier&);
    static AJValue staticFunctionGetter(ExecState*, AJValue, const Identifier&);
    static AJValue callbackGetter(ExecState*, AJValue, const Identifier&);

    OwnPtr<AJCallbackObjectData> m_callbackObjectData;
};

} // namespace AJ

// include the actual template class implementation
#include "AJCallbackObjectFunctions.h"

#endif // AJCallbackObject_h
