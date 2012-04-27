	const AJOABindingType AJVector3::s_type = AJVector3BindingType;

ASSERT_CLASS_FITS_IN_CELL(AJVector3);
ASSERT_CLASS_FITS_IN_CELL(AJVector3Prototype);
ASSERT_CLASS_FITS_IN_CELL(AJVector3Constructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJVector3TableValues[6] = 
	{
		{"x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector3X), (intptr_t)setAJVector3X THUNK_GENERATOR(0)},
		{"y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector3Y), (intptr_t)setAJVector3Y THUNK_GENERATOR(0)},
		{"z", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector3Z), (intptr_t)setAJVector3Z THUNK_GENERATOR(0)},
		{"setXYZ", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector3FunctionSetXYZ), (intptr_t)3 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector3Constructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJVector3Table = 
	{17, 15, AJVector3TableValues, 0};

	static const HashTableValue AJVector3PrototypeTableValues[4] = 
	{
		{"equals", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector3PrototypeFunctionEquals), (intptr_t)1 THUNK_GENERATOR(0)},
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector3PrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{"copy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajVector3PrototypeFunctionCopy), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJVector3PrototypeTable = 
	{8, 7, AJVector3PrototypeTableValues, 0};

	static const HashTableValue AJVector3ConstructurTableValues[2] =
	{
		{"zero", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajVector3ConstructorZero), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJVector3ConstructorTable = 
	{2, 1, AJVector3ConstructurTableValues, 0};

	const ClassInfo AJVector3::s_info = {"Vector3", 0, &AJVector3Table, 0};
	
	bool AJVector3::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticPropertySlot<AJVector3, Base>(exec, &AJVector3Table, this, propertyName, slot);
	}
	
	bool AJVector3::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticPropertyDescriptor<AJVector3, Base>(exec, &AJVector3Table, this, propertyName, slot);
	}

	void AJVector3::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJVector3, Base>(exec, propertyName, value, &AJVector3Table, this, slot);
	}

	AJObject* AJVector3::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJVector3Prototype(AJVector3Prototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJVector3Prototype::AJVector3Prototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJVector3Prototype::s_info = {"Vector3Prototype", 0, &AJVector3PrototypeTable, 0};
	
	bool AJVector3Prototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJVector3PrototypeTable, this, propertyName, slot);
	}
	
	bool AJVector3Prototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJVector3PrototypeTable, this, propertyName, descriptor);
	}

	void AJVector3Prototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJVector3Prototype, Base>(exec, propertyName, value, &AJVector3PrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJVector3Constructor::AJVector3Constructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJVector3Constructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJVector3Prototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJVector3Constructor::s_info = {"Vector3Constructor", 0, &AJVector3ConstructorTable, 0};
	
	bool AJVector3Constructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJVector3Constructor, Base>(exec, &AJVector3ConstructorTable, this, propertyName, slot);
	}
	
	bool AJVector3Constructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJVector3Constructor, Base>(exec, &AJVector3ConstructorTable, this, propertyName, descriptor);
	}

	void AJVector3Constructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJVector3Constructor, Base>(exec, propertyName, value, &AJVector3ConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
