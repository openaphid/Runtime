	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return pixelWidth, float
	*/
	AJ::AJValue ajTexture2DPixelWidth(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return pixelHeight, float
	*/
	AJ::AJValue ajTexture2DPixelHeight(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return contentSize, Size
	*/
	AJ::AJValue ajTexture2DContentSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return contentSizeInPixels, Size
	*/
	AJ::AJValue ajTexture2DContentSizeInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return name, int
	*/
	AJ::AJValue ajTexture2DName(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return hasPremultipliedAlpha, bool
	*/
	AJ::AJValue ajTexture2DHasPremultipliedAlpha(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return antialias, bool
	*/
	AJ::AJValue ajTexture2DAntialias(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param antialias, bool
	*/
	void setAJTexture2DAntialias(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, Texture2D
	*/
	AJ::AJValue ajTexture2DConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajTexture2DPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
