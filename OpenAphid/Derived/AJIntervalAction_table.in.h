	const AJOABindingType AJIntervalAction::s_type = AJIntervalActionBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJIntervalAction);
ASSERT_CLASS_FITS_IN_CELL(AJIntervalActionPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJIntervalActionTableValues[2] = 
	{
		{"elapsed", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajIntervalActionElapsed), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJIntervalActionTable = 
	{2, 1, AJIntervalActionTableValues, 0};



	const ClassInfo AJIntervalAction::s_info = {"IntervalAction", &AJFiniteTimeAction::s_info, &AJIntervalActionTable, 0};
	
	bool AJIntervalAction::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJIntervalAction, Base>(exec, &AJIntervalActionTable, this, propertyName, slot);
	}
	
	bool AJIntervalAction::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJIntervalAction, Base>(exec, &AJIntervalActionTable, this, propertyName, slot);
	}

	void AJIntervalAction::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJIntervalAction, Base>(exec, propertyName, value, &AJIntervalActionTable, this, slot);
	}

	AJObject* AJIntervalAction::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJIntervalActionPrototype(AJIntervalActionPrototype::createStructure(AJFiniteTimeActionPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJIntervalActionPrototype::AJIntervalActionPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJIntervalActionPrototype::s_info = {"IntervalActionPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
