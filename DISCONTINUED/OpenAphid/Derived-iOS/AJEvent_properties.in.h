	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return type, string
	*/
	AJ::AJValue ajEventType(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return target, Node
	*/
	AJ::AJValue ajEventTarget(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return currentTarget, Node
	*/
	AJ::AJValue ajEventCurrentTarget(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return eventPhase, int
	*/
	AJ::AJValue ajEventEventPhase(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return bubbles, bool
	*/
	AJ::AJValue ajEventBubbles(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return cancelable, bool
	*/
	AJ::AJValue ajEventCancelable(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return timestamp, int
	*/
	AJ::AJValue ajEventTimestamp(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajEventPrototypeFunctionStopPropagation(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajEventPrototypeFunctionPreventDefault(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
