	const AJOABindingType AJVector2::s_type = AJVector2BindingType;

ASSERT_CLASS_FITS_IN_CELL(AJVector2);
ASSERT_CLASS_FITS_IN_CELL(AJVector2Prototype);
ASSERT_CLASS_FITS_IN_CELL(AJVector2Constructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJVector2TableValues[5] = 
	{
		{"x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector2X), (intptr_t)setAJVector2X THUNK_GENERATOR(0)},
		{"y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector2Y), (intptr_t)setAJVector2Y THUNK_GENERATOR(0)},
		{"setXY", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector2FunctionSetXY), (intptr_t)2 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector2Constructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJVector2Table = 
	{9, 7, AJVector2TableValues, 0};

	static const HashTableValue AJVector2PrototypeTableValues[4] = 
	{
		{"equals", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector2PrototypeFunctionEquals), (intptr_t)1 THUNK_GENERATOR(0)},
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector2PrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{"copy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector2PrototypeFunctionCopy), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJVector2PrototypeTable = 
	{8, 7, AJVector2PrototypeTableValues, 0};

	static const HashTableValue AJVector2ConstructurTableValues[2] =
	{
		{"zero", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector2ConstructorZero), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJVector2ConstructorTable = 
	{2, 1, AJVector2ConstructurTableValues, 0};

	const ClassInfo AJVector2::s_info = {"Vector2", 0, &AJVector2Table, 0};
	
	bool AJVector2::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticPropertySlot<AJVector2, Base>(exec, &AJVector2Table, this, propertyName, slot);
	}
	
	bool AJVector2::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticPropertyDescriptor<AJVector2, Base>(exec, &AJVector2Table, this, propertyName, slot);
	}

	void AJVector2::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJVector2, Base>(exec, propertyName, value, &AJVector2Table, this, slot);
	}

	AJObject* AJVector2::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJVector2Prototype(AJVector2Prototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJVector2Prototype::AJVector2Prototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJVector2Prototype::s_info = {"Vector2Prototype", 0, &AJVector2PrototypeTable, 0};
	
	bool AJVector2Prototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJVector2PrototypeTable, this, propertyName, slot);
	}
	
	bool AJVector2Prototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJVector2PrototypeTable, this, propertyName, descriptor);
	}

	void AJVector2Prototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJVector2Prototype, Base>(exec, propertyName, value, &AJVector2PrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJVector2Constructor::AJVector2Constructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJVector2Constructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJVector2Prototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJVector2Constructor::s_info = {"Vector2Constructor", 0, &AJVector2ConstructorTable, 0};
	
	bool AJVector2Constructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJVector2Constructor, Base>(exec, &AJVector2ConstructorTable, this, propertyName, slot);
	}
	
	bool AJVector2Constructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJVector2Constructor, Base>(exec, &AJVector2ConstructorTable, this, propertyName, descriptor);
	}

	void AJVector2Constructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJVector2Constructor, Base>(exec, propertyName, value, &AJVector2ConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
