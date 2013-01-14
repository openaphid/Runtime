	const AJOABindingType AJNamespaceAndroid::s_type = AJNamespaceAndroidBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceAndroid);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceAndroidPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif




	const ClassInfo AJNamespaceAndroid::s_info = {"NamespaceAndroid", 0, 0, 0};

	AJObject* AJNamespaceAndroid::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceAndroidPrototype(AJNamespaceAndroidPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceAndroidPrototype::AJNamespaceAndroidPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceAndroidPrototype::s_info = {"NamespaceAndroidPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
