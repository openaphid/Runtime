	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return x, float
	*/
	AJ::AJValue ajRectX(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param x, float
	*/
	void setAJRectX(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return y, float
	*/
	AJ::AJValue ajRectY(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param y, float
	*/
	void setAJRectY(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return width, float
	*/
	AJ::AJValue ajRectWidth(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param width, float
	*/
	void setAJRectWidth(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return height, float
	*/
	AJ::AJValue ajRectHeight(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param height, float
	*/
	void setAJRectHeight(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return origin, Point
	*/
	AJ::AJValue ajRectOrigin(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param origin, Point
	*/
	void setAJRectOrigin(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return size, Size
	*/
	AJ::AJValue ajRectSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param size, Size
	*/
	void setAJRectSize(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@param x, float
	@param y, float
	@param w, float
	@param h, float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajRectFunctionSetXYWH(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return constructor, Rect
	*/
	AJ::AJValue ajRectConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param r, Rect
	@return bool
	*/
	AJ::AJValue JSC_HOST_CALL ajRectPrototypeFunctionEquals(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return string
	*/
	AJ::AJValue JSC_HOST_CALL ajRectPrototypeFunctionToString(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return Rect
	*/
	AJ::AJValue JSC_HOST_CALL ajRectPrototypeFunctionCopy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return zero, Rect
	*/
	AJ::AJValue ajRectConstructorZero(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
