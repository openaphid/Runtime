	const AJOABindingType AJSize::s_type = AJSizeBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJSize);
ASSERT_CLASS_FITS_IN_CELL(AJSizePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJSizeConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJSizeTableValues[5] = 
	{
		{"width", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSizeWidth), (intptr_t)setAJSizeWidth THUNK_GENERATOR(0)},
		{"height", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSizeHeight), (intptr_t)setAJSizeHeight THUNK_GENERATOR(0)},
		{"setWH", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajSizeFunctionSetWH), (intptr_t)2 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSizeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJSizeTable = 
	{10, 7, AJSizeTableValues, 0};

	static const HashTableValue AJSizePrototypeTableValues[4] = 
	{
		{"equals", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajSizePrototypeFunctionEquals), (intptr_t)1 THUNK_GENERATOR(0)},
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajSizePrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{"copy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajSizePrototypeFunctionCopy), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJSizePrototypeTable = 
	{8, 7, AJSizePrototypeTableValues, 0};

	static const HashTableValue AJSizeConstructurTableValues[2] =
	{
		{"zero", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSizeConstructorZero), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJSizeConstructorTable = 
	{2, 1, AJSizeConstructurTableValues, 0};

	const ClassInfo AJSize::s_info = {"Size", 0, &AJSizeTable, 0};
	
	bool AJSize::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticPropertySlot<AJSize, Base>(exec, &AJSizeTable, this, propertyName, slot);
	}
	
	bool AJSize::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticPropertyDescriptor<AJSize, Base>(exec, &AJSizeTable, this, propertyName, slot);
	}

	void AJSize::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSize, Base>(exec, propertyName, value, &AJSizeTable, this, slot);
	}

	AJObject* AJSize::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJSizePrototype(AJSizePrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJSizePrototype::AJSizePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJSizePrototype::s_info = {"SizePrototype", 0, &AJSizePrototypeTable, 0};
	
	bool AJSizePrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJSizePrototypeTable, this, propertyName, slot);
	}
	
	bool AJSizePrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJSizePrototypeTable, this, propertyName, descriptor);
	}

	void AJSizePrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSizePrototype, Base>(exec, propertyName, value, &AJSizePrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJSizeConstructor::AJSizeConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJSizeConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJSizePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJSizeConstructor::s_info = {"SizeConstructor", 0, &AJSizeConstructorTable, 0};
	
	bool AJSizeConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJSizeConstructor, Base>(exec, &AJSizeConstructorTable, this, propertyName, slot);
	}
	
	bool AJSizeConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJSizeConstructor, Base>(exec, &AJSizeConstructorTable, this, propertyName, descriptor);
	}

	void AJSizeConstructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSizeConstructor, Base>(exec, propertyName, value, &AJSizeConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
