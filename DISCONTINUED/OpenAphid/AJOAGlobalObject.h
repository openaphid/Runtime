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

#ifndef OpenAphid_AJOAGlobalObject_h
#define OpenAphid_AJOAGlobalObject_h

#include "runtime/AJGlobalObject.h"
#include "OAUtil.h"

#include "AJOABindingTypes.h"

namespace Aphid {
	
	class OAGlobalObject;
	
	//typedef HashMap<const AJ::ClassInfo*, RefPtr<AJ::Structure> > AJOAStructureMap;
	typedef Vector<RefPtr<AJ::Structure> > AJOAStructureVector;
	typedef Vector<AJ::AJObject* > AJOAConstructorVector;
	
	///-------------------------------------------------------------------------------------------------------------------
	class AJOAGlobalObject : public AJ::AJGlobalObject {
		typedef AJ::AJGlobalObject Base;
		
	public:
		virtual ~AJOAGlobalObject();
		
		struct AJOAGlobalObjectData;
		
		AJOAGlobalObject(ATF::NonNullPassRefPtr<AJ::Structure>, PassRefPtr<OAGlobalObject> impl);		
		
		OAGlobalObject* impl() const {return m_impl.get();}
		
		virtual const AJ::ClassInfo* classInfo() const 
		{
			return &s_info;
		}
		static const AJ::ClassInfo s_info;
		
		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
    {
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
    }
		
		virtual bool getOwnPropertySlot(AJ::ExecState*, const AJ::Identifier&, AJ::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(AJ::ExecState*, const AJ::Identifier&, AJ::PropertyDescriptor&);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);
		virtual void markChildren(AJ::MarkStack& markStack);

		AJOAStructureVector& structures() const
		{
			return data()->structures;
		}
		
		AJOAConstructorVector& constructors() const
		{
			return data()->constructors;
		}
		
	public:
		struct AJOAGlobalObjectData : public AJGlobalObject::AJGlobalObjectData {
			AJOAGlobalObjectData(Destructor destructor=destroyAJOAGlobalObjectData)
				: AJGlobalObjectData(destructor)
			{
			}
			
			AJOAStructureVector structures;
			AJOAConstructorVector constructors;
		};
		
		static AJ::Structure* createOAStructure(AJOABindingType type, AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		static AJ::AJObject* createOAConstructor(AJOABindingType type, AJ::ExecState* exec, const AJOAGlobalObject* globalObject);
		
	private:
		RefPtr<OAGlobalObject> m_impl;
		
	protected:
		static const unsigned StructureFlags = AJ::OverridesMarkChildren | AJ::NeedsThisConversion | AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;//TODO: added NeedsThisConversion as JSDOMWindow does, don't know why though...
		
	private:
		static void destroyAJOAGlobalObjectData(void* globalObjectData)
		{
			delete static_cast<AJOAGlobalObjectData*>(globalObjectData);
		}
		
		AJOAGlobalObjectData* data() const 
		{
			return static_cast<AJOAGlobalObjectData*>(JSVariableObject::d);
		}
		
		
		
	}; // class AJOAGlobalObject
	
	///-------------------------------------------------------------------------------------------------------------------
	OAGlobalObject* toOAGlobalObject(AJ::AJValue value);
	AJ::AJValue toAJ(AJ::ExecState* exec, AJOAGlobalObject* globalObject, OAGlobalObject* impl);
	
	static AJ::Structure* getOAStructure(AJOABindingType type, AJ::ExecState* exec, AJOAGlobalObject* globalObject)
	{
		if (AJ::Structure* structure = globalObject->structures().at((unsigned)type).get())
			return structure;
		return AJOAGlobalObject::createOAStructure(type, exec, globalObject);
	}
	
	static AJ::AJObject* getOAConstructor(AJOABindingType type, AJ::ExecState* exec, const AJOAGlobalObject* globalObject)
	{
		if (AJ::AJObject* constructor = globalObject->constructors().at((unsigned)type))
			return constructor;
		return AJOAGlobalObject::createOAConstructor(type, exec, globalObject);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
#include "AJOAGlobalObject_properties.in.h"
}

#endif
