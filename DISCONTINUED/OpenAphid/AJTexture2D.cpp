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

#include "AJTexture2D.h"

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
	
#include "AJTexture2D_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJTexture2D::AJTexture2D(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Texture2D> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJTexture2D");
	}
	
	AJTexture2D::~AJTexture2D()
	{
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJTexture2D");
	}
	
	AJ::UString toUString(Texture2D* impl)
	{
		return formatUString("Texture2D(%d, %d)", impl->pixelWidth(), impl->pixelHeight());
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJTexture2DConstructor::constructTexture2D(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Texture2D(string textureUrl)
		if (args.size() >= 1) {
			if (
					ensureStringArg(exec, args, 0, "textureUrl", false)
					) {
				const UString& textureUrl = valueToUString(exec, args.at(0));
				
				RefPtr<Texture2D> impl = Texture2D::create(textureUrl);
				AJOAGlobalObject* globalObject = ajoa_cast<AJTexture2DConstructor*>(constructor)->globalObject();
				return createAJOAWrapper<AJTexture2D>(exec, globalObject, impl.get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Texture2D");
		return 0;
	}
	
	ConstructType AJTexture2DConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJTexture2DConstructor::constructTexture2D;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Texture2D* toTexture2D(AJValue value)
	{
		return value.inherits(&AJTexture2D::s_info) ? ajoa_cast<AJTexture2D*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Texture2D* impl)
	{
		if (!impl)
			return jsNull();
		
		AJTexture2D* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		
		return createAJOAWrapper<AJTexture2D>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajTexture2DPixelWidth(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* ajTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return jsNumber(exec, ajTexture2D->impl()->pixelWidth());
	}
	
	AJ::AJValue ajTexture2DPixelHeight(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* ajTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return jsNumber(exec, ajTexture2D->impl()->pixelHeight());
	}
	
	AJ::AJValue ajTexture2DContentSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* ajTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return toAJ(exec, ajTexture2D->globalObject(), ajTexture2D->impl()->contentSize());
	}
	
	AJ::AJValue ajTexture2DContentSizeInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* ajTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return toAJ(exec, ajTexture2D->globalObject(), ajTexture2D->impl()->contentSizeInPixels());
	}
	
	AJ::AJValue ajTexture2DName(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* ajTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return jsNumber(exec, ajTexture2D->impl()->name());
	}
	
	AJ::AJValue ajTexture2DHasPremultipliedAlpha(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* ajTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return jsBoolean(ajTexture2D->impl()->hasPremultipliedAlpha());
	}
	
	AJ::AJValue ajTexture2DAntialias(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJTexture2D* jsTexture2D = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return jsBoolean(jsTexture2D->impl()->antialias());
	}
	
	void setAJTexture2DAntialias(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		 if (value.isBoolean()) {
		 AJTexture2D* jsThis = ajoa_cast<AJTexture2D*>(thisObject);
		 if (value.toBoolean(exec))
			 jsThis->impl()->setAntiAliasTexParameters();
		 else 
			 jsThis->impl()->setAliasTexParameters();
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "antialias", "bool");
	}
	
	AJ::AJValue ajTexture2DConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		return getOAConstructor(AJTexture2DBindingType, exec, ajoa_cast<AJTexture2D*>(asObject(thisValue))->globalObject());
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajTexture2DPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJTexture2D);
		AJTexture2D* thisObject = ajoa_cast<AJTexture2D*>(asObject(thisValue));
		return jsString(exec, toUString(thisObject->impl()));
	}	
}
