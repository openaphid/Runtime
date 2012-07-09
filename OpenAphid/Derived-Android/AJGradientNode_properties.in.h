	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return startColor, color
	*/
	AJ::AJValue ajGradientNodeStartColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param startColor, color
	*/
	void setAJGradientNodeStartColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return endColor, color
	*/
	AJ::AJValue ajGradientNodeEndColor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param endColor, color
	*/
	void setAJGradientNodeEndColor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return vector, Vector2
	*/
	AJ::AJValue ajGradientNodeVector(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param vector, Vector2
	*/
	void setAJGradientNodeVector(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return compressedInterpolation, bool
	*/
	AJ::AJValue ajGradientNodeCompressedInterpolation(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param compressedInterpolation, bool
	*/
	void setAJGradientNodeCompressedInterpolation(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, GradientNode
	*/
	AJ::AJValue ajGradientNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
