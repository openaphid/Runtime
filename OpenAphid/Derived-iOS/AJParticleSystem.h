//
//  AJParticleSystem.h
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

#ifndef OpenAphid_AJParticleSystem_h
#define OpenAphid_AJParticleSystem_h

#include "AJOAObject.h"

#include "runtime/AJGlobalObject.h"
#include "runtime/Lookup.h"
#include "runtime/ObjectPrototype.h"
#include "wtf/AlwaysInline.h"

#include "ParticleSystem.h"

#include "AJOABinding.h"
#include "OAUtil.h"

#include "AJNode.h"

namespace Aphid {

	class ParticleSystem;
	class AJParticleSystemConstructor;

	///-------------------------------------------------------------------------------------------------------------------
	class AJParticleSystem : public AJNode {
		typedef AJNode Base;

	public:
		AJParticleSystem(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<ParticleSystem> impl);
		virtual ~AJParticleSystem();

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		static const AJOABindingType s_type;

		static AJ::AJObject* createPrototype(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject);

		virtual bool getOwnPropertySlot(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertySlot& slot);
		virtual bool getOwnPropertyDescriptor(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertyDescriptor& descriptor);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);
		virtual void markChildren(AJ::MarkStack& markStack);

		ParticleSystem* impl() const
		{
			return oa_cast<ParticleSystem*>(Base::impl());
		}

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

	protected:
    static const unsigned StructureFlags = AJ::OverridesMarkChildren | AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //class AJParticleSystem

	///-------------------------------------------------------------------------------------------------------------------
	class AJParticleSystemPrototype : public AJ::AJObject {
		typedef AJ::AJObject Base;

	public:
		AJParticleSystemPrototype(NonNullPassRefPtr<AJ::Structure> structure);
		
		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;


		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		static AJObject* self(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
		{
			return AJ::asObject(getOAStructure(AJParticleSystemBindingType, exec, globalObject)->storedPrototype());
		}

	protected:
		static const unsigned StructureFlags = Base::StructureFlags;
	}; //AJParticleSystemPrototype

	///-------------------------------------------------------------------------------------------------------------------
	class AJParticleSystemConstructor : public AJOAConstructor {
		typedef AJOAConstructor Base;

	public:
		AJParticleSystemConstructor(AJ::ExecState* exec, AJOAGlobalObject* globalObject);

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;


		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		virtual AJ::ConstructType getConstructData(AJ::ConstructData& constructData);
		
		static AJ::AJObject* constructParticleSystem(AJ::ExecState* exec, AJ::AJObject* constructor, const AJ::ArgList& args);

	protected:
		static const unsigned StructureFlags = Base::StructureFlags;
	}; //AJParticleSystemConstructor

	///-------------------------------------------------------------------------------------------------------------------
	ParticleSystem* toParticleSystem(AJ::AJValue value);

#include "AJParticleSystem_properties.in.h"
} //namespace Aphid


#endif
