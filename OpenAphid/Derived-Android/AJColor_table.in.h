	const AJOABindingType AJColor::s_type = AJColorBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJColor);
ASSERT_CLASS_FITS_IN_CELL(AJColorPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJColorConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJColorTableValues[6] = 
	{
		{"r", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorR), (intptr_t)0 THUNK_GENERATOR(0)},
		{"g", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorG), (intptr_t)0 THUNK_GENERATOR(0)},
		{"b", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorB), (intptr_t)0 THUNK_GENERATOR(0)},
		{"a", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorA), (intptr_t)0 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJColorTable = 
	{16, 15, AJColorTableValues, 0};

	static const HashTableValue AJColorPrototypeTableValues[2] = 
	{
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajColorPrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJColorPrototypeTable = 
	{2, 1, AJColorPrototypeTableValues, 0};


	const ClassInfo AJColor::s_info = {"Color", 0, &AJColorTable, 0};
	
	bool AJColor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJColor, Base>(exec, &AJColorTable, this, propertyName, slot);
	}
	
	bool AJColor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJColor, Base>(exec, &AJColorTable, this, propertyName, slot);
	}

	void AJColor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJColor, Base>(exec, propertyName, value, &AJColorTable, this, slot);
	}

	AJObject* AJColor::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJColorPrototype(AJColorPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJColorPrototype::AJColorPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJColorPrototype::s_info = {"ColorPrototype", 0, &AJColorPrototypeTable, 0};
	
	bool AJColorPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJColorPrototypeTable, this, propertyName, slot);
	}
	
	bool AJColorPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJColorPrototypeTable, this, propertyName, descriptor);
	}

	void AJColorPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJColorPrototype, Base>(exec, propertyName, value, &AJColorPrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJColorConstructor::AJColorConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJColorConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJColorPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJColorConstructor::s_info = {"ColorConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
