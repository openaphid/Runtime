	const AJOABindingType AJGridBaseAction::s_type = AJGridBaseActionBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJGridBaseAction);
ASSERT_CLASS_FITS_IN_CELL(AJGridBaseActionPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJGridBaseActionTableValues[2] = 
	{
		{"gridSize", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajGridBaseActionGridSize), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJGridBaseActionTable = 
	{2, 1, AJGridBaseActionTableValues, 0};



	const ClassInfo AJGridBaseAction::s_info = {"GridBaseAction", &AJIntervalAction::s_info, &AJGridBaseActionTable, 0};
	
	bool AJGridBaseAction::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJGridBaseAction, Base>(exec, &AJGridBaseActionTable, this, propertyName, slot);
	}
	
	bool AJGridBaseAction::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJGridBaseAction, Base>(exec, &AJGridBaseActionTable, this, propertyName, slot);
	}

	void AJGridBaseAction::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJGridBaseAction, Base>(exec, propertyName, value, &AJGridBaseActionTable, this, slot);
	}

	AJObject* AJGridBaseAction::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJGridBaseActionPrototype(AJGridBaseActionPrototype::createStructure(AJIntervalActionPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJGridBaseActionPrototype::AJGridBaseActionPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJGridBaseActionPrototype::s_info = {"GridBaseActionPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
