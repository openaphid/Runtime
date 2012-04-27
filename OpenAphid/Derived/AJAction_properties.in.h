	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return baseType, int
	*/
	AJ::AJValue ajActionBaseType(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return typeName, string
	*/
	AJ::AJValue ajActionTypeName(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return isTimeReversed, bool
	*/
	AJ::AJValue ajActionIsTimeReversed(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param isTimeReversed, bool
	*/
	void setAJActionIsTimeReversed(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return Action
	*/
	AJ::AJValue JSC_HOST_CALL ajActionFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return Action
	*/
	AJ::AJValue JSC_HOST_CALL ajActionFunctionReverse(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
