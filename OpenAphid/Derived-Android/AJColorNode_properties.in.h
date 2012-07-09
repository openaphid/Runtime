	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return color, color
	*/
	AJ::AJValue ajColorNodeColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param color, color
	*/
	void setAJColorNodeColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return opacity, int
	*/
	AJ::AJValue ajColorNodeOpacity(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param opacity, int
	*/
	void setAJColorNodeOpacity(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, ColorNode
	*/
	AJ::AJValue ajColorNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
