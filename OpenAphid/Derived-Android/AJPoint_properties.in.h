	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return x, float
	*/
	AJ::AJValue ajPointX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param x, float
	*/
	void setAJPointX(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return y, float
	*/
	AJ::AJValue ajPointY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param y, float
	*/
	void setAJPointY(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@param x, float
	@param y, float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajPointFunctionSetXY(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return constructor, Point
	*/
	AJ::AJValue ajPointConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param p, Point
	@return bool
	*/
	AJ::AJValue JSC_HOST_CALL ajPointPrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajPointPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajPointPrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return zero, Point
	*/
	AJ::AJValue ajPointConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
