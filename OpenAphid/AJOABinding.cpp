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
#include <runtime/AJLock.h>
#include <bytecode/CodeBlock.h>
#include <runtime/Lookup.h>
#include "AJOABinding.h"
#include "OAGlobalObject.h"
#include "AJColor.h"
#include "Diagnostic.h"

namespace Aphid {
	using namespace AJ;
	
	void reportAJException(AJ::ExecState* exec, AJ::AJValue exception, bool reportExecException)
	{
		if (exception.isObject()) {
			if (asObject(exception)->exceptionType() == Terminated)
				return;
			
			AJLock lock(exec);
			
			if (exec->hadException()) {
				AJValue previousException = exec->exception();
				exec->clearException();
				
				if (reportExecException && previousException != exception)
					reportAJException(exec, previousException, false);
			}
			
			AJObject* exceptionObj = exception.toObject(exec);
			int lineNO = exceptionObj->get(exec, Identifier(exec, "line")).toUInt32(exec);
			UString exceptionURL = exceptionObj->get(exec, Identifier(exec, "sourceURL")).toString(exec);
			UString errorMessage = exception.toString(exec);
			exec->clearException();
			
			Diagnostic::error(formatUString("[%s:%d] JS Exception: %s", exceptionURL.UTF8String().data(), lineNO, errorMessage.UTF8String().data()));
		} else 
			oa_debug("Not an exception object");
	}
	
	void applyJSExceptionCode(AJ::ExecState* exec, JSExceptionCode error)
	{
		if (error == 0 || exec->hadException())
			return;
		///TODO:
		throwError(exec, GeneralError, formatUString("exception code %d", error));
	}
	
	void OA_ERROR_INVALID_ARGUMENT(AJ::ExecState* exec, const char* name, const char* requiredType)
	{
		throwError(exec, TypeError, formatUString("Invalid argument, type of param '%s' should be %s", name, requiredType));
	}
	
	void OA_ERROR_NOT_ENOUGH_ARGUMENTS(AJ::ExecState* exec, int requiredCount, int actualCount)
	{
		throwError(exec, GeneralError, formatUString("Not enough arguments: required %d, got %d", requiredCount, actualCount));
	}
	
	void OA_ERROR_INVALID_PROPERTY(AJ::ExecState* exec, const char* name, const char* requiredType)
	{
		throwError(exec, TypeError, formatUString("Invalid property assignment, type of property '%s' should be %s", name, requiredType));
	}
	
	void OA_ERROR_INVALID_ARGS(AJ::ExecState* exec, const char* functionName)
	{
		throwError(exec, TypeError, formatUString("Invalid arguments for %s", functionName));
	}
	
	UString formatUString(const char* format, ...)
	{
		///TODO: switch to use String::format directly?
		va_list args;
		va_start(args, format);
		
		Vector<char, 256> buffer;
		char ch;
		int result = vsnprintf(&ch, 1, format, args);
		va_end(args);
		va_start(args, format);
		
		if (result == 0)
			return UString("");
		else if (result < 0)
			return UString();
		else {
			unsigned len = result;
			buffer.grow(len + 1);
			vsnprintf(buffer.data(), buffer.size(), format, args);
			
			va_end(args);
			
			return UString(buffer.data(), len);
		}
	}
	
	AJ::UString toUString(Rect r)
	{
		return formatUString("Rect(%.2f, %.2f, %.2f, %.2f)", r.origin.x, r.origin.y, r.size.width, r.size.height);
	}
	
	AJ::UString toUString(Point p)
	{
		return formatUString("Point(%.2f, %.2f)", p.x, p.y);
	}
	AJ::UString toUString(Size s)
	{
		return formatUString("Size(%.2f, %.2f)", s.width, s.height);
	}
	AJ::UString toUString(Vector2 v)
	{
		return formatUString("Vector2(%.2f, %.2f)", v.x, v.y);
	}
	AJ::UString toUString(Vector3 v)
	{
		return formatUString("Vector3(%.2f, %.2f, %.2f)", v.x, v.y, v.z);
	}
	AJ::UString toUString(GridSize s)
	{
		return formatUString("GridSize(%d, %d)", s.x, s.y);
	}
	
	bool ensureArrayArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName, bool supportNull)
	{
		if (index < args.size()) {
			AJValue arg = args.at(index);
			if (arg.inherits(&AJArray::info) || (supportNull && arg.isNull()))
				return true;
		}
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "Array");
		return false;
	}
	
	bool isColorValue(AJ::ExecState* exec, AJ::AJValue value)
	{
		if (value.isNumber())
			return true;
		else if (value.inherits(&AJColor::s_info))
			return true;
		else if (value.isString()) {
			Color color(value.toString(exec)); //TODO: duplicate parsing in ensuring and getting, can be improved?
			if (color.isValid())
				return true;
		}
		
		return false;
	}
	
	bool ensureColorArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, const char* parameterName)
	{
		if (index < args.size()) {
			if (isColorValue(exec, args.at(index)))
				return true;
			if (args.at(index).isString())
				OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "valid color name");
			return false;
		}
		
		OA_ERROR_INVALID_ARGUMENT(exec, parameterName, "color");
		return false;
	}
	
	Color toColor(AJ::ExecState* exec, AJ::AJValue value)
	{
		if (value.isNumber())
			return Color(value.toInt32(exec));
		else if (value.inherits(&AJColor::s_info))
			return oa_cast<AJColor*>(asObject(value))->data();
		else if (value.isString()) {
			const UString& string = value.toString(exec);
			Color color(string);
			if (!color.isValid())
				throwError(exec, TypeError, formatUString("Invalid color string: %s", string.UTF8String().data()));
			return color;
		}
		throwError(exec, TypeError, "Invalid value type for color");
		return Color();
	}
	
	Color optColorArg(AJ::ExecState* exec, const AJ::ArgList& args, size_t index, unsigned defaultValue)
	{
		if (index < args.size()) {
			const Color& color = toColor(exec, args.at(index));
			if (color.isValid())
				return color;
		}
		
		return Color(defaultValue);
	}
	
	void warnReadonlyAccess(AJ::ExecState* exec, const UString& className, const AJ::Identifier& propertyName)
	{
		if (OAGlobalObject::s_develop_mode)
			throwError(exec, TypeError, formatUString("Invalid access to readonly property '%s' of '%s'", propertyName.ustring().ascii(), className.ascii()));
		else {
			CodeBlock* codeBlock = exec->codeBlock();
			const UString& sourceURL = codeBlock->ownerExecutable()->sourceURL();
			Diagnostic::warn(formatUString("Invalid access to readonly property '%s' of '%s' in %s", propertyName.ustring().ascii(), className.ascii(), sourceURL.ascii()));
		}
			
	}
	
	bool oalookupPut(AJ::ExecState* exec, const AJ::Identifier& propertyName, AJ::AJValue value, const AJ::HashTable* table, AJ::AJObject* thisObj)
	{
		const AJ::HashEntry* entry = table->entry(exec, propertyName);
		
		if (!entry)
			return false;
		
		if (entry->attributes() & AJ::Function) { // function: put as override property
			if (LIKELY(value.isCell()))
				thisObj->putDirectFunction(propertyName, value.asCell());
			else
				thisObj->putDirect(propertyName, value);
		} else if (!(entry->attributes() & AJ::ReadOnly))
			entry->propertyPutter()(exec, thisObj, value);
		else 
			warnReadonlyAccess(exec, thisObj->className(), propertyName);
		
		return true;
	}
}
