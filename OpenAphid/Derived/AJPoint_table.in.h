	const AJOABindingType AJPoint::s_type = AJPointBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJPoint);
ASSERT_CLASS_FITS_IN_CELL(AJPointPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJPointConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJPointTableValues[5] = 
	{
		{"x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajPointX), (intptr_t)setAJPointX THUNK_GENERATOR(0)},
		{"y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajPointY), (intptr_t)setAJPointY THUNK_GENERATOR(0)},
		{"setXY", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajPointFunctionSetXY), (intptr_t)2 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajPointConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJPointTable = 
	{9, 7, AJPointTableValues, 0};

	static const HashTableValue AJPointPrototypeTableValues[4] = 
	{
		{"equals", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajPointPrototypeFunctionEquals), (intptr_t)1 THUNK_GENERATOR(0)},
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajPointPrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{"copy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajPointPrototypeFunctionCopy), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJPointPrototypeTable = 
	{8, 7, AJPointPrototypeTableValues, 0};

	static const HashTableValue AJPointConstructurTableValues[2] =
	{
		{"zero", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajPointConstructorZero), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJPointConstructorTable = 
	{2, 1, AJPointConstructurTableValues, 0};

	const ClassInfo AJPoint::s_info = {"Point", 0, &AJPointTable, 0};
	
	bool AJPoint::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticPropertySlot<AJPoint, Base>(exec, &AJPointTable, this, propertyName, slot);
	}
	
	bool AJPoint::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticPropertyDescriptor<AJPoint, Base>(exec, &AJPointTable, this, propertyName, slot);
	}

	void AJPoint::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJPoint, Base>(exec, propertyName, value, &AJPointTable, this, slot);
	}

	AJObject* AJPoint::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJPointPrototype(AJPointPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJPointPrototype::AJPointPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJPointPrototype::s_info = {"PointPrototype", 0, &AJPointPrototypeTable, 0};
	
	bool AJPointPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJPointPrototypeTable, this, propertyName, slot);
	}
	
	bool AJPointPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJPointPrototypeTable, this, propertyName, descriptor);
	}

	void AJPointPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJPointPrototype, Base>(exec, propertyName, value, &AJPointPrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJPointConstructor::AJPointConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJPointConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJPointPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJPointConstructor::s_info = {"PointConstructor", 0, &AJPointConstructorTable, 0};
	
	bool AJPointConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJPointConstructor, Base>(exec, &AJPointConstructorTable, this, propertyName, slot);
	}
	
	bool AJPointConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJPointConstructor, Base>(exec, &AJPointConstructorTable, this, propertyName, descriptor);
	}

	void AJPointConstructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJPointConstructor, Base>(exec, propertyName, value, &AJPointConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
