	const AJOABindingType AJNamespaceActions::s_type = AJNamespaceActionsBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceActions);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceActionsPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJNamespaceActionsTableValues[11] = 
	{
		{"TYPE_MASK_DEFAULT", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_DEFAULT), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_FINITE_TIME", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_FINITE_TIME), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_INTERVAL", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_INTERVAL), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_INSTANT", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_INSTANT), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_EASE", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_EASE), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_GRID", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_GRID), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_GRID3D", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_GRID3D), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_TILED_GRID3D", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_TILED_GRID3D), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_CAMERA", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_CAMERA), (intptr_t)0 THUNK_GENERATOR(0)},
		{"TYPE_MASK_CALL_FUNCTION", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceActionsTYPE_MASK_CALL_FUNCTION), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJNamespaceActionsTable = 
	{32, 31, AJNamespaceActionsTableValues, 0};

	static const HashTableValue AJNamespaceActionsPrototypeTableValues[80] = 
	{
		{"repeatForever", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionRepeatForever), (intptr_t)1 THUNK_GENERATOR(0)},
		{"speed", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSpeed), (intptr_t)2 THUNK_GENERATOR(0)},
		{"follow", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFollow), (intptr_t)1 THUNK_GENERATOR(0)},
		{"sequence", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSequence), (intptr_t)2 THUNK_GENERATOR(0)},
		{"repeat", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionRepeat), (intptr_t)2 THUNK_GENERATOR(0)},
		{"spawn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSpawn), (intptr_t)2 THUNK_GENERATOR(0)},
		{"rotateTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionRotateTo), (intptr_t)2 THUNK_GENERATOR(0)},
		{"rotateBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionRotateBy), (intptr_t)2 THUNK_GENERATOR(0)},
		{"moveTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionMoveTo), (intptr_t)2 THUNK_GENERATOR(0)},
		{"moveBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionMoveBy), (intptr_t)2 THUNK_GENERATOR(0)},
		{"skewTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSkewTo), (intptr_t)2 THUNK_GENERATOR(0)},
		{"skewBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSkewBy), (intptr_t)2 THUNK_GENERATOR(0)},
		{"jumpBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionJumpBy), (intptr_t)4 THUNK_GENERATOR(0)},
		{"jumpTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionJumpTo), (intptr_t)4 THUNK_GENERATOR(0)},
		{"bezierBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionBezierBy), (intptr_t)4 THUNK_GENERATOR(0)},
		{"bezierTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionBezierTo), (intptr_t)4 THUNK_GENERATOR(0)},
		{"scaleTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionScaleTo), (intptr_t)2 THUNK_GENERATOR(0)},
		{"scaleBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionScaleBy), (intptr_t)2 THUNK_GENERATOR(0)},
		{"blink", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionBlink), (intptr_t)2 THUNK_GENERATOR(0)},
		{"fadeIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeIn), (intptr_t)1 THUNK_GENERATOR(0)},
		{"fadeOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"fadeTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeTo), (intptr_t)2 THUNK_GENERATOR(0)},
		{"tintTo", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionTintTo), (intptr_t)4 THUNK_GENERATOR(0)},
		{"tintBy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionTintBy), (intptr_t)4 THUNK_GENERATOR(0)},
		{"delay", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionDelay), (intptr_t)1 THUNK_GENERATOR(0)},
		{"animate", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionAnimate), (intptr_t)1 THUNK_GENERATOR(0)},
		{"orbitCamera", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionOrbitCamera), (intptr_t)7 THUNK_GENERATOR(0)},
		{"easeBackIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseBackIn), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeBackOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseBackOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeBackInOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseBackInOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeBounceIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseBounceIn), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeBounceOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseBounceOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeBounceInOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseBounceInOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeElasticIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseElasticIn), (intptr_t)2 THUNK_GENERATOR(0)},
		{"easeElasticOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseElasticOut), (intptr_t)2 THUNK_GENERATOR(0)},
		{"easeElasticInOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseElasticInOut), (intptr_t)2 THUNK_GENERATOR(0)},
		{"easeExponentialIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseExponentialIn), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeExponentialOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseExponentialOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeExponentialInOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseExponentialInOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseIn), (intptr_t)2 THUNK_GENERATOR(0)},
		{"easeOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseOut), (intptr_t)2 THUNK_GENERATOR(0)},
		{"easeInOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseInOut), (intptr_t)2 THUNK_GENERATOR(0)},
		{"easeSineIn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseSineIn), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeSineOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseSineOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"easeSineInOut", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionEaseSineInOut), (intptr_t)1 THUNK_GENERATOR(0)},
		{"accelDeccelAmplitude", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionAccelDeccelAmplitude), (intptr_t)2 THUNK_GENERATOR(0)},
		{"accelAmplitude", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionAccelAmplitude), (intptr_t)2 THUNK_GENERATOR(0)},
		{"deccelAmplitude", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionDeccelAmplitude), (intptr_t)2 THUNK_GENERATOR(0)},
		{"stopGrid", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionStopGrid), (intptr_t)0 THUNK_GENERATOR(0)},
		{"reuseGrid", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionReuseGrid), (intptr_t)1 THUNK_GENERATOR(0)},
		{"waves3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionWaves3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"flipX3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFlipX3D), (intptr_t)1 THUNK_GENERATOR(0)},
		{"flipY3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFlipY3D), (intptr_t)1 THUNK_GENERATOR(0)},
		{"lens3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionLens3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"ripple3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionRipple3D), (intptr_t)6 THUNK_GENERATOR(0)},
		{"shaky3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionShaky3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"liquid", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionLiquid), (intptr_t)4 THUNK_GENERATOR(0)},
		{"waves", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionWaves), (intptr_t)6 THUNK_GENERATOR(0)},
		{"twirl", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionTwirl), (intptr_t)5 THUNK_GENERATOR(0)},
		{"pageTurn3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionPageTurn3D), (intptr_t)2 THUNK_GENERATOR(0)},
		{"shakyTiles3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionShakyTiles3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"shatteredTiles3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionShatteredTiles3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"shuffleTiles", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionShuffleTiles), (intptr_t)3 THUNK_GENERATOR(0)},
		{"fadeOutTRTiles", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeOutTRTiles), (intptr_t)2 THUNK_GENERATOR(0)},
		{"fadeOutBLTiles", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeOutBLTiles), (intptr_t)2 THUNK_GENERATOR(0)},
		{"fadeOutUpTiles", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeOutUpTiles), (intptr_t)2 THUNK_GENERATOR(0)},
		{"fadeOutDownTiles", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFadeOutDownTiles), (intptr_t)2 THUNK_GENERATOR(0)},
		{"turnOffTiles", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionTurnOffTiles), (intptr_t)3 THUNK_GENERATOR(0)},
		{"wavesTiles3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionWavesTiles3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"jumpTiles3D", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionJumpTiles3D), (intptr_t)4 THUNK_GENERATOR(0)},
		{"splitRows", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSplitRows), (intptr_t)2 THUNK_GENERATOR(0)},
		{"splitColumns", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionSplitColumns), (intptr_t)2 THUNK_GENERATOR(0)},
		{"show", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionShow), (intptr_t)0 THUNK_GENERATOR(0)},
		{"hide", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionHide), (intptr_t)0 THUNK_GENERATOR(0)},
		{"toggleVisible", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionToggleVisible), (intptr_t)0 THUNK_GENERATOR(0)},
		{"flipX", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFlipX), (intptr_t)1 THUNK_GENERATOR(0)},
		{"flipY", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionFlipY), (intptr_t)1 THUNK_GENERATOR(0)},
		{"place", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionPlace), (intptr_t)1 THUNK_GENERATOR(0)},
		{"callFunction", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceActionsPrototypeFunctionCallFunction), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJNamespaceActionsPrototypeTable = 
	{273, 255, AJNamespaceActionsPrototypeTableValues, 0};


	const ClassInfo AJNamespaceActions::s_info = {"NamespaceActions", 0, &AJNamespaceActionsTable, 0};
	
	bool AJNamespaceActions::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJNamespaceActions, Base>(exec, &AJNamespaceActionsTable, this, propertyName, slot);
	}
	
	bool AJNamespaceActions::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJNamespaceActions, Base>(exec, &AJNamespaceActionsTable, this, propertyName, slot);
	}

	void AJNamespaceActions::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceActions, Base>(exec, propertyName, value, &AJNamespaceActionsTable, this, slot);
	}

	AJObject* AJNamespaceActions::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceActionsPrototype(AJNamespaceActionsPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceActionsPrototype::AJNamespaceActionsPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceActionsPrototype::s_info = {"NamespaceActionsPrototype", 0, &AJNamespaceActionsPrototypeTable, 0};
	
	bool AJNamespaceActionsPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJNamespaceActionsPrototypeTable, this, propertyName, slot);
	}
	
	bool AJNamespaceActionsPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJNamespaceActionsPrototypeTable, this, propertyName, descriptor);
	}

	void AJNamespaceActionsPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceActionsPrototype, Base>(exec, propertyName, value, &AJNamespaceActionsPrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
