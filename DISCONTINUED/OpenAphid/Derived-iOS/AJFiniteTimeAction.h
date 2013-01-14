//
//  AJFiniteTimeAction.h
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

#ifndef OpenAphid_AJFiniteTimeAction_h
#define OpenAphid_AJFiniteTimeAction_h

#include "AJOAObject.h"

#include "runtime/AJGlobalObject.h"
#include "runtime/Lookup.h"
#include "runtime/ObjectPrototype.h"
#include "wtf/AlwaysInline.h"

#include "Action.h"

#include "AJOABinding.h"
#include "OAUtil.h"

#include "AJAction.h"

namespace Aphid {

	class FiniteTimeAction;

	///-------------------------------------------------------------------------------------------------------------------
	class AJFiniteTimeAction : public AJAction {
		typedef AJAction Base;

	public:
		AJFiniteTimeAction(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<FiniteTimeAction> impl);
		virtual ~AJFiniteTimeAction();

		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;

		static const AJOABindingType s_type;

		static AJ::AJObject* createPrototype(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject);

		virtual bool getOwnPropertySlot(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertySlot& slot);
		virtual bool getOwnPropertyDescriptor(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::PropertyDescriptor& descriptor);
		virtual void put(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, AJ::PutPropertySlot& slot);

		FiniteTimeAction* impl() const
		{
			return oa_cast<FiniteTimeAction*>(Base::impl());
		}

		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

	protected:
    static const unsigned StructureFlags = AJ::OverridesGetOwnPropertySlot | Base::StructureFlags;
	}; //class AJFiniteTimeAction

	///-------------------------------------------------------------------------------------------------------------------
	class AJFiniteTimeActionPrototype : public AJ::AJObject {
		typedef AJ::AJObject Base;

	public:
		AJFiniteTimeActionPrototype(NonNullPassRefPtr<AJ::Structure> structure);
		
		virtual const AJ::ClassInfo* classInfo() const {return &s_info;}
		static const AJ::ClassInfo s_info;


		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}

		static AJObject* self(AJ::ExecState* exec, AJOAGlobalObject* globalObject)
		{
			return AJ::asObject(getOAStructure(AJFiniteTimeActionBindingType, exec, globalObject)->storedPrototype());
		}

	protected:
		static const unsigned StructureFlags = Base::StructureFlags;
	}; //AJFiniteTimeActionPrototype


	///-------------------------------------------------------------------------------------------------------------------
	FiniteTimeAction* toFiniteTimeAction(AJ::AJValue value);

#include "AJFiniteTimeAction_properties.in.h"
} //namespace Aphid


#endif
