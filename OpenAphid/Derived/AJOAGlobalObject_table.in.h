
ASSERT_CLASS_FITS_IN_CELL(AJOAGlobalObject);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJOAGlobalObjectTableValues[4] = 
	{
		{"console", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajOAGlobalObjectConsole), (intptr_t)0 THUNK_GENERATOR(0)},
		{"aphid", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajOAGlobalObjectAphid), (intptr_t)0 THUNK_GENERATOR(0)},
		{"XMLHttpRequest", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajOAGlobalObjectXMLHttpRequestConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJOAGlobalObjectTable = 
	{9, 7, AJOAGlobalObjectTableValues, 0};



	const ClassInfo AJOAGlobalObject::s_info = {"OAGlobalObject", 0, &AJOAGlobalObjectTable, 0};
	
	bool AJOAGlobalObject::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJOAGlobalObject, Base>(exec, &AJOAGlobalObjectTable, this, propertyName, slot);
	}
	
	bool AJOAGlobalObject::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJOAGlobalObject, Base>(exec, &AJOAGlobalObjectTable, this, propertyName, slot);
	}

	void AJOAGlobalObject::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJOAGlobalObject, Base>(exec, propertyName, value, &AJOAGlobalObjectTable, this, slot);
	}


#undef THUNK_GENERATOR
