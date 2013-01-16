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

#include "AJGridBaseAction.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
//referred
#include "AJSize.h"
//end

namespace Aphid {
	using namespace AJ;

#include "AJGridBaseAction_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJGridBaseAction::AJGridBaseAction(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<GridBaseAction> impl)
	: Base(structure, globalObject, impl)
	{
		AJ_LEAK_DETECT_INC("AJGridBaseAction");
	}
	
	AJGridBaseAction::~AJGridBaseAction()
	{
		//oa_debug("AJGridBaseAction destructed");
		AJ_LEAK_DETECT_DEC("AJGridBaseAction");
	}
	///-------------------------------------------------------------------------------------------------------------------
	GridBaseAction* toGridBaseAction(AJValue value)
	{
		return value.inherits(&AJGridBaseAction::s_info) ? ajoa_cast<AJGridBaseAction*>(asObject(value))->impl() : 0;
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajGridBaseActionGridSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJGridBaseAction* ajGridBaseAction = ajoa_cast<AJGridBaseAction*>(asObject(thisValue));
		return toAJ(exec, ajGridBaseAction->globalObject(), toSize(ajGridBaseAction->impl()->gridSize()));
	}

	/*
	void setAJGridBaseActionGridSize(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.inherits(&AJSize::s_info)) {
			AJGridBaseAction* jsThis = ajoa_cast<AJGridBaseAction*>(thisObject);
			jsThis->impl()->setGridSize(toGridSize(ajoa_cast<AJSize*>(asObject(value))->data()));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "gridSize", "GridSize");
	}*/

}
