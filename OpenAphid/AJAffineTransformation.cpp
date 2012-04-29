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

#include "AJAffineTransformation.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
#include "AJAffineTransformation_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJAffineTransformation::AJAffineTransformation(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<AffineTransformation> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJAffineTransformation");
	}
	
	AJAffineTransformation::~AJAffineTransformation()
	{
		//oa_debug("AJAffineTransformation destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJAffineTransformation");
	}
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJAffineTransformationConstructor::constructAffineTransformation(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//AffineTransformation(float a, float b, float c, float d, float tx, float ty)
		if (args.size() >= 6) {
			if (
					ensureFloatArg(exec, args, 0, "a")
					&& ensureFloatArg(exec, args, 1, "b")
					&& ensureFloatArg(exec, args, 2, "c")
					&& ensureFloatArg(exec, args, 3, "d")
					&& ensureFloatArg(exec, args, 4, "tx")
					&& ensureFloatArg(exec, args, 5, "ty")
					) {
				float a = args.at(0).toFloat(exec);
				float b = args.at(1).toFloat(exec);
				float c = args.at(2).toFloat(exec);
				float d = args.at(3).toFloat(exec);
				float tx = args.at(4).toFloat(exec);
				float ty = args.at(5).toFloat(exec);
				
				RefPtr<AffineTransformation> impl = AffineTransformation::create(AffineTransformMake(a, b, c, d, tx, ty));
				AJOAGlobalObject* globalObject = ajoa_cast<AJAffineTransformationConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJAffineTransformation>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "AffineTransformation");
		return 0;
	}
	
	ConstructType AJAffineTransformationConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJAffineTransformationConstructor::constructAffineTransformation;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AffineTransformation* toAffineTransformation(AJValue value)
	{
		return value.inherits(&AJAffineTransformation::s_info) ? ajoa_cast<AJAffineTransformation*>(asObject(value))->impl() : 0;
	}
	
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, AffineTransformation* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJAffineTransformation>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajAffineTransformationA(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAffineTransformation* jsAffineTransformation = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		return jsNumber(exec, jsAffineTransformation->impl()->a());
	}
	
	void setAJAffineTransformationA(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAffineTransformation* jsThis = ajoa_cast<AJAffineTransformation*>(thisObject);
			jsThis->impl()->setA(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "a", "float");
	}
	
	AJ::AJValue ajAffineTransformationB(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAffineTransformation* jsAffineTransformation = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		return jsNumber(exec, jsAffineTransformation->impl()->b());
	}
	
	void setAJAffineTransformationB(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAffineTransformation* jsThis = ajoa_cast<AJAffineTransformation*>(thisObject);
			jsThis->impl()->setB(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "b", "float");
	}
	
	AJ::AJValue ajAffineTransformationC(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAffineTransformation* jsAffineTransformation = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		return jsNumber(exec, jsAffineTransformation->impl()->c());
	}
	
	void setAJAffineTransformationC(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAffineTransformation* jsThis = ajoa_cast<AJAffineTransformation*>(thisObject);
			jsThis->impl()->setC(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "c", "float");
	}
	
	AJ::AJValue ajAffineTransformationD(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAffineTransformation* jsAffineTransformation = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		return jsNumber(exec, jsAffineTransformation->impl()->d());
	}
	
	void setAJAffineTransformationD(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAffineTransformation* jsThis = ajoa_cast<AJAffineTransformation*>(thisObject);
			jsThis->impl()->setD(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "d", "float");
	}
	
	AJ::AJValue ajAffineTransformationTx(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAffineTransformation* jsAffineTransformation = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		return jsNumber(exec, jsAffineTransformation->impl()->tx());
	}
	
	void setAJAffineTransformationTx(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAffineTransformation* jsThis = ajoa_cast<AJAffineTransformation*>(thisObject);
			jsThis->impl()->setTx(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "tx", "float");
	}
	
	AJ::AJValue ajAffineTransformationTy(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJAffineTransformation* jsAffineTransformation = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		return jsNumber(exec, jsAffineTransformation->impl()->ty());
	}
	
	void setAJAffineTransformationTy(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (value.isNumber()) {
			AJAffineTransformation* jsThis = ajoa_cast<AJAffineTransformation*>(thisObject);
			jsThis->impl()->setTy(value.toFloat(exec));
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "ty", "float");
	}
	
	AJ::AJValue JSC_HOST_CALL ajAffineTransformationFunctionSetAll(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJAffineTransformation* thisObject = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		
		//setAll(float a, float b, float c, float d, float tx, float ty)
		if (args.size() >= 6) {
			if (
					ensureFloatArg(exec, args, 0, "a")
					&& ensureFloatArg(exec, args, 1, "b")
					&& ensureFloatArg(exec, args, 2, "c")
					&& ensureFloatArg(exec, args, 3, "d")
					&& ensureFloatArg(exec, args, 4, "tx")
					&& ensureFloatArg(exec, args, 5, "ty")
					) {
				float a = args.at(0).toFloat(exec);
				float b = args.at(1).toFloat(exec);
				float c = args.at(2).toFloat(exec);
				float d = args.at(3).toFloat(exec);
				float tx = args.at(4).toFloat(exec);
				float ty = args.at(5).toFloat(exec);
				thisObject->impl()->setTransform(AffineTransformMake(a, b, c, d, tx, ty));
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "setAll");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajAffineTransformationFunctionClone(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);

		AJAffineTransformation* thisObject = ajoa_cast<AJAffineTransformation*>(asObject(thisValue));
		
		 //clone()
		return toAJ(exec, thisObject->globalObject(), AffineTransformation::create(thisObject->impl()->transform()).get());
	}
	
	AJ::AJValue ajAffineTransformationConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJAffineTransformationBindingType, exec, ajoa_cast<AJAffineTransformation*>(asObject(thisValue))->globalObject());
	}
	
}
