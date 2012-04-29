	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return touches, TouchList
	*/
	AJ::AJValue ajTouchEventTouches(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return targetTouches, TouchList
	*/
	AJ::AJValue ajTouchEventTargetTouches(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return changedTouches, TouchList
	*/
	AJ::AJValue ajTouchEventChangedTouches(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return altKey, bool
	*/
	AJ::AJValue ajTouchEventAltKey(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return metaKey, bool
	*/
	AJ::AJValue ajTouchEventMetaKey(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return ctrlKey, bool
	*/
	AJ::AJValue ajTouchEventCtrlKey(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return shiftKey, bool
	*/
	AJ::AJValue ajTouchEventShiftKey(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
