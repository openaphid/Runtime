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

#include <config.h>

#include "AJLabelTTF.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "OAGlobalObject.h"
#include "NamespaceG2D.h"
#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJFont.h"
#include "AJSize.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJLabelTTF_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJLabelTTF::AJLabelTTF(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<LabelTTF> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJLabelTTF");
	}
	
	AJLabelTTF::~AJLabelTTF()
	{
		//oa_debug("AJLabelTTF destructed");
		AJ_LEAK_DETECT_DEC("AJLabelTTF");
	}
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJLabelTTFConstructor::constructLabelTTF(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//LabelTTF(string label, [Size dimension], [Font font], [string alignment], [string lineBreakMode])
		if (args.size() >= 1) {
			if (
					ensureStringArg(exec, args, 0, "label")
					) {
				const UString& label = valueToUString(exec, args.at(0));
				AJSize* dimension = optObjectArg<AJSize>(exec, args, 1, 0);
				AJFont* font = optObjectArg<AJFont>(exec, args, 2, 0);
				const UString& alignment = optStringArg(exec, args, 3, 0);
				const UString& lineBreakMode = optStringArg(exec, args, 4, 0);
				
				RefPtr<LabelTTF> impl = LabelTTF::create(
																								 label, 
																								 dimension ? dimension->data() : SizeZero, 
																								 font ? font->impl() : Font::create(), 
																								 toTextAlignment(alignment), 
																								 toLineBreakMode(lineBreakMode)
																								 );
				AJOAGlobalObject* globalObject = ajoa_cast<AJLabelTTFConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJLabelTTF>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "LabelTTF");
		return 0;
	}
	
	ConstructType AJLabelTTFConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJLabelTTFConstructor::constructLabelTTF;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	LabelTTF* toLabelTTF(AJValue value)
	{
		return value.inherits(&AJLabelTTF::s_info) ? ajoa_cast<AJLabelTTF*>(asObject(value))->impl() : 0;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajLabelTTFText(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJLabelTTF* ajLabelTTF = ajoa_cast<AJLabelTTF*>(asObject(thisValue));
		return jsString(exec, ajLabelTTF->impl()->labelString());
	}
	
	void setAJLabelTTFText(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsString(value)) {
			AJLabelTTF* jsThis = ajoa_cast<AJLabelTTF*>(thisObject);
			jsThis->impl()->setLabelString(valueToUString(exec, value));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "label", "string");
	}
	
	AJ::AJValue ajLabelTTFConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJLabelTTFBindingType, exec, ajoa_cast<AJLabelTTF*>(asObject(thisValue))->globalObject());
	}
	
}
