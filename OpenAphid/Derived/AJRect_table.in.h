	const AJOABindingType AJRect::s_type = AJRectBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJRect);
ASSERT_CLASS_FITS_IN_CELL(AJRectPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJRectConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJRectTableValues[9] = 
	{
		{"x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectX), (intptr_t)setAJRectX THUNK_GENERATOR(0)},
		{"y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectY), (intptr_t)setAJRectY THUNK_GENERATOR(0)},
		{"width", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectWidth), (intptr_t)setAJRectWidth THUNK_GENERATOR(0)},
		{"height", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectHeight), (intptr_t)setAJRectHeight THUNK_GENERATOR(0)},
		{"origin", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectOrigin), (intptr_t)setAJRectOrigin THUNK_GENERATOR(0)},
		{"size", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectSize), (intptr_t)setAJRectSize THUNK_GENERATOR(0)},
		{"setXYWH", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajRectFunctionSetXYWH), (intptr_t)4 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJRectTable = 
	{18, 15, AJRectTableValues, 0};

	static const HashTableValue AJRectPrototypeTableValues[4] = 
	{
		{"equals", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajRectPrototypeFunctionEquals), (intptr_t)1 THUNK_GENERATOR(0)},
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajRectPrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{"copy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajRectPrototypeFunctionCopy), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJRectPrototypeTable = 
	{8, 7, AJRectPrototypeTableValues, 0};

	static const HashTableValue AJRectConstructurTableValues[2] =
	{
		{"zero", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajRectConstructorZero), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJRectConstructorTable = 
	{2, 1, AJRectConstructurTableValues, 0};

	const ClassInfo AJRect::s_info = {"Rect", 0, &AJRectTable, 0};
	
	bool AJRect::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticPropertySlot<AJRect, Base>(exec, &AJRectTable, this, propertyName, slot);
	}
	
	bool AJRect::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticPropertyDescriptor<AJRect, Base>(exec, &AJRectTable, this, propertyName, slot);
	}

	void AJRect::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJRect, Base>(exec, propertyName, value, &AJRectTable, this, slot);
	}

	AJObject* AJRect::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJRectPrototype(AJRectPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJRectPrototype::AJRectPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJRectPrototype::s_info = {"RectPrototype", 0, &AJRectPrototypeTable, 0};
	
	bool AJRectPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJRectPrototypeTable, this, propertyName, slot);
	}
	
	bool AJRectPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJRectPrototypeTable, this, propertyName, descriptor);
	}

	void AJRectPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJRectPrototype, Base>(exec, propertyName, value, &AJRectPrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJRectConstructor::AJRectConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJRectConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJRectPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJRectConstructor::s_info = {"RectConstructor", 0, &AJRectConstructorTable, 0};
	
	bool AJRectConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJRectConstructor, Base>(exec, &AJRectConstructorTable, this, propertyName, slot);
	}
	
	bool AJRectConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJRectConstructor, Base>(exec, &AJRectConstructorTable, this, propertyName, descriptor);
	}

	void AJRectConstructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJRectConstructor, Base>(exec, propertyName, value, &AJRectConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
