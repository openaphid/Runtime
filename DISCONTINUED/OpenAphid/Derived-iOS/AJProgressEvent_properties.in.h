	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return lengthComputable, bool
	*/
	AJ::AJValue ajProgressEventLengthComputable(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return loaded, int
	*/
	AJ::AJValue ajProgressEventLoaded(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return total, int
	*/
	AJ::AJValue ajProgressEventTotal(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
