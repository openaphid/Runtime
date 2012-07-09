	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return x, float
	*/
	AJ::AJValue ajVector3X(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param x, float
	*/
	void setAJVector3X(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return y, float
	*/
	AJ::AJValue ajVector3Y(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param y, float
	*/
	void setAJVector3Y(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return z, float
	*/
	AJ::AJValue ajVector3Z(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param z, float
	*/
	void setAJVector3Z(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@param , float
	@param , float
	@param , float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajVector3FunctionSetXYZ(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return constructor, Vector3
	*/
	AJ::AJValue ajVector3Constructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param v, Vector3
	@return bool
	*/
	AJ::AJValue JSC_HOST_CALL ajVector3PrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajVector3PrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return Vector3
	*/
	AJ::AJValue JSC_HOST_CALL ajVector3PrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return zero, Vector3
	*/
	AJ::AJValue ajVector3ConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
