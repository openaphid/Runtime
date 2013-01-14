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

#include "AJXMLHttpRequest.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "AJEventListener.h"
#include "NamespaceCore.h"
#include "OAGlobalObject.h"
//referred
//#include "AJXMLHttpRequestUpload.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJXMLHttpRequest_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJXMLHttpRequest::AJXMLHttpRequest(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<XMLHttpRequest> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJXMLHttpRequest");
	}
	
	AJXMLHttpRequest::~AJXMLHttpRequest()
	{
		//oa_debug("AJXMLHttpRequest destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJXMLHttpRequest");
	}
	
	void AJXMLHttpRequest::markChildren(AJ::MarkStack &markStack)
	{
		Base::markChildren(markStack);
		m_impl->markObjects(markStack, Heap::s_mark_id);
	}
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJObject* AJXMLHttpRequestConstructor::constructXMLHttpRequest(ExecState* exec, AJObject* constructor, const ArgList& args) 
	{
		UNUSED_PARAM(args);
		RefPtr<XMLHttpRequest> impl = XMLHttpRequest::create();
		AJOAGlobalObject* globalObject = ajoa_cast<AJXMLHttpRequestConstructor*>(constructor)->globalObject();
		return createAJOAWrapper<AJXMLHttpRequest>(exec, globalObject, impl.get());
	}
	
	ConstructType AJXMLHttpRequestConstructor::getConstructData(ConstructData& constructData)
	{
		constructData.native.function = AJXMLHttpRequestConstructor::constructXMLHttpRequest;
		return ConstructTypeHost;
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	XMLHttpRequest* toXMLHttpRequest(AJValue value)
	{
		return value.inherits(&AJXMLHttpRequest::s_info) ? ajoa_cast<AJXMLHttpRequest*>(asObject(value))->impl() : 0;
	}
	
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, XMLHttpRequest* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJXMLHttpRequest>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajXMLHttpRequestReadyState(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return jsNumber(exec, ajXMLHttpRequest->impl()->readyState());
	}
	
	AJ::AJValue ajXMLHttpRequestStatus(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		JSExceptionCode error = 0;
		int status = ajXMLHttpRequest->impl()->status(error);
		if (error != 0)
			applyJSExceptionCode(exec, error);
		return jsNumber(exec, status);
	}
	
	AJ::AJValue ajXMLHttpRequestStatusText(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		JSExceptionCode error = 0;
		Aphid::String statusText = ajXMLHttpRequest->impl()->statusText(error);
		if (error != 0)
			applyJSExceptionCode(exec, error);
		return jsString(exec, toUString(statusText));
	}
	
	AJ::AJValue ajXMLHttpRequestResponseText(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return jsString(exec, toUString(ajXMLHttpRequest->impl()->responseText()));
	}
	
	AJ::AJValue ajXMLHttpRequestResponseXML(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNull();
	}
	
	AJ::AJValue ajXMLHttpRequestWithCredentials(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		OA_TODO();
		return jsBoolean(false);
	}
	
	void setAJXMLHttpRequestWithCredentials(AJ::ExecState*, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		UNUSED_PARAM(thisObject);
		UNUSED_PARAM(value);
		//Shadowing a builtin object
		//ajoa_cast<AJXMLHttpRequest*>(thisObject)->putDirect(Identifier(exec, "withCredentials"), value);
		/*
		 if (value.isBoolean()) {
		 AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
		 jsThis->impl()->setWithCredentials(value.toBoolean(exec));
		 } else
		 OA_ERROR_INVALID_PROPERTY(exec, "withCredentials", "bool");
		 */
		OA_TODO();
	}
	
	AJ::AJValue ajXMLHttpRequestOnloadstart(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestLoadStart));		
	}
	
	void setAJXMLHttpRequestOnloadstart(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestLoadStart, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onloadstart", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOnprogress(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestProgress));		
	}
	
	void setAJXMLHttpRequestOnprogress(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestProgress, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onprogress", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOnabort(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestAbort));		
	}
	
	void setAJXMLHttpRequestOnabort(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestAbort, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onabort", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOnerror(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestError));
	}
	
	void setAJXMLHttpRequestOnerror(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestError, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onerror", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOnload(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestLoad));
	}
	
	void setAJXMLHttpRequestOnload(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestLoad, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onload", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOntimeout(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestTimeout));
	}
	
	void setAJXMLHttpRequestOntimeout(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestTimeout, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "ontimeout", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOnloadend(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestLoadEnd));
	}
	
	void setAJXMLHttpRequestOnloadend(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestLoadEnd, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onloadend", "function");
	}
	
	AJ::AJValue ajXMLHttpRequestOnreadystatechange(AJ::ExecState*, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		AJXMLHttpRequest* ajXMLHttpRequest = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		return AJEventListener::findAJCallback(ajXMLHttpRequest->impl()->findEventListener(EventFlagRequestReadyStateChange));
	}
	
	void setAJXMLHttpRequestOnreadystatechange(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value) 
	{
		if (valueIsObject(value)) {
			AJXMLHttpRequest* jsThis = ajoa_cast<AJXMLHttpRequest*>(thisObject);
			AJEventListener::setAJListener(EventFlagRequestReadyStateChange, jsThis->impl(), exec, value);
		} else
			OA_ERROR_INVALID_PROPERTY(exec, "onreadystatechange", "function");
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajXMLHttpRequestPrototypeUNSENT(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, XMLHttpRequest::UNSENT);
	}
	
	AJ::AJValue ajXMLHttpRequestPrototypeOPENED(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, XMLHttpRequest::OPENED);
	}
	
	AJ::AJValue ajXMLHttpRequestPrototypeHEADERS_RECEIVED(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, XMLHttpRequest::HEADERS_RECEIVED);
	}
	
	AJ::AJValue ajXMLHttpRequestPrototypeLOADING(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, XMLHttpRequest::LOADING);
	}
	
	AJ::AJValue ajXMLHttpRequestPrototypeDONE(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, XMLHttpRequest::DONE);
	}
	
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionOpen(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJXMLHttpRequest);
		
		AJXMLHttpRequest* thisObject = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		
		//open(string method, string url, [bool async], [string user], [string password])
		if (args.size() >= 2) {
			if (
					ensureStringArg(exec, args, 0, "method")
					&& ensureStringArg(exec, args, 1, "url")
					) {
				const UString& method = valueToUString(exec, args.at(0));
				const UString& url = valueToUString(exec, args.at(1));
				bool async = optBoolArg(exec, args, 2, true);
				//const UString& user = optStringArg(exec, args, 3, 0);
				//const UString& password = optStringArg(exec, args, 4, 0);
				JSExceptionCode error = 0;
				thisObject->impl()->open(toString(method), thisObject->globalObject()->impl()->namespaceCore()->makeURL(url), async, error);
				if (error != 0)
					applyJSExceptionCode(exec, error);
				
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "open");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionSetRequestHeader(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJXMLHttpRequest);
		
		AJXMLHttpRequest* thisObject = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		
		//setRequestHeader(string name, string value)
		if (args.size() >= 2) {
			if (
					ensureStringArg(exec, args, 0, "name")
					&& ensureStringArg(exec, args, 1, "value")
					) {
				const UString& name = valueToUString(exec, args.at(0));
				const UString& value = valueToUString(exec, args.at(1));
				
				JSExceptionCode error = 0;
				thisObject->impl()->setRequestHeader(toString(name), toString(value), error);
				if (error != 0)
					applyJSExceptionCode(exec, error);
				return jsUndefined();
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "setRequestHeader");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionSend(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJXMLHttpRequest);
		
		AJXMLHttpRequest* thisObject = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		
		//send([string data])
		const UString& data = optStringArg(exec, args, 0, 0);
		//TODO: support send(FormData)
		JSExceptionCode error = 0;
		thisObject->impl()->send(toString(data), error);
		if (error != 0)
			applyJSExceptionCode(exec, error);
		
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionAbort(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJXMLHttpRequest);
		
		AJXMLHttpRequest* thisObject = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		
		//abort()
		thisObject->impl()->abort();
		
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionGetAllResponseHeaders(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		CHECK_INHERITS(exec, thisValue, AJXMLHttpRequest);
		OA_TODO();
		//AJXMLHttpRequest* thisObject = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		/*
		 //getAllResponseHeaders()
		 if (args.size() >= 0) {
		 //TODO:
		 }
		 */
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "getAllResponseHeaders");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionGetResponseHeader(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		CHECK_INHERITS(exec, thisValue, AJXMLHttpRequest);
		
		AJXMLHttpRequest* thisObject = ajoa_cast<AJXMLHttpRequest*>(asObject(thisValue));
		
		//getResponseHeader(string name)
		if (args.size() >= 1) {
			if (
					ensureStringArg(exec, args, 0, "name")
					) {
				const UString& name = valueToUString(exec, args.at(0));
				JSExceptionCode error = 0;
				Aphid::String value = thisObject->impl()->getResponseHeader(toString(name), error);
				if (error != 0)
					applyJSExceptionCode(exec, error);
				return jsString(exec, toUString(value));
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "getResponseHeader");
		return jsUndefined();
	}
	
}
