	const AJOABindingType AJNamespaceExt::s_type = AJNamespaceExtBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceExt);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceExtPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif




	const ClassInfo AJNamespaceExt::s_info = {"NamespaceExt", 0, 0, 0};

	AJObject* AJNamespaceExt::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceExtPrototype(AJNamespaceExtPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceExtPrototype::AJNamespaceExtPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceExtPrototype::s_info = {"NamespaceExtPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
