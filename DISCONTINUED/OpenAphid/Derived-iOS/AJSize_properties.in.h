	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return width, float
	*/
	AJ::AJValue ajSizeWidth(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param width, float
	*/
	void setAJSizeWidth(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return height, float
	*/
	AJ::AJValue ajSizeHeight(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param height, float
	*/
	void setAJSizeHeight(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@param , float
	@param , float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajSizeFunctionSetWH(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return constructor, Size
	*/
	AJ::AJValue ajSizeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param s, Size
	@return bool
	*/
	AJ::AJValue JSC_HOST_CALL ajSizePrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajSizePrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return Size
	*/
	AJ::AJValue JSC_HOST_CALL ajSizePrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return zero, Size
	*/
	AJ::AJValue ajSizeConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
