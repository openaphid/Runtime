	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return winSize, Size
	*/
	AJ::AJValue ajDirectorWinSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return winSizeInPixels, Size
	*/
	AJ::AJValue ajDirectorWinSizeInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return runningScene, Scene
	*/
	AJ::AJValue ajDirectorRunningScene(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return contentScaleFactor, float
	*/
	AJ::AJValue ajDirectorContentScaleFactor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param contentScaleFactor, float
	*/
	void setAJDirectorContentScaleFactor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return displayFPS, bool
	*/
	AJ::AJValue ajDirectorDisplayFPS(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param displayFPS, bool
	*/
	void setAJDirectorDisplayFPS(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return fpsInterval, float
	*/
	AJ::AJValue ajDirectorFpsInterval(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param fpsInterval, float
	*/
	void setAJDirectorFpsInterval(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return multipleTouchEnabled, bool
	*/
	AJ::AJValue ajDirectorMultipleTouchEnabled(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param multipleTouchEnabled, bool
	*/
	void setAJDirectorMultipleTouchEnabled(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param p, Point
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajDirectorPrototypeFunctionConvertToGL(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param scene, Scene
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajDirectorPrototypeFunctionRunScene(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
