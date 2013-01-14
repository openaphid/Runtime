	const AJOABindingType AJConsole::s_type = AJConsoleBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJConsole);
ASSERT_CLASS_FITS_IN_CELL(AJConsolePrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif


	static const HashTableValue AJConsolePrototypeTableValues[8] = 
	{
		{"debug", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionDebug), (intptr_t)1 THUNK_GENERATOR(0)},
		{"error", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionError), (intptr_t)1 THUNK_GENERATOR(0)},
		{"info", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionInfo), (intptr_t)1 THUNK_GENERATOR(0)},
		{"log", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionLog), (intptr_t)1 THUNK_GENERATOR(0)},
		{"warn", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionWarn), (intptr_t)1 THUNK_GENERATOR(0)},
		{"trace", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionTrace), (intptr_t)0 THUNK_GENERATOR(0)},
		{"assert", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajConsolePrototypeFunctionAssert), (intptr_t)2 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJConsolePrototypeTable = 
	{17, 15, AJConsolePrototypeTableValues, 0};


	const ClassInfo AJConsole::s_info = {"Console", 0, 0, 0};

	AJObject* AJConsole::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJConsolePrototype(AJConsolePrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJConsolePrototype::AJConsolePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJConsolePrototype::s_info = {"ConsolePrototype", 0, &AJConsolePrototypeTable, 0};
	
	bool AJConsolePrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJConsolePrototypeTable, this, propertyName, slot);
	}
	
	bool AJConsolePrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJConsolePrototypeTable, this, propertyName, descriptor);
	}

	void AJConsolePrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJConsolePrototype, Base>(exec, propertyName, value, &AJConsolePrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
