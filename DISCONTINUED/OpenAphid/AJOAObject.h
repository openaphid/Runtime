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

#ifndef OpenAphid_AJOAObject_h
#define OpenAphid_AJOAObject_h

#include "AJOAGlobalObject.h"
#include "runtime/AJObject.h"

namespace Aphid {	
	///-------------------------------------------------------------------------------------------------------------------
	class AJOAObject : public AJ::AJObject {
		typedef AJ::AJObject Base;
		
	public:
		AJOAGlobalObject * globalObject() const;
		
		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}
		
	protected:
		static const unsigned AnonymousSlotCount = 1 + Base::AnonymousSlotCount;
		static const unsigned GlobalObjectSlot = AnonymousSlotCount - 1;
		
		AJOAObject(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject) 
		: Base(structure) 
		{
			putAnonymousValue(GlobalObjectSlot, globalObject);
		}
		
		virtual ~AJOAObject() {
		}
	}; // class AJOAObject
	
	///-------------------------------------------------------------------------------------------------------------------
	class AJOAConstructor : public AJOAObject {
		typedef AJOAObject Base;
	public:
		static PassRefPtr<AJ::Structure> createStructure(AJ::AJValue prototype)
		{
			return AJ::Structure::create(prototype, AJ::TypeInfo(AJ::ObjectType, StructureFlags), AnonymousSlotCount);
		}
		
	protected:
		//TODO: OverridesMarkChildren?
		static const unsigned StructureFlags =AJ::ImplementsHasInstance | AJ::OverridesMarkChildren | Base::StructureFlags;
		
		AJOAConstructor(NonNullPassRefPtr<AJ::Structure> structure, AJOAGlobalObject* globalObject)
		: Base(structure, globalObject)
		{
		}
	};

} // namespace Aphid

#endif
