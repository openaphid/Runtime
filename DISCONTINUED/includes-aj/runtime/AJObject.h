
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
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef AJObject_h
#define AJObject_h

#include "ArgList.h"
#include "ClassInfo.h"
#include "CommonIdentifiers.h"
#include "Completion.h"
#include "CallFrame.h"
#include "AJCell.h"
#include "JSNumberCell.h"
#include "MarkStack.h"
#include "PropertySlot.h"
#include "PutPropertySlot.h"
#include "ScopeChain.h"
#include "Structure.h"
#include "AJGlobalData.h"
#include "AJString.h"
#include <wtf/StdLibExtras.h>

namespace AJ {

    inline AJCell* getAJFunction(AJGlobalData& globalData, AJValue value)
    {
        if (value.isCell() && (value.asCell()->vptr() == globalData.jsFunctionVPtr))
            return value.asCell();
        return 0;
    }
    
    class HashEntry;
    class InternalFunction;
    class PropertyDescriptor;
    class PropertyNameArray;
    class Structure;
    struct HashTable;

    // ECMA 262-3 8.6.1
    // Property attributes
    enum Attribute {
        None         = 0,
        ReadOnly     = 1 << 1,  // property can be only read, not written
        DontEnum     = 1 << 2,  // property doesn't appear in (for .. in ..)
        DontDelete   = 1 << 3,  // property can't be deleted
        Function     = 1 << 4,  // property is a function - only used by static hashtables
        Getter       = 1 << 5,  // property is a getter
        Setter       = 1 << 6   // property is a setter
    };

    typedef EncodedAJValue* PropertyStorage;
    typedef const EncodedAJValue* ConstPropertyStorage;

    class AJObject : public AJCell {
        friend class BatchedTransitionOptimizer;
        friend class JIT;
        friend class AJCell;

    public:
        explicit AJObject(NonNullPassRefPtr<Structure>);

        virtual void markChildren(MarkStack&);
        ALWAYS_INLINE void markChildrenDirect(MarkStack& markStack);

        // The inline virtual destructor cannot be the first virtual function declared
        // in the class as it results in the vtable being generated as a weak symbol
        virtual ~AJObject();

        AJValue prototype() const;
        void setPrototype(AJValue prototype);
        
        void setStructure(NonNullPassRefPtr<Structure>);
        Structure* inheritorID();

        virtual UString className() const;

        AJValue get(ExecState*, const Identifier& propertyName) const;
        AJValue get(ExecState*, unsigned propertyName) const;

        bool getPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        bool getPropertySlot(ExecState*, unsigned propertyName, PropertySlot&);
        bool getPropertyDescriptor(ExecState*, const Identifier& propertyName, PropertyDescriptor&);

        virtual bool getOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);
        virtual bool getOwnPropertySlot(ExecState*, unsigned propertyName, PropertySlot&);
        virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);

        virtual void put(ExecState*, const Identifier& propertyName, AJValue value, PutPropertySlot&);
        virtual void put(ExecState*, unsigned propertyName, AJValue value);

        virtual void putWithAttributes(ExecState*, const Identifier& propertyName, AJValue value, unsigned attributes, bool checkReadOnly, PutPropertySlot& slot);
        virtual void putWithAttributes(ExecState*, const Identifier& propertyName, AJValue value, unsigned attributes);
        virtual void putWithAttributes(ExecState*, unsigned propertyName, AJValue value, unsigned attributes);

        bool propertyIsEnumerable(ExecState*, const Identifier& propertyName) const;

        bool hasProperty(ExecState*, const Identifier& propertyName) const;
        bool hasProperty(ExecState*, unsigned propertyName) const;
        bool hasOwnProperty(ExecState*, const Identifier& propertyName) const;

        virtual bool deleteProperty(ExecState*, const Identifier& propertyName);
        virtual bool deleteProperty(ExecState*, unsigned propertyName);

        virtual AJValue defaultValue(ExecState*, PreferredPrimitiveType) const;

        virtual bool hasInstance(ExecState*, AJValue, AJValue prototypeProperty);

        virtual void getPropertyNames(ExecState*, PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);
        virtual void getOwnPropertyNames(ExecState*, PropertyNameArray&, EnumerationMode mode = ExcludeDontEnumProperties);

        virtual AJValue toPrimitive(ExecState*, PreferredPrimitiveType = NoPreference) const;
        virtual bool getPrimitiveNumber(ExecState*, double& number, AJValue& value);
        virtual bool toBoolean(ExecState*) const;
        virtual double toNumber(ExecState*) const;
        virtual UString toString(ExecState*) const;
        virtual AJObject* toObject(ExecState*) const;

        virtual AJObject* toThisObject(ExecState*) const;
        virtual AJObject* unwrappedObject();

        bool getPropertySpecificValue(ExecState* exec, const Identifier& propertyName, AJCell*& specificFunction) const;

        // This get function only looks at the property map.
        AJValue getDirect(const Identifier& propertyName) const
        {
            size_t offset = m_structure->get(propertyName);
            return offset != ATF::notFound ? getDirectOffset(offset) : AJValue();
        }

        AJValue* getDirectLocation(const Identifier& propertyName)
        {
            size_t offset = m_structure->get(propertyName);
            return offset != ATF::notFound ? locationForOffset(offset) : 0;
        }

        AJValue* getDirectLocation(const Identifier& propertyName, unsigned& attributes)
        {
            AJCell* specificFunction;
            size_t offset = m_structure->get(propertyName, attributes, specificFunction);
            return offset != ATF::notFound ? locationForOffset(offset) : 0;
        }

        size_t offsetForLocation(AJValue* location) const
        {
            return location - reinterpret_cast<const AJValue*>(propertyStorage());
        }

        void transitionTo(Structure*);

        void removeDirect(const Identifier& propertyName);
        bool hasCustomProperties() { return !m_structure->isEmpty(); }
        bool hasGetterSetterProperties() { return m_structure->hasGetterSetterProperties(); }

        void putDirect(const Identifier& propertyName, AJValue value, unsigned attr, bool checkReadOnly, PutPropertySlot& slot);
        void putDirect(const Identifier& propertyName, AJValue value, unsigned attr = 0);
        void putDirect(const Identifier& propertyName, AJValue value, PutPropertySlot&);

        void putDirectFunction(const Identifier& propertyName, AJCell* value, unsigned attr = 0);
        void putDirectFunction(const Identifier& propertyName, AJCell* value, unsigned attr, bool checkReadOnly, PutPropertySlot& slot);
        void putDirectFunction(ExecState* exec, InternalFunction* function, unsigned attr = 0);

        void putDirectWithoutTransition(const Identifier& propertyName, AJValue value, unsigned attr = 0);
        void putDirectFunctionWithoutTransition(const Identifier& propertyName, AJCell* value, unsigned attr = 0);
        void putDirectFunctionWithoutTransition(ExecState* exec, InternalFunction* function, unsigned attr = 0);

        // Fast access to known property offsets.
        AJValue getDirectOffset(size_t offset) const { return AJValue::decode(propertyStorage()[offset]); }
        void putDirectOffset(size_t offset, AJValue value) { propertyStorage()[offset] = AJValue::encode(value); }

        void fillGetterPropertySlot(PropertySlot&, AJValue* location);

        virtual void defineGetter(ExecState*, const Identifier& propertyName, AJObject* getterFunction, unsigned attributes = 0);
        virtual void defineSetter(ExecState*, const Identifier& propertyName, AJObject* setterFunction, unsigned attributes = 0);
        virtual AJValue lookupGetter(ExecState*, const Identifier& propertyName);
        virtual AJValue lookupSetter(ExecState*, const Identifier& propertyName);
        virtual bool defineOwnProperty(ExecState*, const Identifier& propertyName, PropertyDescriptor&, bool shouldThrow);

        virtual bool isGlobalObject() const { return false; }
        virtual bool isVariableObject() const { return false; }
        virtual bool isActivationObject() const { return false; }
        virtual bool isNotAnObjectErrorStub() const { return false; }

        virtual ComplType exceptionType() const { return Throw; }

        void allocatePropertyStorage(size_t oldSize, size_t newSize);
        void allocatePropertyStorageInline(size_t oldSize, size_t newSize);
        bool isUsingInlineStorage() const { return m_structure->isUsingInlineStorage(); }

        static const unsigned inlineStorageCapacity = sizeof(EncodedAJValue) == 2 * sizeof(void*) ? 4 : 3;
        static const unsigned nonInlineBaseStorageCapacity = 16;

        static PassRefPtr<Structure> createStructure(AJValue prototype)
        {
            return Structure::create(prototype, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount);
        }

        void flattenDictionaryObject()
        {
            m_structure->flattenDictionaryStructure(this);
        }

    protected:
        static const unsigned StructureFlags = 0;

        void putAnonymousValue(unsigned index, AJValue value)
        {
            ASSERT(index < m_structure->anonymousSlotCount());
            *locationForOffset(index) = value;
        }
        AJValue getAnonymousValue(unsigned index) const
        {
            ASSERT(index < m_structure->anonymousSlotCount());
            return *locationForOffset(index);
        }

    private:
        // Nobody should ever ask any of these questions on something already known to be a AJObject.
        using AJCell::isAPIValueWrapper;
        using AJCell::isGetterSetter;
        using AJCell::toObject;
        void getObject();
        void getString(ExecState* exec);
        void isObject();
        void isString();
#if USE(JSVALUE32)
        void isNumber();
#endif

        ConstPropertyStorage propertyStorage() const { return (isUsingInlineStorage() ? m_inlineStorage : m_externalStorage); }
        PropertyStorage propertyStorage() { return (isUsingInlineStorage() ? m_inlineStorage : m_externalStorage); }

        const AJValue* locationForOffset(size_t offset) const
        {
            return reinterpret_cast<const AJValue*>(&propertyStorage()[offset]);
        }

        AJValue* locationForOffset(size_t offset)
        {
            return reinterpret_cast<AJValue*>(&propertyStorage()[offset]);
        }

        void putDirectInternal(const Identifier& propertyName, AJValue value, unsigned attr, bool checkReadOnly, PutPropertySlot& slot, AJCell*);
        void putDirectInternal(AJGlobalData&, const Identifier& propertyName, AJValue value, unsigned attr, bool checkReadOnly, PutPropertySlot& slot);
        void putDirectInternal(AJGlobalData&, const Identifier& propertyName, AJValue value, unsigned attr = 0);

        bool inlineGetOwnPropertySlot(ExecState*, const Identifier& propertyName, PropertySlot&);

        const HashEntry* findPropertyHashEntry(ExecState*, const Identifier& propertyName) const;
        Structure* createInheritorID();

        union {
            PropertyStorage m_externalStorage;
            EncodedAJValue m_inlineStorage[inlineStorageCapacity];
        };

        RefPtr<Structure> m_inheritorID;
    };
    
inline AJObject* asObject(AJCell* cell)
{
    ASSERT(cell->isObject());
    return static_cast<AJObject*>(cell);
}

inline AJObject* asObject(AJValue value)
{
    return asObject(value.asCell());
}

inline AJObject::AJObject(NonNullPassRefPtr<Structure> structure)
    : AJCell(structure.releaseRef()) // ~AJObject balances this ref()
{
    ASSERT(m_structure->propertyStorageCapacity() == inlineStorageCapacity);
    ASSERT(m_structure->isEmpty());
    ASSERT(prototype().isNull() || Heap::heap(this) == Heap::heap(prototype()));
#if USE(JSVALUE64) || USE(JSVALUE32_64)
    ASSERT(OBJECT_OFFSETOF(AJObject, m_inlineStorage) % sizeof(double) == 0);
#endif
}

inline AJObject::~AJObject()
{
    ASSERT(m_structure);
    if (!isUsingInlineStorage())
        delete [] m_externalStorage;
    m_structure->deref();
}

inline AJValue AJObject::prototype() const
{
    return m_structure->storedPrototype();
}

inline void AJObject::setPrototype(AJValue prototype)
{
    ASSERT(prototype);
    RefPtr<Structure> newStructure = Structure::changePrototypeTransition(m_structure, prototype);
    setStructure(newStructure.release());
}

inline void AJObject::setStructure(NonNullPassRefPtr<Structure> structure)
{
    m_structure->deref();
    m_structure = structure.releaseRef(); // ~AJObject balances this ref()
}

inline Structure* AJObject::inheritorID()
{
    if (m_inheritorID)
        return m_inheritorID.get();
    return createInheritorID();
}

inline bool Structure::isUsingInlineStorage() const
{
    return (propertyStorageCapacity() == AJObject::inlineStorageCapacity);
}

inline bool AJCell::inherits(const ClassInfo* info) const
{
    for (const ClassInfo* ci = classInfo(); ci; ci = ci->parentClass) {
        if (ci == info)
            return true;
    }
    return false;
}

// this method is here to be after the inline declaration of AJCell::inherits
inline bool AJValue::inherits(const ClassInfo* classInfo) const
{
    return isCell() && asCell()->inherits(classInfo);
}

ALWAYS_INLINE bool AJObject::inlineGetOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (AJValue* location = getDirectLocation(propertyName)) {
        if (m_structure->hasGetterSetterProperties() && location[0].isGetterSetter())
            fillGetterPropertySlot(slot, location);
        else
            slot.setValueSlot(this, location, offsetForLocation(location));
        return true;
    }

    // non-standard Netscape extension
    if (propertyName == exec->propertyNames().underscoreProto) {
        slot.setValue(prototype());
        return true;
    }

    return false;
}

// It may seem crazy to inline a function this large, especially a virtual function,
// but it makes a big difference to property lookup that derived classes can inline their
// base class call to this.
ALWAYS_INLINE bool AJObject::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return inlineGetOwnPropertySlot(exec, propertyName, slot);
}

ALWAYS_INLINE bool AJCell::fastGetOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (!structure()->typeInfo().overridesGetOwnPropertySlot())
        return asObject(this)->inlineGetOwnPropertySlot(exec, propertyName, slot);
    return getOwnPropertySlot(exec, propertyName, slot);
}

// It may seem crazy to inline a function this large but it makes a big difference
// since this is function very hot in variable lookup
ALWAYS_INLINE bool AJObject::getPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    AJObject* object = this;
    while (true) {
        if (object->fastGetOwnPropertySlot(exec, propertyName, slot))
            return true;
        AJValue prototype = object->prototype();
        if (!prototype.isObject())
            return false;
        object = asObject(prototype);
    }
}

ALWAYS_INLINE bool AJObject::getPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
{
    AJObject* object = this;
    while (true) {
        if (object->getOwnPropertySlot(exec, propertyName, slot))
            return true;
        AJValue prototype = object->prototype();
        if (!prototype.isObject())
            return false;
        object = asObject(prototype);
    }
}

inline AJValue AJObject::get(ExecState* exec, const Identifier& propertyName) const
{
    PropertySlot slot(this);
    if (const_cast<AJObject*>(this)->getPropertySlot(exec, propertyName, slot))
        return slot.getValue(exec, propertyName);
    
    return jsUndefined();
}

inline AJValue AJObject::get(ExecState* exec, unsigned propertyName) const
{
    PropertySlot slot(this);
    if (const_cast<AJObject*>(this)->getPropertySlot(exec, propertyName, slot))
        return slot.getValue(exec, propertyName);

    return jsUndefined();
}

inline void AJObject::putDirectInternal(const Identifier& propertyName, AJValue value, unsigned attributes, bool checkReadOnly, PutPropertySlot& slot, AJCell* specificFunction)
{
    ASSERT(value);
    ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

    if (m_structure->isDictionary()) {
        unsigned currentAttributes;
        AJCell* currentSpecificFunction;
        size_t offset = m_structure->get(propertyName, currentAttributes, currentSpecificFunction);
        if (offset != ATF::notFound) {
            // If there is currently a specific function, and there now either isn't,
            // or the new value is different, then despecify.
            if (currentSpecificFunction && (specificFunction != currentSpecificFunction))
                m_structure->despecifyDictionaryFunction(propertyName);
            if (checkReadOnly && currentAttributes & ReadOnly)
                return;
            putDirectOffset(offset, value);
            // At this point, the objects structure only has a specific value set if previously there
            // had been one set, and if the new value being specified is the same (otherwise we would
            // have despecified, above).  So, if currentSpecificFunction is not set, or if the new
            // value is different (or there is no new value), then the slot now has no value - and
            // as such it is cachable.
            // If there was previously a value, and the new value is the same, then we cannot cache.
            if (!currentSpecificFunction || (specificFunction != currentSpecificFunction))
                slot.setExistingProperty(this, offset);
            return;
        }

        size_t currentCapacity = m_structure->propertyStorageCapacity();
        offset = m_structure->addPropertyWithoutTransition(propertyName, attributes, specificFunction);
        if (currentCapacity != m_structure->propertyStorageCapacity())
            allocatePropertyStorage(currentCapacity, m_structure->propertyStorageCapacity());

        ASSERT(offset < m_structure->propertyStorageCapacity());
        putDirectOffset(offset, value);
        // See comment on setNewProperty call below.
        if (!specificFunction)
            slot.setNewProperty(this, offset);
        return;
    }

    size_t offset;
    size_t currentCapacity = m_structure->propertyStorageCapacity();
    if (RefPtr<Structure> structure = Structure::addPropertyTransitionToExistingStructure(m_structure, propertyName, attributes, specificFunction, offset)) {    
        if (currentCapacity != structure->propertyStorageCapacity())
            allocatePropertyStorage(currentCapacity, structure->propertyStorageCapacity());

        ASSERT(offset < structure->propertyStorageCapacity());
        setStructure(structure.release());
        putDirectOffset(offset, value);
        // This is a new property; transitions with specific values are not currently cachable,
        // so leave the slot in an uncachable state.
        if (!specificFunction)
            slot.setNewProperty(this, offset);
        return;
    }

    unsigned currentAttributes;
    AJCell* currentSpecificFunction;
    offset = m_structure->get(propertyName, currentAttributes, currentSpecificFunction);
    if (offset != ATF::notFound) {
        if (checkReadOnly && currentAttributes & ReadOnly)
            return;

        // There are three possibilities here:
        //  (1) There is an existing specific value set, and we're overwriting with *the same value*.
        //       * Do nothing - no need to despecify, but that means we can't cache (a cached
        //         put could write a different value). Leave the slot in an uncachable state.
        //  (2) There is a specific value currently set, but we're writing a different value.
        //       * First, we have to despecify.  Having done so, this is now a regular slot
        //         with no specific value, so go ahead & cache like normal.
        //  (3) Normal case, there is no specific value set.
        //       * Go ahead & cache like normal.
        if (currentSpecificFunction) {
            // case (1) Do the put, then return leaving the slot uncachable.
            if (specificFunction == currentSpecificFunction) {
                putDirectOffset(offset, value);
                return;
            }
            // case (2) Despecify, fall through to (3).
            setStructure(Structure::despecifyFunctionTransition(m_structure, propertyName));
        }

        // case (3) set the slot, do the put, return.
        slot.setExistingProperty(this, offset);
        putDirectOffset(offset, value);
        return;
    }

    // If we have a specific function, we may have got to this point if there is
    // already a transition with the correct property name and attributes, but
    // specialized to a different function.  In this case we just want to give up
    // and despecialize the transition.
    // In this case we clear the value of specificFunction which will result
    // in us adding a non-specific transition, and any subsequent lookup in
    // Structure::addPropertyTransitionToExistingStructure will just use that.
    if (specificFunction && m_structure->hasTransition(propertyName, attributes))
        specificFunction = 0;

    RefPtr<Structure> structure = Structure::addPropertyTransition(m_structure, propertyName, attributes, specificFunction, offset);

    if (currentCapacity != structure->propertyStorageCapacity())
        allocatePropertyStorage(currentCapacity, structure->propertyStorageCapacity());

    ASSERT(offset < structure->propertyStorageCapacity());
    setStructure(structure.release());
    putDirectOffset(offset, value);
    // This is a new property; transitions with specific values are not currently cachable,
    // so leave the slot in an uncachable state.
    if (!specificFunction)
        slot.setNewProperty(this, offset);
}

inline void AJObject::putDirectInternal(AJGlobalData& globalData, const Identifier& propertyName, AJValue value, unsigned attributes, bool checkReadOnly, PutPropertySlot& slot)
{
    ASSERT(value);
    ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

    putDirectInternal(propertyName, value, attributes, checkReadOnly, slot, getAJFunction(globalData, value));
}

inline void AJObject::putDirectInternal(AJGlobalData& globalData, const Identifier& propertyName, AJValue value, unsigned attributes)
{
    PutPropertySlot slot;
    putDirectInternal(propertyName, value, attributes, false, slot, getAJFunction(globalData, value));
}

inline void AJObject::putDirect(const Identifier& propertyName, AJValue value, unsigned attributes, bool checkReadOnly, PutPropertySlot& slot)
{
    ASSERT(value);
    ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

    putDirectInternal(propertyName, value, attributes, checkReadOnly, slot, 0);
}

inline void AJObject::putDirect(const Identifier& propertyName, AJValue value, unsigned attributes)
{
    PutPropertySlot slot;
    putDirectInternal(propertyName, value, attributes, false, slot, 0);
}

inline void AJObject::putDirect(const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
{
    putDirectInternal(propertyName, value, 0, false, slot, 0);
}

inline void AJObject::putDirectFunction(const Identifier& propertyName, AJCell* value, unsigned attributes, bool checkReadOnly, PutPropertySlot& slot)
{
    putDirectInternal(propertyName, value, attributes, checkReadOnly, slot, value);
}

inline void AJObject::putDirectFunction(const Identifier& propertyName, AJCell* value, unsigned attr)
{
    PutPropertySlot slot;
    putDirectInternal(propertyName, value, attr, false, slot, value);
}

inline void AJObject::putDirectWithoutTransition(const Identifier& propertyName, AJValue value, unsigned attributes)
{
    size_t currentCapacity = m_structure->propertyStorageCapacity();
    size_t offset = m_structure->addPropertyWithoutTransition(propertyName, attributes, 0);
    if (currentCapacity != m_structure->propertyStorageCapacity())
        allocatePropertyStorage(currentCapacity, m_structure->propertyStorageCapacity());
    putDirectOffset(offset, value);
}

inline void AJObject::putDirectFunctionWithoutTransition(const Identifier& propertyName, AJCell* value, unsigned attributes)
{
    size_t currentCapacity = m_structure->propertyStorageCapacity();
    size_t offset = m_structure->addPropertyWithoutTransition(propertyName, attributes, value);
    if (currentCapacity != m_structure->propertyStorageCapacity())
        allocatePropertyStorage(currentCapacity, m_structure->propertyStorageCapacity());
    putDirectOffset(offset, value);
}

inline void AJObject::transitionTo(Structure* newStructure)
{
    if (m_structure->propertyStorageCapacity() != newStructure->propertyStorageCapacity())
        allocatePropertyStorage(m_structure->propertyStorageCapacity(), newStructure->propertyStorageCapacity());
    setStructure(newStructure);
}

inline AJValue AJObject::toPrimitive(ExecState* exec, PreferredPrimitiveType preferredType) const
{
    return defaultValue(exec, preferredType);
}

inline AJValue AJValue::get(ExecState* exec, const Identifier& propertyName) const
{
    PropertySlot slot(asValue());
    return get(exec, propertyName, slot);
}

inline AJValue AJValue::get(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) const
{
    if (UNLIKELY(!isCell())) {
        AJObject* prototype = synthesizePrototype(exec);
        if (propertyName == exec->propertyNames().underscoreProto)
            return prototype;
        if (!prototype->getPropertySlot(exec, propertyName, slot))
            return jsUndefined();
        return slot.getValue(exec, propertyName);
    }
    AJCell* cell = asCell();
    while (true) {
        if (cell->fastGetOwnPropertySlot(exec, propertyName, slot))
            return slot.getValue(exec, propertyName);
        AJValue prototype = asObject(cell)->prototype();
        if (!prototype.isObject())
            return jsUndefined();
        cell = asObject(prototype);
    }
}

inline AJValue AJValue::get(ExecState* exec, unsigned propertyName) const
{
    PropertySlot slot(asValue());
    return get(exec, propertyName, slot);
}

inline AJValue AJValue::get(ExecState* exec, unsigned propertyName, PropertySlot& slot) const
{
    if (UNLIKELY(!isCell())) {
        AJObject* prototype = synthesizePrototype(exec);
        if (!prototype->getPropertySlot(exec, propertyName, slot))
            return jsUndefined();
        return slot.getValue(exec, propertyName);
    }
    AJCell* cell = const_cast<AJCell*>(asCell());
    while (true) {
        if (cell->getOwnPropertySlot(exec, propertyName, slot))
            return slot.getValue(exec, propertyName);
        AJValue prototype = asObject(cell)->prototype();
        if (!prototype.isObject())
            return jsUndefined();
        cell = prototype.asCell();
    }
}

inline void AJValue::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
{
    if (UNLIKELY(!isCell())) {
        synthesizeObject(exec)->put(exec, propertyName, value, slot);
        return;
    }
    asCell()->put(exec, propertyName, value, slot);
}

inline void AJValue::putDirect(ExecState*, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
{
    ASSERT(isCell() && isObject());
    asObject(asCell())->putDirect(propertyName, value, slot);
}

inline void AJValue::put(ExecState* exec, unsigned propertyName, AJValue value)
{
    if (UNLIKELY(!isCell())) {
        synthesizeObject(exec)->put(exec, propertyName, value);
        return;
    }
    asCell()->put(exec, propertyName, value);
}

ALWAYS_INLINE void AJObject::allocatePropertyStorageInline(size_t oldSize, size_t newSize)
{
    ASSERT(newSize > oldSize);

    // It's important that this function not rely on m_structure, since
    // we might be in the middle of a transition.
    bool wasInline = (oldSize == AJObject::inlineStorageCapacity);

    PropertyStorage oldPropertyStorage = (wasInline ? m_inlineStorage : m_externalStorage);
    PropertyStorage newPropertyStorage = new EncodedAJValue[newSize];

    for (unsigned i = 0; i < oldSize; ++i)
       newPropertyStorage[i] = oldPropertyStorage[i];

    if (!wasInline)
        delete [] oldPropertyStorage;

    m_externalStorage = newPropertyStorage;
}

ALWAYS_INLINE void AJObject::markChildrenDirect(MarkStack& markStack)
{
    AJCell::markChildren(markStack);

    markStack.append(prototype());
    
    PropertyStorage storage = propertyStorage();
    size_t storageSize = m_structure->propertyStorageSize();
    markStack.appendValues(reinterpret_cast<AJValue*>(storage), storageSize);
}

// --- AJValue inlines ----------------------------

ALWAYS_INLINE UString AJValue::toThisString(ExecState* exec) const
{
    return isString() ? static_cast<AJString*>(asCell())->value(exec) : toThisObject(exec)->toString(exec);
}

inline AJString* AJValue::toThisAJString(ExecState* exec) const
{
    return isString() ? static_cast<AJString*>(asCell()) : jsString(exec, toThisObject(exec)->toString(exec));
}

} // namespace AJ

#endif // AJObject_h
