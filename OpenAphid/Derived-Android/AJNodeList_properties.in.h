	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return length, int
	*/
	AJ::AJValue ajNodeListLength(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param index, int
	@return Node
	*/
	AJ::AJValue JSC_HOST_CALL ajNodeListPrototypeFunctionItem(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
