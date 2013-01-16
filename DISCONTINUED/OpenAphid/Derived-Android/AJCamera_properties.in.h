	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return eye, Vector3
	*/
	AJ::AJValue ajCameraEye(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param eye, Vector3
	*/
	void setAJCameraEye(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return center, Vector3
	*/
	AJ::AJValue ajCameraCenter(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param center, Vector3
	*/
	void setAJCameraCenter(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return up, Vector3
	*/
	AJ::AJValue ajCameraUp(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param up, Vector3
	*/
	void setAJCameraUp(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, Camera
	*/
	AJ::AJValue ajCameraConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
