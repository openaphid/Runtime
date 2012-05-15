	const AJOABindingType AJTouch::s_type = AJTouchBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJTouch);
ASSERT_CLASS_FITS_IN_CELL(AJTouchPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJTouchTableValues[9] = 
	{
		{"identifier", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchIdentifier), (intptr_t)0 THUNK_GENERATOR(0)},
		{"target", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchTarget), (intptr_t)0 THUNK_GENERATOR(0)},
		{"screenX", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchScreenX), (intptr_t)0 THUNK_GENERATOR(0)},
		{"screenY", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchScreenY), (intptr_t)0 THUNK_GENERATOR(0)},
		{"clientX", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchClientX), (intptr_t)0 THUNK_GENERATOR(0)},
		{"clientY", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchClientY), (intptr_t)0 THUNK_GENERATOR(0)},
		{"pageX", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchPageX), (intptr_t)0 THUNK_GENERATOR(0)},
		{"pageY", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchPageY), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJTouchTable = 
	{17, 15, AJTouchTableValues, 0};



	const ClassInfo AJTouch::s_info = {"Touch", 0, &AJTouchTable, 0};
	
	bool AJTouch::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJTouch, Base>(exec, &AJTouchTable, this, propertyName, slot);
	}
	
	bool AJTouch::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJTouch, Base>(exec, &AJTouchTable, this, propertyName, slot);
	}

	void AJTouch::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJTouch, Base>(exec, propertyName, value, &AJTouchTable, this, slot);
	}

	AJObject* AJTouch::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJTouchPrototype(AJTouchPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJTouchPrototype::AJTouchPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJTouchPrototype::s_info = {"TouchPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
