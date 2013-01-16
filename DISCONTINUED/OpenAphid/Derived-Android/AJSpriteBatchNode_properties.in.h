	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return texture, Texture2D
	*/
	AJ::AJValue ajSpriteBatchNodeTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param texture, Texture2D
	*/
	void setAJSpriteBatchNodeTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, SpriteBatchNode
	*/
	AJ::AJValue ajSpriteBatchNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param child, Sprite
	@param z, int
	@param tag, int
	@return SpriteBatchNode
	*/
	AJ::AJValue JSC_HOST_CALL ajSpriteBatchNodePrototypeFunctionAddChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
