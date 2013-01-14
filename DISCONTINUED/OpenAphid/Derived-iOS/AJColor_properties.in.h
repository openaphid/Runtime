	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return r, int
	*/
	AJ::AJValue ajColorR(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return g, int
	*/
	AJ::AJValue ajColorG(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return b, int
	*/
	AJ::AJValue ajColorB(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return a, int
	*/
	AJ::AJValue ajColorA(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return constructor, Color
	*/
	AJ::AJValue ajColorConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajColorPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
