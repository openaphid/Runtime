	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return a, float
	*/
	AJ::AJValue ajAffineTransformationA(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param a, float
	*/
	void setAJAffineTransformationA(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return b, float
	*/
	AJ::AJValue ajAffineTransformationB(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param b, float
	*/
	void setAJAffineTransformationB(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return c, float
	*/
	AJ::AJValue ajAffineTransformationC(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param c, float
	*/
	void setAJAffineTransformationC(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return d, float
	*/
	AJ::AJValue ajAffineTransformationD(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param d, float
	*/
	void setAJAffineTransformationD(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return tx, float
	*/
	AJ::AJValue ajAffineTransformationTx(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param tx, float
	*/
	void setAJAffineTransformationTx(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return ty, float
	*/
	AJ::AJValue ajAffineTransformationTy(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param ty, float
	*/
	void setAJAffineTransformationTy(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@param a, float
	@param b, float
	@param c, float
	@param d, float
	@param tx, float
	@param ty, float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajAffineTransformationFunctionSetAll(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return AffineTransformation
	*/
	AJ::AJValue JSC_HOST_CALL ajAffineTransformationFunctionClone(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return constructor, AffineTransformation
	*/
	AJ::AJValue ajAffineTransformationConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
