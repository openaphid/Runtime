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
#include <runtime/InternalFunction.h>
#include "DynamicBinding+Android.h"
#include "AJDynamicBinding.h"
#include "OAGlobalObject.h"

namespace Aphid {
	using JNI::Cache;
	using namespace AJ;

	JavaDynamicBinding::JavaDynamicBinding(const char* name, PassRefPtr<JNI::GlobalObject> jbinder)
	: Base(name)
	, m_jbinder(jbinder)
	{
	}

	JavaDynamicBinding::~JavaDynamicBinding()
	{
	}
	
	AJ::AJValue JavaDynamicBinding::convertToAJ(JNIEnv* env, AJ::ExecState* exec, AJOAGlobalObject* globalObject, jchar type, jobject jobj)
	{
		if (!jobj)
			return type == 'v' ? jsUndefined() : jsNull();
		
		AJValue ret = jsUndefined();
		switch (type) {
		case 'Z': //boolean
			ret = jsBoolean(JNI::callStaticMethod<jboolean>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_getBoolean, jobj));
			break;
		case 'B': //byte
		case 'C': //char
		case 'S': //short
		case 'I': //int
		case 'J': //long
			ret = jsNumber(exec, JNI::callStaticMethod<jint>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_getInt, jobj));
			break;
		case 'F': //float
			ret = jsNumber(exec, JNI::callStaticMethod<jfloat>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_getFloat, jobj));
			break;
		case 'D': //double
			ret = jsNumber(exec, JNI::callStaticMethod<jdouble>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_getDouble, jobj));
			break;
		case 'v': //void
			ret = jsUndefined();
			break;
		case 's': //string
			ASSERT(env->IsInstanceOf(jobj, Cache::s_string_jclass->global()));
			ret = jsString(exec, toUString(JNI::toString(env, (jstring)jobj)));
			break;
		case 'l': //list
		{
			MarkedArgumentBuffer args;
			jint size = JNI::callMethod<jint>(env, jobj, Cache::s_list_jmethod_size);
			for (int i = 0; i < size; i++) {
				jobject jElement = JNI::callMethod<jobject>(env, jobj, Cache::s_list_jmethod_get, i);
				jchar jType = JNI::callStaticMethod<jchar>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_s_jmethod_getObjectTypeSignature, jElement);
				args.append(convertToAJ(env, exec, globalObject, jType, jElement));
				env->DeleteLocalRef(jElement);
			}
			
			ret = constructArray(exec, args);
		}
			break;
		case 'm': //map
		{
			AJObject* aj = constructEmptyObject(exec, globalObject);
			jobject jIterator = JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_s_jmethod_getKeyIterator, jobj);
			ASSERT(jIterator);
			while(JNI::callMethod<jboolean>(env, jIterator, Cache::s_iterator_jmethod_hasNext)) {
				jobject jKey = JNI::callMethod<jobject>(env, jIterator, Cache::s_iterator_jmethod_next);
				if (jKey) {
					if (env->IsInstanceOf(jKey, Cache::s_string_jclass->global())) {
						jobject jValue = JNI::callMethod<jobject>(env, jobj, Cache::s_map_jmethod_get, jKey);
						jchar jType = JNI::callStaticMethod<jchar>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_s_jmethod_getObjectTypeSignature, jValue);
						aj->putDirect(
								Identifier(exec, toUString(JNI::toString(env, (jstring)jKey))),
								convertToAJ(env, exec, globalObject, jType, jValue)
								);
						env->DeleteLocalRef(jValue);
					}
				}
				env->DeleteLocalRef(jKey);
			}
			env->DeleteLocalRef(jIterator);
			ret = aj;
		}
			break;
		default:
			oa_error("Unrecognized Java object type: %c", type);
			break;
		}
		
		return ret;
	}

	jobject JavaDynamicBinding::convertToJava(JNIEnv* env, AJ::ExecState* exec, AJ::AJValue value,
			bool treatArrayAsObject)
	{
		if (!value)
			return 0;
		else if (value.isBoolean())
			return JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toBoolean,
					value.toBoolean(exec) ? JNI_TRUE : JNI_FALSE);
		else if (value.isInt32())
			return JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toInteger, value.toInt32(exec));
		else if (value.isNumber())
			return JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toDouble, value.toNumber(exec));
		else if (value.isUndefinedOrNull())
			return 0;
		else if (value.isString())
			return env->NewStringUTF(value.toString(exec).UTF8String().data());
		else if (value.isObject()) {
			if (!treatArrayAsObject && value.inherits(&AJArray::info)) {
				AJArray* ajArray = ajoa_cast<AJArray*>(asObject(value));
				jint arrayLength = ajArray->length();
				jobject jList =
						env->NewObject(Cache::s_arraylist_jclass->global(), Cache::s_arraylist_jmethod_constructor_int, arrayLength);
				ASSERT(jList);

				for (int i = 0; i < arrayLength; i++) {
					jobject jObj = convertToJava(env, exec, ajArray->getIndex(i));
					JNI::callMethod<jboolean>(env, jList, Cache::s_list_jmethod_add, jObj);
					env->DeleteLocalRef(jObj);
				}

				return jList;
			} else if (value.inherits(&InternalFunction::info)) {
				if (OAGlobalObject::s_develop_mode)
					oa_info("Convert function object to null");
				return 0;
			} else {
				AJObject* ajObject = asObject(value);
				PropertyNameArray properties(exec->lexicalGlobalObject()->globalData());
				ajObject->getPropertyNames(exec, properties);
				size_t size = properties.size();
				jobject jMap =
						env->NewObject(Cache::s_hashmap_jclass->global(), Cache::s_hashmap_jmethod_constructor_int, size);
				for (size_t i = 0; i < size; i++) {
					const Identifier& identifier = properties[i];
					AJValue ajV = ajObject->get(exec, identifier);
					if (ajV) {
						if (!ajV.inherits(&InternalFunction::info)) {
							jobject jKey = env->NewStringUTF(identifier.ustring().UTF8String().data());
							jobject jValue = convertToJava(env, exec, ajV);
							JNI::callObjectMethod(env, jMap, Cache::s_map_jmethod_put, jKey, jValue);
							env->DeleteLocalRef(jKey);
							env->DeleteLocalRef(jValue);
						} else {
							oa_debug("function value is ignored in dictionary: %s", identifier.ascii());
						}
					} else {
						oa_debug("empty value is ignored in dictionary: %s", identifier.ascii());
					}
				}

				return jMap;
			} //end of JS object
		} else {
			oa_error("Invalid value type for convertToObjC");
			ASSERT_NOT_REACHED();
		}
		return 0;
	}

	void JavaDynamicBinding::listFunctionNames(Vector<AJ::UString>& names)
	{
		JNIEnv* env = JNI::getJNIEnv();
		jobjectArray jnames =
				(jobjectArray) JNI::callMethod<jobject>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_listFunctionNames);
		jsize size = env->GetArrayLength(jnames);
		for (jsize i = 0; i < size; i++) {
			jstring name = (jstring) env->GetObjectArrayElement(jnames, i);
			names.append(toUString(JNI::toString(env, name)));
			env->DeleteLocalRef(name);
		}
	}

	bool JavaDynamicBinding::respondsToFunction(const AJ::UString& functionName)
	{
		JNIEnv* env = JNI::getJNIEnv();
		return JNI::callMethod<jboolean>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_reapondsToFunction, JNI::toJNIString(env, toString(functionName))->local());
	}

	AJ::AJValue JavaDynamicBinding::invokeFunction(AJ::ExecState* exec, AJOAGlobalObject* globalObject,
			const AJ::UString& functionName, const AJ::ArgList& args)
	{
		JNIEnv* env = JNI::getJNIEnv();
		OwnPtr<JNI::LocalString> jfunctionName = JNI::toJNIString(env, toString(functionName));
		AJValue retValue = jsUndefined();

		jcharArray jparameterSignature =
				(jcharArray) JNI::callMethod<jobject>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_getParameterTypeSignature, jfunctionName->local());
		
		jsize parameterCount = env->GetArrayLength(jparameterSignature);
		if (parameterCount > 0) {
			jchar* jptrParameterSignature = env->GetCharArrayElements(jparameterSignature, 0);
			if (args.size() >= parameterCount) {
				jobjectArray jargs = env->NewObjectArray(parameterCount, Cache::s_object_jclass->global(), 0);

				for (int i = 0; i < parameterCount; i++) {
					char type = jptrParameterSignature[i];
					AJValue arg = args.at(i);
					bool error = false;
					switch (type) {
					case 'Z': //boolean
						if (arg.isBoolean()) {
							jobject jBoolean =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toBoolean,
											arg.toBoolean(exec) ? JNI_TRUE : JNI_FALSE);
							ASSERT(jBoolean);
							env->SetObjectArrayElement(jargs, i, jBoolean);
							env->DeleteLocalRef(jBoolean);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "bool");
							error = true;
						}
						break;
					case 'B': //byte
						if (arg.isNumber()) {
							jobject jByte =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toByte, arg.toInt32(exec));
							ASSERT(jByte);
							env->SetObjectArrayElement(jargs, i, jByte);
							env->DeleteLocalRef(jByte);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "byte");
							error = true;
						}
						break;
					case 'C': //char
						if (arg.isNumber()) {
							jobject jChar =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toChar_int, arg.toInt32(exec));
							ASSERT(jChar);
							env->SetObjectArrayElement(jargs, i, jChar);
							env->DeleteLocalRef(jChar);
						} else if (arg.isString()) {
							jobject jChar =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toChar_String, JNI::toJNIString(env, arg.toString(exec))->local());
							ASSERT(jChar);
							env->SetObjectArrayElement(jargs, i, jChar);
							env->DeleteLocalRef(jChar);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "char");
							error = true;
						}
						break;
					case 'S': //short
						if (arg.isNumber()) {
							jobject jShort =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toShort, arg.toInt32(exec));
							ASSERT(jShort);
							env->SetObjectArrayElement(jargs, i, jShort);
							env->DeleteLocalRef(jShort);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "short");
							error = true;
						}
						break;
					case 'I': //int
						if (arg.isNumber()) {
							jobject jInt =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toInteger, arg.toInt32(exec));
							ASSERT(jInt);
							env->SetObjectArrayElement(jargs, i, jInt);
							env->DeleteLocalRef(jInt);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "int");
							error = true;
						}
						break;
					case 'J': //long
						if (arg.isNumber()) {
							jobject jLong =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toLong, arg.toInt32(exec));
							ASSERT(jLong);
							env->SetObjectArrayElement(jargs, i, jLong);
							env->DeleteLocalRef(jLong);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "long");
							error = true;
						}
						break;
					case 'F': //float
						if (arg.isNumber()) {
							jobject jFloat =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toFloat, arg.toFloat(exec));
							ASSERT(jFloat);
							env->SetObjectArrayElement(jargs, i, jFloat);
							env->DeleteLocalRef(jFloat);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "float");
							error = true;
						}
						break;
					case 'D': //double
						if (arg.isNumber()) {
							jobject jDouble =
									JNI::callStaticMethod<jobject>(env, Cache::s_aphidjsbinder_jclass->global(), Cache::s_aphidjsbinder_jmethod_toDouble, arg.toNumber(exec));
							ASSERT(jDouble);
							env->SetObjectArrayElement(jargs, i, jDouble);
							env->DeleteLocalRef(jDouble);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "double");
							error = true;
						}
						break;
					case 'v': //void
						oa_error("Void type is invalid for parameter");
						ASSERT_NOT_REACHED();
						break;
					case 's' : //String
						if (arg.isString()) {
							OwnPtr<JNI::LocalString> jString = JNI::toJNIString(env, arg.toString(exec));
							env->SetObjectArrayElement(jargs, i, jString->local());
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "String");
							error = true;
						}
						break;
					case 'l': //list
						if (arg.inherits(&AJArray::info)) {
							jobject jList = convertToJava(env, exec, arg);
							ASSERT(jList);
							env->SetObjectArrayElement(jargs, i, jList);
							env->DeleteLocalRef(jList);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "array");
							error = true;
						}
						break;
					case 'm': //map
						if (arg.isObject()) {
							jobject jMap = convertToJava(env, exec, arg, true);
							ASSERT(jMap);
							env->SetObjectArrayElement(jargs, i, jMap);
							env->DeleteLocalRef(jMap);
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "object");
							error = true;
						}
						break;
					default:
						oa_error("Unknown arg type in native function: %c", type);
						ASSERT_NOT_REACHED();
						break;
					}
				} //end of for

				if (!exec->hadException()) {
					jchar jReturnTypeSignature =
							JNI::callMethod<jchar>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_getReturnTypeSignature, jfunctionName->local());
					jobject jRet = JNI::callMethod<jobject>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_invokeMethod, jfunctionName->local(), jargs);
					if (env->IsSameObject(jRet, Cache::s_aphidjsbinder_error_return->global())) {
						exec->setException(
								Error::create(exec, GeneralError, formatUString("Failed to invoke Java function '%s'", functionName.UTF8String().data()).UTF8String().data()));
					} else {
						retValue = convertToAJ(env, exec, globalObject, jReturnTypeSignature, jRet);
					}
					env->DeleteLocalRef(jRet);
				}

				env->DeleteLocalRef(jargs);
			} else
				OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, parameterCount, args.size());

			env->ReleaseCharArrayElements(jparameterSignature, jptrParameterSignature, 0);
		} else { //empty argument
			jchar jReturnTypeSignature =
					JNI::callMethod<jchar>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_getReturnTypeSignature, jfunctionName->local());
			jobject jRet = JNI::callMethod<jobject>(env, m_jbinder->global(), Cache::s_aphidjsbinder_jmethod_invokeMethod, jfunctionName->local(), 0);
			if (env->IsSameObject(jRet, Cache::s_aphidjsbinder_error_return->global())) {
				exec->setException(
						Error::create(exec, GeneralError, formatUString("Failed to invoke Java function '%s'", functionName.UTF8String().data()).UTF8String().data()));
			} else {
				retValue = convertToAJ(env, exec, globalObject, jReturnTypeSignature, jRet);
			}
			env->DeleteLocalRef(jRet);
		}

		env->DeleteLocalRef(jparameterSignature);

		return retValue;
	}
}
