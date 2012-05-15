	const AJOABindingType AJEvent::s_type = AJEventBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJEvent);
ASSERT_CLASS_FITS_IN_CELL(AJEventPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJEventTableValues[8] = 
	{
		{"type", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventType), (intptr_t)0 THUNK_GENERATOR(0)},
		{"target", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventTarget), (intptr_t)0 THUNK_GENERATOR(0)},
		{"currentTarget", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventCurrentTarget), (intptr_t)0 THUNK_GENERATOR(0)},
		{"eventPhase", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventEventPhase), (intptr_t)0 THUNK_GENERATOR(0)},
		{"bubbles", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventBubbles), (intptr_t)0 THUNK_GENERATOR(0)},
		{"cancelable", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventCancelable), (intptr_t)0 THUNK_GENERATOR(0)},
		{"timestamp", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajEventTimestamp), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJEventTable = 
	{17, 15, AJEventTableValues, 0};

	static const HashTableValue AJEventPrototypeTableValues[3] = 
	{
		{"stopPropagation", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajEventPrototypeFunctionStopPropagation), (intptr_t)0 THUNK_GENERATOR(0)},
		{"preventDefault", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajEventPrototypeFunctionPreventDefault), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJEventPrototypeTable = 
	{4, 3, AJEventPrototypeTableValues, 0};


	const ClassInfo AJEvent::s_info = {"Event", 0, &AJEventTable, 0};
	
	bool AJEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJEvent, Base>(exec, &AJEventTable, this, propertyName, slot);
	}
	
	bool AJEvent::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJEvent, Base>(exec, &AJEventTable, this, propertyName, slot);
	}

	void AJEvent::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJEvent, Base>(exec, propertyName, value, &AJEventTable, this, slot);
	}

	AJObject* AJEvent::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJEventPrototype(AJEventPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJEventPrototype::AJEventPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJEventPrototype::s_info = {"EventPrototype", 0, &AJEventPrototypeTable, 0};
	
	bool AJEventPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJEventPrototypeTable, this, propertyName, slot);
	}
	
	bool AJEventPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJEventPrototypeTable, this, propertyName, descriptor);
	}

	void AJEventPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJEventPrototype, Base>(exec, propertyName, value, &AJEventPrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
