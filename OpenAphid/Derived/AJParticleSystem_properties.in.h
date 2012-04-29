	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return constructor, ParticleSystem
	*/
	AJ::AJValue ajParticleSystemConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return texture, Texture2D
	*/
	AJ::AJValue ajParticleSystemTexture(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param texture, Texture2D
	*/
	void setAJParticleSystemTexture(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return blendAdditive, bool
	*/
	AJ::AJValue ajParticleSystemBlendAdditive(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param blendAdditive, bool
	*/
	void setAJParticleSystemBlendAdditive(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
