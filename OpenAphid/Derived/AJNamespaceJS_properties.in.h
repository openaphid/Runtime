	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return onload, function
	*/
	AJ::AJValue ajNamespaceJSOnload(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onload, function
	*/
	void setAJNamespaceJSOnload(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return gcFrequency, int
	*/
	AJ::AJValue ajNamespaceJSGcFrequency(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param gcFrequency, int
	*/
	void setAJNamespaceJSGcFrequency(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return bool
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceJSPrototypeFunctionGc(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param filename, string
	@return object
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceJSPrototypeFunctionIncludeFile(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param script, string
	@return object
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceJSPrototypeFunctionIncludeScript(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
