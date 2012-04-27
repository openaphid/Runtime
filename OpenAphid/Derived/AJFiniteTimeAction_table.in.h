	const AJOABindingType AJFiniteTimeAction::s_type = AJFiniteTimeActionBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJFiniteTimeAction);
ASSERT_CLASS_FITS_IN_CELL(AJFiniteTimeActionPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJFiniteTimeActionTableValues[2] = 
	{
		{"duration", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajFiniteTimeActionDuration), (intptr_t)setAJFiniteTimeActionDuration THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJFiniteTimeActionTable = 
	{2, 1, AJFiniteTimeActionTableValues, 0};



	const ClassInfo AJFiniteTimeAction::s_info = {"FiniteTimeAction", &AJAction::s_info, &AJFiniteTimeActionTable, 0};
	
	bool AJFiniteTimeAction::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJFiniteTimeAction, Base>(exec, &AJFiniteTimeActionTable, this, propertyName, slot);
	}
	
	bool AJFiniteTimeAction::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJFiniteTimeAction, Base>(exec, &AJFiniteTimeActionTable, this, propertyName, slot);
	}

	void AJFiniteTimeAction::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJFiniteTimeAction, Base>(exec, propertyName, value, &AJFiniteTimeActionTable, this, slot);
	}

	AJObject* AJFiniteTimeAction::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJFiniteTimeActionPrototype(AJFiniteTimeActionPrototype::createStructure(AJActionPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJFiniteTimeActionPrototype::AJFiniteTimeActionPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJFiniteTimeActionPrototype::s_info = {"FiniteTimeActionPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
