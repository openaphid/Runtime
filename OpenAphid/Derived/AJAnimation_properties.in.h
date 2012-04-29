	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return delay, float
	*/
	AJ::AJValue ajAnimationDelay(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param delay, float
	*/
	void setAJAnimationDelay(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, Animation
	*/
	AJ::AJValue ajAnimationConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param frame, SpriteFrame
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajAnimationPrototypeFunctionAddFrame(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
