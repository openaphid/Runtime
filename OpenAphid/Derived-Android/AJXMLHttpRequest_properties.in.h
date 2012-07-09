	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return readyState, int
	*/
	AJ::AJValue ajXMLHttpRequestReadyState(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return status, int
	*/
	AJ::AJValue ajXMLHttpRequestStatus(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return statusText, string
	*/
	AJ::AJValue ajXMLHttpRequestStatusText(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return responseText, string
	*/
	AJ::AJValue ajXMLHttpRequestResponseText(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return responseXML, string
	*/
	AJ::AJValue ajXMLHttpRequestResponseXML(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return withCredentials, bool
	*/
	AJ::AJValue ajXMLHttpRequestWithCredentials(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param withCredentials, bool
	*/
	void setAJXMLHttpRequestWithCredentials(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onloadstart, function
	*/
	AJ::AJValue ajXMLHttpRequestOnloadstart(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onloadstart, function
	*/
	void setAJXMLHttpRequestOnloadstart(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onprogress, function
	*/
	AJ::AJValue ajXMLHttpRequestOnprogress(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onprogress, function
	*/
	void setAJXMLHttpRequestOnprogress(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onabort, function
	*/
	AJ::AJValue ajXMLHttpRequestOnabort(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onabort, function
	*/
	void setAJXMLHttpRequestOnabort(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onerror, function
	*/
	AJ::AJValue ajXMLHttpRequestOnerror(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onerror, function
	*/
	void setAJXMLHttpRequestOnerror(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onload, function
	*/
	AJ::AJValue ajXMLHttpRequestOnload(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onload, function
	*/
	void setAJXMLHttpRequestOnload(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return ontimeout, function
	*/
	AJ::AJValue ajXMLHttpRequestOntimeout(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param ontimeout, function
	*/
	void setAJXMLHttpRequestOntimeout(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onloadend, function
	*/
	AJ::AJValue ajXMLHttpRequestOnloadend(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onloadend, function
	*/
	void setAJXMLHttpRequestOnloadend(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onreadystatechange, function
	*/
	AJ::AJValue ajXMLHttpRequestOnreadystatechange(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onreadystatechange, function
	*/
	void setAJXMLHttpRequestOnreadystatechange(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return UNSENT, int
	*/
	AJ::AJValue ajXMLHttpRequestPrototypeUNSENT(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return OPENED, int
	*/
	AJ::AJValue ajXMLHttpRequestPrototypeOPENED(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return HEADERS_RECEIVED, int
	*/
	AJ::AJValue ajXMLHttpRequestPrototypeHEADERS_RECEIVED(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return LOADING, int
	*/
	AJ::AJValue ajXMLHttpRequestPrototypeLOADING(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return DONE, int
	*/
	AJ::AJValue ajXMLHttpRequestPrototypeDONE(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param method, string
	@param url, string
	@param async, bool
	@param user, string
	@param password, string
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionOpen(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param name, string
	@param value, string
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionSetRequestHeader(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param data, string
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionSend(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionAbort(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionGetAllResponseHeaders(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param name, string
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajXMLHttpRequestPrototypeFunctionGetResponseHeader(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
