	const AJOABindingType AJEaseAction::s_type = AJEaseActionBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJEaseAction);
ASSERT_CLASS_FITS_IN_CELL(AJEaseActionPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif




	const ClassInfo AJEaseAction::s_info = {"EaseAction", &AJIntervalAction::s_info, 0, 0};

	AJObject* AJEaseAction::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJEaseActionPrototype(AJEaseActionPrototype::createStructure(AJIntervalActionPrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJEaseActionPrototype::AJEaseActionPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJEaseActionPrototype::s_info = {"EaseActionPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
