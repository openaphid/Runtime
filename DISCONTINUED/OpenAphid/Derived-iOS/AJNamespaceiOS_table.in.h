	const AJOABindingType AJNamespaceiOS::s_type = AJNamespaceiOSBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceiOS);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceiOSPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif


	static const HashTableValue AJNamespaceiOSPrototypeTableValues[2] = 
	{
		{"enableRetinaDisplay", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNamespaceiOSPrototypeFunctionEnableRetinaDisplay), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJNamespaceiOSPrototypeTable = 
	{2, 1, AJNamespaceiOSPrototypeTableValues, 0};


	const ClassInfo AJNamespaceiOS::s_info = {"NamespaceiOS", 0, 0, 0};

	AJObject* AJNamespaceiOS::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceiOSPrototype(AJNamespaceiOSPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceiOSPrototype::AJNamespaceiOSPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceiOSPrototype::s_info = {"NamespaceiOSPrototype", 0, &AJNamespaceiOSPrototypeTable, 0};
	
	bool AJNamespaceiOSPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJNamespaceiOSPrototypeTable, this, propertyName, slot);
	}
	
	bool AJNamespaceiOSPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJNamespaceiOSPrototypeTable, this, propertyName, descriptor);
	}

	void AJNamespaceiOSPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceiOSPrototype, Base>(exec, propertyName, value, &AJNamespaceiOSPrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
