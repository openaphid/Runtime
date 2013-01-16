//
//  AJInstantAction.h
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

#ifndef OpenAphid_AJInstantAction_h
#define OpenAphid_AJInstantAction_h

#include "AJOAObject.h"

#include "runtime/AJGlobalObject.h"
#include "runtime/Lookup.h"
#include "runtime/ObjectPrototype.h"
#include "wtf/AlwaysInline.h"

#include "Action.h"

#include "AJOABinding.h"
#include "OAUtil.h"

#include "AJFiniteTimeAction.h"

namespace Aphid {

	class InstantAction;

	///-------------------------------------------------------------------------------------------------------------------
	class AJInstantAction : public AJFiniteTimeAction {
		typedef AJFiniteTimeAction Base;

	public:
		AJInstantAction(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<InstantAction> impl);
		virtual ~AJInstantAction();

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		static const AJOABindingType s_type;

		static AJ::AJObject* createPrototype(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject);


		InstantAction* impl() const
		{
			return oa_cast<InstantAction*>(Base::impl());
		}

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

	protected:
    static const unsigned StructureFlags = Base::StructureFlags;
	}; //class AJInstantAction

	///-------------------------------------------------------------------------------------------------------------------
	class AJInstantActionPrototype : public AJ::AJObject {
		typedef AJ::AJObject Base;

	public:
		AJInstantActionPrototype(NonNullPassRefPtr<AJ::Structure> structure);
		
		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;


		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		static AJObject* self(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
		{
			return AJ::asObject(getOAStructure(AJInstantActionBindingType, exec, globalObject)->storedPrototype());
		}

	protected:
		static const unsigned StructureFlags = Base::StructureFlags;
	}; //AJInstantActionPrototype


	///-------------------------------------------------------------------------------------------------------------------
	InstantAction* toInstantAction(AJ::AJValue value);

#include "AJInstantAction_properties.in.h"
} //namespace Aphid


#endif
