	const AJOABindingType AJDynamicBinding::s_type = AJDynamicBindingBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJDynamicBinding);
ASSERT_CLASS_FITS_IN_CELL(AJDynamicBindingPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif




	const ClassInfo AJDynamicBinding::s_info = {"DynamicBinding", 0, 0, 0};

	AJObject* AJDynamicBinding::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJDynamicBindingPrototype(AJDynamicBindingPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJDynamicBindingPrototype::AJDynamicBindingPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJDynamicBindingPrototype::s_info = {"DynamicBindingPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
