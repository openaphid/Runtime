//
//  AJTexture2D.h
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

#ifndef OpenAphid_AJTexture2D_h
#define OpenAphid_AJTexture2D_h

#include "AJOAObject.h"

#include "runtime/AJGlobalObject.h"
#include "runtime/Lookup.h"
#include "runtime/ObjectPrototype.h"
#include "wtf/AlwaysInline.h"

#include "Texture2D.h"

#include "AJOABinding.h"
#include "OAUtil.h"


namespace Aphid {

	class Texture2D;
	class AJTexture2DConstructor;

	///-------------------------------------------------------------------------------------------------------------------
	class AJTexture2D : public AJOAObject {
		typedef AJOAObject Base;

	public:
		AJTexture2D(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Texture2D> impl);
		virtual ~AJTexture2D();

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		static const AJOABindingType s_type;

		static AJ::AJObject* createPrototype(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject);

		virtual bool getOwnPropertySlot(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertySlot& slot);
		virtual bool getOwnPropertyDescriptor(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertyDescriptor& descriptor);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);

		Texture2D* impl() const {return m_impl.get();}

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		static AJ::UString toUString(Texture2D* impl);

	private:
		RefPtr<Texture2D> m_impl;

	protected:
    static const unsigned StructureFlags = AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //class AJTexture2D

	///-------------------------------------------------------------------------------------------------------------------
	class AJTexture2DPrototype : public AJ::AJObject {
		typedef AJ::AJObject Base;

	public:
		AJTexture2DPrototype(NonNullPassRefPtr<AJ::Structure> structure);
		
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
			return AJ::asObject(getOAStructure(AJTexture2DBindingType, exec, globalObject)->storedPrototype());
		}

	protected:
		static const unsigned StructureFlags = AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //AJTexture2DPrototype

	///-------------------------------------------------------------------------------------------------------------------
	class AJTexture2DConstructor : public AJOAConstructor {
		typedef AJOAConstructor Base;

	public:
		AJTexture2DConstructor(AJ::ExecState* exec, AJOAGlobalObject* globalObject);

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;


		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		virtual AJ::ConstructType getConstructData(AJ::ConstructData& constructData);
		
		static AJ::AJObject* constructTexture2D(AJ::ExecState* exec, AJ::AJObject* constructor, const AJ::ArgList& args);

	protected:
		static const unsigned StructureFlags = Base::StructureFlags;
	}; //AJTexture2DConstructor

	///-------------------------------------------------------------------------------------------------------------------
	Texture2D* toTexture2D(AJ::AJValue value);
	AJ::AJValue toAJ(AJ::ExecState* exec, AJOAGlobalObject* globalObject, Texture2D* impl);
	AJ::UString toUString(Texture2D* impl);

#include "AJTexture2D_properties.in.h"
} //namespace Aphid


#endif
