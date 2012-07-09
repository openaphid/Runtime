	const AJOABindingType AJInstantAction::s_type = AJInstantActionBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJInstantAction);
ASSERT_CLASS_FITS_IN_CELL(AJInstantActionPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif




	const ClassInfo AJInstantAction::s_info = {"InstantAction", &AJFiniteTimeAction::s_info, 0, 0};

	AJObject* AJInstantAction::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJInstantActionPrototype(AJInstantActionPrototype::createStructure(AJFiniteTimeActionPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJInstantActionPrototype::AJInstantActionPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJInstantActionPrototype::s_info = {"InstantActionPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
