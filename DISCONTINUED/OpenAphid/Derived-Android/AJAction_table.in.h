	const AJOABindingType AJAction::s_type = AJActionBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJAction);
ASSERT_CLASS_FITS_IN_CELL(AJActionPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJActionTableValues[6] = 
	{
		{"baseType", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajActionBaseType), (intptr_t)0 THUNK_GENERATOR(0)},
		{"typeName", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajActionTypeName), (intptr_t)0 THUNK_GENERATOR(0)},
		{"isTimeReversed", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajActionIsTimeReversed), (intptr_t)setAJActionIsTimeReversed THUNK_GENERATOR(0)},
		{"copy", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajActionFunctionCopy), (intptr_t)0 THUNK_GENERATOR(0)},
		{"reverse", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajActionFunctionReverse), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJActionTable = 
	{16, 15, AJActionTableValues, 0};



	const ClassInfo AJAction::s_info = {"Action", 0, &AJActionTable, 0};
	
	bool AJAction::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticPropertySlot<AJAction, Base>(exec, &AJActionTable, this, propertyName, slot);
	}
	
	bool AJAction::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticPropertyDescriptor<AJAction, Base>(exec, &AJActionTable, this, propertyName, slot);
	}

	void AJAction::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJAction, Base>(exec, propertyName, value, &AJActionTable, this, slot);
	}

	AJObject* AJAction::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJActionPrototype(AJActionPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJActionPrototype::AJActionPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJActionPrototype::s_info = {"ActionPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
