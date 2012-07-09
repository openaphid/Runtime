	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param message, object
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionDebug(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param message, object
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionError(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param message, object
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionInfo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param message, object
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionLog(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param message, object
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionWarn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionTrace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param condition, bool
	@param message, object
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajConsolePrototypeFunctionAssert(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
