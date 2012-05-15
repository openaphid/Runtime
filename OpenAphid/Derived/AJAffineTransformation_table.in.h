	const AJOABindingType AJAffineTransformation::s_type = AJAffineTransformationBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJAffineTransformation);
ASSERT_CLASS_FITS_IN_CELL(AJAffineTransformationPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJAffineTransformationConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJAffineTransformationTableValues[10] = 
	{
		{"a", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationA), (intptr_t)setAJAffineTransformationA THUNK_GENERATOR(0)},
		{"b", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationB), (intptr_t)setAJAffineTransformationB THUNK_GENERATOR(0)},
		{"c", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationC), (intptr_t)setAJAffineTransformationC THUNK_GENERATOR(0)},
		{"d", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationD), (intptr_t)setAJAffineTransformationD THUNK_GENERATOR(0)},
		{"tx", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationTx), (intptr_t)setAJAffineTransformationTx THUNK_GENERATOR(0)},
		{"ty", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationTy), (intptr_t)setAJAffineTransformationTy THUNK_GENERATOR(0)},
		{"setAll", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajAffineTransformationFunctionSetAll), (intptr_t)6 THUNK_GENERATOR(0)},
		{"clone", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajAffineTransformationFunctionClone), (intptr_t)0 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAffineTransformationConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJAffineTransformationTable = 
	{33, 31, AJAffineTransformationTableValues, 0};



	const ClassInfo AJAffineTransformation::s_info = {"AffineTransformation", 0, &AJAffineTransformationTable, 0};
	
	bool AJAffineTransformation::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticPropertySlot<AJAffineTransformation, Base>(exec, &AJAffineTransformationTable, this, propertyName, slot);
	}
	
	bool AJAffineTransformation::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticPropertyDescriptor<AJAffineTransformation, Base>(exec, &AJAffineTransformationTable, this, propertyName, slot);
	}

	void AJAffineTransformation::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJAffineTransformation, Base>(exec, propertyName, value, &AJAffineTransformationTable, this, slot);
	}

	AJObject* AJAffineTransformation::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJAffineTransformationPrototype(AJAffineTransformationPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJAffineTransformationPrototype::AJAffineTransformationPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJAffineTransformationPrototype::s_info = {"AffineTransformationPrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJAffineTransformationConstructor::AJAffineTransformationConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJAffineTransformationConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJAffineTransformationPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJAffineTransformationConstructor::s_info = {"AffineTransformationConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
