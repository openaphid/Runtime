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

#include "AJFont.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;

#include "AJFont_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJFont::AJFont(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<Font> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJFont");
	}
	
	AJFont::~AJFont()
	{
		//oa_debug("AJFont destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJFont");
	}
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJFontConstructor::constructFont(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		//Font(string fontName, float fontSize)
		if (args.size() >= 2) {
			if (
				ensureStringArg(exec, args, 0, "fontName")
				&& ensureFloatArg(exec, args, 1, "fontSize")
				) {
				const UString& fontName = valueToUString(exec, args.at(0));
			float fontSize = args.at(1).toFloat(exec);

			RefPtr<Font> impl = Font::create(fontName, fontSize);
			AJOAGlobalObject* globalObject = ajoa_cast<AJFontConstructor*>(constructor)->globalObject();
			return createAJOAWrapper<AJFont>(exec, globalObject, impl.get());
			}
		}

		exec->clearException();
		//Font(float fontSize)
		if (args.size() >= 1) {
			if (
				ensureFloatArg(exec, args, 0, "fontSize")
				) {
			float fontSize = args.at(0).toFloat(exec);

			RefPtr<Font> impl = Font::create(fontSize);
			AJOAGlobalObject* globalObject = ajoa_cast<AJFontConstructor*>(constructor)->globalObject();
			return createAJOAWrapper<AJFont>(exec, globalObject, impl.get());
			}
		}

		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Font");
		return 0;
	}
	
	ConstructType AJFontConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJFontConstructor::constructFont;
		return ConstructTypeHost;
	}

	///-------------------------------------------------------------------------------------------------------------------
	Font* toFont(AJValue value)
	{
		return value.inherits(&AJFont::s_info) ? ajoa_cast<AJFont*>(asObject(value))->impl() : 0;
	}

	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, Font* impl)
	{
		if (!impl)
			return jsNull();

		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJFont>(exec, globalObject, impl);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajFontFontName(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJFont* ajFont = ajoa_cast<AJFont*>(asObject(thisValue));
		return jsString(exec, ajFont->impl()->fontName());
	}

	AJ::AJValue ajFontFontSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJFont* ajFont = ajoa_cast<AJFont*>(asObject(thisValue));
		return jsNumber(exec, ajFont->impl()->fontSize());
	}

	AJ::AJValue ajFontConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		return getOAConstructor(AJFontBindingType, exec, ajoa_cast<AJFont*>(asObject(thisValue))->globalObject());
	}

}
