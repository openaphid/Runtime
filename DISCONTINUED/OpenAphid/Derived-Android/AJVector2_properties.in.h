	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return x, float
	*/
	AJ::AJValue ajVector2X(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param x, float
	*/
	void setAJVector2X(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return y, float
	*/
	AJ::AJValue ajVector2Y(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param y, float
	*/
	void setAJVector2Y(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@param , float
	@param , float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajVector2FunctionSetXY(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return constructor, Vector2
	*/
	AJ::AJValue ajVector2Constructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param v, Vector2
	@return bool
	*/
	AJ::AJValue JSC_HOST_CALL ajVector2PrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajVector2PrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return Vector2
	*/
	AJ::AJValue JSC_HOST_CALL ajVector2PrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return zero, Vector2
	*/
	AJ::AJValue ajVector2ConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
