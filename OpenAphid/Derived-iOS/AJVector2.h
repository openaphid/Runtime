//
//  AJVector2.h
//  OpenAphid
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

#ifndef OpenAphid_AJVector2_h
#define OpenAphid_AJVector2_h

#include "AJOAObject.h"

#include "runtime/AJGlobalObject.h"
#include "runtime/Lookup.h"
#include "runtime/ObjectPrototype.h"
#include "wtf/AlwaysInline.h"

#include "OAData.h"

#include "AJOABinding.h"
#include "OAUtil.h"


namespace Aphid {

	class AJVector2Constructor;

	///-------------------------------------------------------------------------------------------------------------------
	class AJVector2 : public AJOAObject {
		typedef AJOAObject Base;

	public:
		AJVector2(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject, Vector2 impl);
		virtual ~AJVector2();

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		static const AJOABindingType s_type;

		static AJ::AJObject* createPrototype(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject);

		virtual bool getOwnPropertySlot(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertySlot& slot);
		virtual bool getOwnPropertyDescriptor(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertyDescriptor& descriptor);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);

		Vector2 data() const {return m_impl;}
		void setData(Vector2 value) {m_impl = value;}

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

	private:
		Vector2 m_impl;

	protected:
    static const unsigned StructureFlags = AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //class AJVector2

	///-------------------------------------------------------------------------------------------------------------------
	class AJVector2Prototype : public AJ::AJObject {
		typedef AJ::AJObject Base;

	public:
		AJVector2Prototype(NonNullPassRefPtr<AJ::Structure> structure);
		
		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		virtual bool getOwnPropertySlot(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertySlot& slot);
		virtual bool getOwnPropertyDescriptor(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertyDescriptor& descriptor);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		static AJObject* self(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
		{
			return AJ::asObject(getOAStructure(AJVector2BindingType, exec, globalObject)->storedPrototype());
		}

	protected:
		static const unsigned StructureFlags = AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //AJVector2Prototype

	///-------------------------------------------------------------------------------------------------------------------
	class AJVector2Constructor : public AJOAConstructor {
		typedef AJOAConstructor Base;

	public:
		AJVector2Constructor(AJ::ExecState* exec, AJOAGlobalObject* globalObject);

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		virtual bool getOwnPropertySlot(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertySlot& slot);
		virtual bool getOwnPropertyDescriptor(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertyDescriptor& descriptor);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		virtual AJ::ConstructType getConstructData(AJ::ConstructData& constructData);
		
		static AJ::AJObject* constructVector2(AJ::ExecState* exec, AJ::AJObject* constructor, const AJ::ArgList& args);

	protected:
		static const unsigned StructureFlags = AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //AJVector2Constructor

	///-------------------------------------------------------------------------------------------------------------------
	Vector2 toVector2(AJ::AJValue value);
	AJ::AJValue toAJ(AJ::ExecState* exec, AJOAGlobalObject* globalObject, Vector2 impl);

#include "AJVector2_properties.in.h"
} //namespace Aphid


#endif
