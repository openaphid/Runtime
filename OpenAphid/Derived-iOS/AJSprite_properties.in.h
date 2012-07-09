	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return offsetPositionInPixels, Point
	*/
	AJ::AJValue ajSpriteOffsetPositionInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return textureRect, Rect
	*/
	AJ::AJValue ajSpriteTextureRect(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param textureRect, Rect
	*/
	void setAJSpriteTextureRect(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return flipX, bool
	*/
	AJ::AJValue ajSpriteFlipX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param flipX, bool
	*/
	void setAJSpriteFlipX(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return flipY, bool
	*/
	AJ::AJValue ajSpriteFlipY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param flipY, bool
	*/
	void setAJSpriteFlipY(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return opacity, int
	*/
	AJ::AJValue ajSpriteOpacity(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param opacity, int
	*/
	void setAJSpriteOpacity(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return color, color
	*/
	AJ::AJValue ajSpriteColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param color, color
	*/
	void setAJSpriteColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return texture, Texture2D
	*/
	AJ::AJValue ajSpriteTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param texture, Texture2D
	*/
	void setAJSpriteTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, Sprite
	*/
	AJ::AJValue ajSpriteConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return debugDrawOutline, bool
	*/
	AJ::AJValue ajSpriteConstructorDebugDrawOutline(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param debugDrawOutline, bool
	*/
	void setAJSpriteConstructorDebugDrawOutline(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return debugDrawTextureOutline, bool
	*/
	AJ::AJValue ajSpriteConstructorDebugDrawTextureOutline(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param debugDrawTextureOutline, bool
	*/
	void setAJSpriteConstructorDebugDrawTextureOutline(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
