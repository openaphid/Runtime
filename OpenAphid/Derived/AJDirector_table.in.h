	const AJOABindingType AJDirector::s_type = AJDirectorBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJDirector);
ASSERT_CLASS_FITS_IN_CELL(AJDirectorPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJDirectorTableValues[8] = 
	{
		{"winSize", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorWinSize), (intptr_t)0 THUNK_GENERATOR(0)},
		{"winSizeInPixels", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorWinSizeInPixels), (intptr_t)0 THUNK_GENERATOR(0)},
		{"runningScene", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorRunningScene), (intptr_t)0 THUNK_GENERATOR(0)},
		{"contentScaleFactor", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorContentScaleFactor), (intptr_t)setAJDirectorContentScaleFactor THUNK_GENERATOR(0)},
		{"displayFPS", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorDisplayFPS), (intptr_t)setAJDirectorDisplayFPS THUNK_GENERATOR(0)},
		{"fpsInterval", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorFpsInterval), (intptr_t)setAJDirectorFpsInterval THUNK_GENERATOR(0)},
		{"multipleTouchEnabled", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajDirectorMultipleTouchEnabled), (intptr_t)setAJDirectorMultipleTouchEnabled THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJDirectorTable = 
	{17, 15, AJDirectorTableValues, 0};

	static const HashTableValue AJDirectorPrototypeTableValues[3] = 
	{
		{"convertToGL", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajDirectorPrototypeFunctionConvertToGL), (intptr_t)1 THUNK_GENERATOR(0)},
		{"runScene", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajDirectorPrototypeFunctionRunScene), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJDirectorPrototypeTable = 
	{4, 3, AJDirectorPrototypeTableValues, 0};


	const ClassInfo AJDirector::s_info = {"Director", 0, &AJDirectorTable, 0};
	
	bool AJDirector::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJDirector, Base>(exec, &AJDirectorTable, this, propertyName, slot);
	}
	
	bool AJDirector::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJDirector, Base>(exec, &AJDirectorTable, this, propertyName, slot);
	}

	void AJDirector::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJDirector, Base>(exec, propertyName, value, &AJDirectorTable, this, slot);
	}

	AJObject* AJDirector::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJDirectorPrototype(AJDirectorPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJDirectorPrototype::AJDirectorPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJDirectorPrototype::s_info = {"DirectorPrototype", 0, &AJDirectorPrototypeTable, 0};
	
	bool AJDirectorPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJDirectorPrototypeTable, this, propertyName, slot);
	}
	
	bool AJDirectorPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJDirectorPrototypeTable, this, propertyName, descriptor);
	}

	void AJDirectorPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJDirectorPrototype, Base>(exec, propertyName, value, &AJDirectorPrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
