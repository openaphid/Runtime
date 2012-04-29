	const AJOABindingType AJProgressEvent::s_type = AJProgressEventBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJProgressEvent);
ASSERT_CLASS_FITS_IN_CELL(AJProgressEventPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJProgressEventTableValues[4] = 
	{
		{"lengthComputable", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajProgressEventLengthComputable), (intptr_t)0 THUNK_GENERATOR(0)},
		{"loaded", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajProgressEventLoaded), (intptr_t)0 THUNK_GENERATOR(0)},
		{"total", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajProgressEventTotal), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJProgressEventTable = 
	{9, 7, AJProgressEventTableValues, 0};



	const ClassInfo AJProgressEvent::s_info = {"ProgressEvent", &AJEvent::s_info, &AJProgressEventTable, 0};
	
	bool AJProgressEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJProgressEvent, Base>(exec, &AJProgressEventTable, this, propertyName, slot);
	}
	
	bool AJProgressEvent::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJProgressEvent, Base>(exec, &AJProgressEventTable, this, propertyName, slot);
	}

	void AJProgressEvent::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJProgressEvent, Base>(exec, propertyName, value, &AJProgressEventTable, this, slot);
	}

	AJObject* AJProgressEvent::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJProgressEventPrototype(AJProgressEventPrototype::createStructure(AJEventPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJProgressEventPrototype::AJProgressEventPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJProgressEventPrototype::s_info = {"ProgressEventPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
