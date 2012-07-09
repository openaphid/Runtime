	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return length, int
	*/
	AJ::AJValue ajTouchListLength(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param index, int
	@return Touch
	*/
	AJ::AJValue JSC_HOST_CALL ajTouchListFunctionItem(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param identifier, int
	@return Touch
	*/
	AJ::AJValue JSC_HOST_CALL ajTouchListFunctionIdentifiedTouch(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
