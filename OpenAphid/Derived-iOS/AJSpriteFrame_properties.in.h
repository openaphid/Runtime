	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return rect, Rect
	*/
	AJ::AJValue ajSpriteFrameRect(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param rect, Rect
	*/
	void setAJSpriteFrameRect(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return rectInPixels, Rect
	*/
	AJ::AJValue ajSpriteFrameRectInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param rectInPixels, Rect
	*/
	void setAJSpriteFrameRectInPixels(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return rotated, bool
	*/
	AJ::AJValue ajSpriteFrameRotated(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param rotated, bool
	*/
	void setAJSpriteFrameRotated(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return texture, Texture2D
	*/
	AJ::AJValue ajSpriteFrameTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param texture, Texture2D
	*/
	void setAJSpriteFrameTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, SpriteFrame
	*/
	AJ::AJValue ajSpriteFrameConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
