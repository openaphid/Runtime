	const AJOABindingType AJTouchEvent::s_type = AJTouchEventBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJTouchEvent);
ASSERT_CLASS_FITS_IN_CELL(AJTouchEventPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJTouchEventTableValues[8] = 
	{
		{"touches", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventTouches), (intptr_t)0 THUNK_GENERATOR(0)},
		{"targetTouches", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventTargetTouches), (intptr_t)0 THUNK_GENERATOR(0)},
		{"changedTouches", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventChangedTouches), (intptr_t)0 THUNK_GENERATOR(0)},
		{"altKey", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventAltKey), (intptr_t)0 THUNK_GENERATOR(0)},
		{"metaKey", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventMetaKey), (intptr_t)0 THUNK_GENERATOR(0)},
		{"ctrlKey", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventCtrlKey), (intptr_t)0 THUNK_GENERATOR(0)},
		{"shiftKey", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchEventShiftKey), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJTouchEventTable = 
	{18, 15, AJTouchEventTableValues, 0};



	const ClassInfo AJTouchEvent::s_info = {"TouchEvent", &AJEvent::s_info, &AJTouchEventTable, 0};
	
	bool AJTouchEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJTouchEvent, Base>(exec, &AJTouchEventTable, this, propertyName, slot);
	}
	
	bool AJTouchEvent::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJTouchEvent, Base>(exec, &AJTouchEventTable, this, propertyName, slot);
	}

	void AJTouchEvent::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJTouchEvent, Base>(exec, propertyName, value, &AJTouchEventTable, this, slot);
	}

	AJObject* AJTouchEvent::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJTouchEventPrototype(AJTouchEventPrototype::createStructure(AJEventPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJTouchEventPrototype::AJTouchEventPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJTouchEventPrototype::s_info = {"TouchEventPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
