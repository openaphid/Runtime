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

#ifndef OpenAphid_AJOABinding_h
#define OpenAphid_AJOABinding_h

#include "OAConfig.h"
#include "OAUtil.h"
#include "OAData.h"
#include <runtime/Error.h>
#include <wtf/Assertions.h>
#include <runtime/AJCell.h>
#include <WTFString.h>
#include <AtomicString.h>

#include "AJOAGlobalObject.h"
#include "DirectWrapper.h"
#include "ExceptionCode.h"

namespace Aphid {	
	class Color;
	
#define CHECK_INHERITS(exec, thisValue, classType) \
if (!(thisValue).inherits(&classType::s_info)) \
return throwError((exec), TypeError, "Illegal invocation")
	
#if OA_DEV
	template<class T>
	T ajoa_cast(AJ::AJCell* from)
	{
		T ret = dynamic_cast<T>(from);
		if (!ret) {
			oa_error("cast failed!");
			ASSERT_NOT_REACHED();
		}
		return ret;
	}
#define oa_cast dynamic_cast
#else
#define ajoa_cast static_cast
#define oa_cast static_cast
#endif
	
	/*
	template<class ConstructorClass>
	OA_RELEASE_INLINE AJ::AJObject * getOAConstructor(AJ::ExecState* exec, const AJOAGlobalObject* globalObject)
	{
		AJ::AJObject* constructor = globalObject->constructors().get(&ConstructorClass::s_info);
		if (constructor)
			return constructor;
		constructor = new(exec) ConstructorClass(exec, const_cast<AJOAGlobalObject*>(globalObject));
		ASSERT(!globalObject->constructors().contains(&ConstructorClass::s_info));
		globalObject->constructors().set(&ConstructorClass::s_info, constructor);
		return constructor;
	}*/

	//TODO: P3 cache some wrapper objects in a hashmap?
	//TODO: add a AJString cache as WebKit does in jsString(...)?
	
	template<class WrapperClass, class OAClass>
	OA_RELEASE_INLINE WrapperClass* createAJOAWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject, OAClass* object)
	{
		ASSERT(object);
		ASSERT(!object->wrapper());
		
		WrapperClass* wrapper = new (exec) WrapperClass(getOAStructure(WrapperClass::s_type, exec, globalObject), globalObject, object);
		object->setWrapper(wrapper);
		return wrapper;
	}	
	
	template<class WrapperClass, class DataClass>
	OA_RELEASE_INLINE WrapperClass* createAJDataWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject, DataClass object)
	{
		return new (exec) WrapperClass(getOAStructure(WrapperClass::s_type, exec, globalObject), globalObject, object);
	}
	
	//TODO: a rough hack which leverages directWrapper to mark heap, should study deeper on WebKit's way
	template<class WrapperClass>
	void markDirectWrapper(DirectWrapper<WrapperClass>* object, AJ::MarkStack& markStack, unsigned markID)
	{
		//if (!object || !object->directWrapper())
		if (!object)
			return;
		//markStack.append(object->directWrapper());
		if (!object->isMarked(markID))
		object->markObjects(markStack, markID);
	}
	
	//TODO: enhance the error reporting system
	/*
	enum AJOAErrorCode {
		AJOAErrorCodeInvalidArgument = 1,
		AJOAErrorCodeInvalidSetterValue = 2, 
		AJOAErrorCodeNotEnoughArgument = 3
	};
	 */
	
	void reportAJException(AJ::ExecState* exec, AJ::AJValue exception, bool reportExecException=true);
	
	void applyJSExceptionCode(AJ::ExecState* exec, JSExceptionCode error);
	
	void OA_ERROR_INVALID_ARGUMENT(AJ::ExecState* exec, const char* name, const char* requiredType);
	void OA_ERROR_NOT_ENOUGH_ARGUMENTS(AJ::ExecState* exec, int requiredCount, int actualCount);
	void OA_ERROR_INVALID_PROPERTY(AJ::ExecState* exec, const char* name, const char* requiredType);
	void OA_ERROR_INVALID_ARGS(AJ::ExecState* exec, const char* functionName);
	
	AJ::UString formatUString(const char* format, ...);
	
	AJ::UString toUString(Rect r);
	AJ::UString toUString(Point p);
	AJ::UString toUString(Size s);
	AJ::UString toUString(Vector2 v);
	AJ::UString toUString(Vector3 v);
	AJ::UString toUString(GridSize s);
	
	static Aphid::String toString(const AJ::UString& string) {return string.rep();}
	static AJ::UString toUString(const Aphid::String& string) {return AJ::UString(string.impl());}
	
	///TODO: implement a string cache
	static AJ::UString toUString(const Aphid::AtomicString& string)
	{
		if (string.length() == 0)
			return AJ::UString();
		return AJ::UString(string.string().impl());
	}
	
	static AJ::UString valueToUString(AJ::ExecState* exec, AJ::AJValue value)
	{
		if (value.isNull())
			return AJ::UString();
		return value.toString(exec);
	}
	
	static bool valueIsString(AJ::AJValue value, bool supportNull=true)
	{
		return value.isString() || (supportNull && value.isNull());
	}
	
	static bool valueIsArray(AJ::AJValue value, bool supportNull=true)
	{
		return (supportNull && value.isNull()) || value.inherits(&AJ::AJArray::info);
	}
	
	static bool valueIsObject(AJ::AJValue value, bool supportNull=true)
	{
		return (supportNull && value.isNull()) || value.isObject();
	}
	
	static AJ::AJObject* valueAsObject(AJ::AJValue value)
	{
		return value.isObject() ? AJ::asObject(value) : 0;
	}
	
	template<class AJObjectClass>
	bool valueIsOAObject(AJ::AJValue value, bool supportNull = true)
	{
		return (supportNull && value.isNull()) || value.inherits(&AJObjectClass::s_info);
	}
	
	template<class AJObjectClass, class OAObjectClass>
	OAObjectClass* valueToAJObject(AJ::AJValue value, bool supportNull=true)
	{
		if (value.isObject())
			return ajoa_cast<AJObjectClass*>(asObject(value))->impl();
		else if (value.isNull() && supportNull)
			return 0;
		else {
			oa_warn("Invalid AJValue to OAObject conversion!");
			return 0;
		}
	}
	
#define IMPL_FROM_VALUE(class, value) valueToAJObject<AJ##class, class>(value, false)
#define OPT_IMPL_FROM_VALUE(class, value) valueToAJObject<AJ##class, class>(value, true)	

	bool ensureArrayArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName, bool supportNull=true);
	
	bool ensureColorArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName);
	bool isColorValue(AJ::ExecState* exec, AJ::AJValue value);
	Color toColor(AJ::ExecState* exec, AJ::AJValue value);
	Color optColorArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, unsigned defaultValue);
	
	
	static OA_RELEASE_INLINE bool optBoolArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, bool defaultValue)
	{
		if (index < args.size() && args.at(index).isBoolean())
			return args.at(index).toBoolean(exec);
		return defaultValue;
	}
	
	static OA_RELEASE_INLINE int optIntArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, int defaultValue)
	{
		if (index < args.size() && args.at(index).isNumber())
			return args.at(index).toInt32(exec);
		return defaultValue;
	}
	
	static OA_RELEASE_INLINE float optFloatArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, float defaultValue)
	{
		if (index < args.size() && args.at(index).isNumber())
			return args.at(index).toFloat(exec);
		return defaultValue;
	}
	
	static OA_RELEASE_INLINE AJ::UString optStringArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* defaultValue)
	{
		if (index < args.size() && args.at(index).isString())
			return args.at(index).toString(exec);
		return defaultValue; //TODO: is it fine to return null for UString?
	}
	
	template<class AJObjectClass>
	OA_RELEASE_INLINE AJObjectClass* optObjectArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, AJObjectClass* defaultValue=0)
	{
		UNUSED_PARAM(exec);
		if (index < args.size() && args.at(index).inherits(&AJObjectClass::s_info))
			return oa_cast<AJObjectClass*>(asObject(args.at(index)));
		return defaultValue;
	}
	
	static OA_RELEASE_INLINE bool ensureBoolArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName)
	{
		if (index < args.size() && args.at(index).isBoolean())
			return true;
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "boolean");
		return false;
	}
	
	static OA_RELEASE_INLINE bool ensureIntArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName)
	{
		if (index < args.size() && args.at(index).isNumber())
			return true;
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "int");
		return false;
	}
	
	static OA_RELEASE_INLINE bool ensureFloatArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName)
	{
		if (index < args.size() && args.at(index).isNumber())
			return true;
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "float");
		return false;
	}
	
	static OA_RELEASE_INLINE bool ensureStringArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName, bool supportNull=true)
	{
		if (index < args.size()) {
			AJ::AJValue arg = args.at(index);
			if (arg.isString() || (supportNull && arg.isNull()))
				return true;
		}
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "string");
		return false;
	}
	
	static OA_RELEASE_INLINE bool ensureFunctionArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName, bool supportNull=true)
	{
		if (index < args.size()) {
			AJ::AJValue arg = args.at(index);
			if (arg.isObject() || (supportNull && arg.isUndefinedOrNull()))
				return true;
		}
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "function object");
		return false;
	}
	
	template<class AJObjectClass>
	OA_RELEASE_INLINE bool ensureObjectArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName, const char* typeName, bool supportNull=true)
	{
		if (index < args.size()) {
			AJ::AJValue arg = args.at(index);
			if (arg.inherits(&AJObjectClass::s_info) || (supportNull && arg.isUndefinedOrNull()))
				return true;
		}
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, typeName);
		return false;
	}
	
#define ENSURE_OBJ_ARG(exec, args, index, parameter, class) ensureObjectArg<AJ##class>(exec, args, index, parameter, #class, false)
#define ENSURE_OBJ_NULL_ARG(exec, args, index, parameter, class) ensureObjectArg<AJ##class>(exec, args, index, parameter, #class, true)
	
	//#define OA_ERROR_INVALID_ARGUMENT(exec, name, required) oa_error("%s's type should be %s", (name), (required))
	//#define OA_ERROR_INVALID_PROPERTY(exec, name, required) oa_error("%s's type should be %s", (name), (required))
	//#define OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, required, actual) oa_error("not enough arguments, required %d, actual %d", (required), (actual))
	
	//a utility to throw common OA exceptions
	
	void warnReadonlyAccess(AJ::ExecState* exec, const AJ::UString& className, const AJ::Identifier& propertyName);
	
	bool oalookupPut(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, const AJ::HashTable* table, AJ::AJObject* thisObj);
	
	template <class ThisImp, class ParentImp>
	inline void oalookupPut(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, const AJ::HashTable* table, ThisImp* thisObj, AJ::PutPropertySlot& slot)
	{
		if (!oalookupPut(exec, propertyName, value, table, thisObj))
			thisObj->ParentImp::put(exec, propertyName, value, slot); // not found: forward to parent
	}
}

#endif
