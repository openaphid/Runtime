	const AJOABindingType AJNamespaceJS::s_type = AJNamespaceJSBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceJS);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceJSPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJNamespaceJSTableValues[3] = 
	{
		{"onload", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceJSOnload), (intptr_t)setAJNamespaceJSOnload THUNK_GENERATOR(0)},
		{"gcFrequency", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceJSGcFrequency), (intptr_t)setAJNamespaceJSGcFrequency THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJNamespaceJSTable = 
	{4, 3, AJNamespaceJSTableValues, 0};

	static const HashTableValue AJNamespaceJSPrototypeTableValues[4] = 
	{
		{"gc", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceJSPrototypeFunctionGc), (intptr_t)0 THUNK_GENERATOR(0)},
		{"includeFile", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceJSPrototypeFunctionIncludeFile), (intptr_t)1 THUNK_GENERATOR(0)},
		{"includeScript", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceJSPrototypeFunctionIncludeScript), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJNamespaceJSPrototypeTable = 
	{9, 7, AJNamespaceJSPrototypeTableValues, 0};


	const ClassInfo AJNamespaceJS::s_info = {"NamespaceJS", 0, &AJNamespaceJSTable, 0};
	
	bool AJNamespaceJS::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJNamespaceJS, Base>(exec, &AJNamespaceJSTable, this, propertyName, slot);
	}
	
	bool AJNamespaceJS::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJNamespaceJS, Base>(exec, &AJNamespaceJSTable, this, propertyName, slot);
	}

	void AJNamespaceJS::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceJS, Base>(exec, propertyName, value, &AJNamespaceJSTable, this, slot);
	}

	AJObject* AJNamespaceJS::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceJSPrototype(AJNamespaceJSPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceJSPrototype::AJNamespaceJSPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceJSPrototype::s_info = {"NamespaceJSPrototype", 0, &AJNamespaceJSPrototypeTable, 0};
	
	bool AJNamespaceJSPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJNamespaceJSPrototypeTable, this, propertyName, slot);
	}
	
	bool AJNamespaceJSPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJNamespaceJSPrototypeTable, this, propertyName, descriptor);
	}

	void AJNamespaceJSPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceJSPrototype, Base>(exec, propertyName, value, &AJNamespaceJSPrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
