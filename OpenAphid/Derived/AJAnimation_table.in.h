	const AJOABindingType AJAnimation::s_type = AJAnimationBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJAnimation);
ASSERT_CLASS_FITS_IN_CELL(AJAnimationPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJAnimationConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJAnimationTableValues[3] = 
	{
		{"delay", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAnimationDelay), (intptr_t)setAJAnimationDelay THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajAnimationConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJAnimationTable = 
	{4, 3, AJAnimationTableValues, 0};

	static const HashTableValue AJAnimationPrototypeTableValues[2] = 
	{
		{"addFrame", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajAnimationPrototypeFunctionAddFrame), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJAnimationPrototypeTable = 
	{2, 1, AJAnimationPrototypeTableValues, 0};


	const ClassInfo AJAnimation::s_info = {"Animation", 0, &AJAnimationTable, 0};
	
	bool AJAnimation::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJAnimation, Base>(exec, &AJAnimationTable, this, propertyName, slot);
	}
	
	bool AJAnimation::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJAnimation, Base>(exec, &AJAnimationTable, this, propertyName, slot);
	}

	void AJAnimation::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJAnimation, Base>(exec, propertyName, value, &AJAnimationTable, this, slot);
	}

	AJObject* AJAnimation::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJAnimationPrototype(AJAnimationPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJAnimationPrototype::AJAnimationPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJAnimationPrototype::s_info = {"AnimationPrototype", 0, &AJAnimationPrototypeTable, 0};
	
	bool AJAnimationPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJAnimationPrototypeTable, this, propertyName, slot);
	}
	
	bool AJAnimationPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJAnimationPrototypeTable, this, propertyName, descriptor);
	}

	void AJAnimationPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJAnimationPrototype, Base>(exec, propertyName, value, &AJAnimationPrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJAnimationConstructor::AJAnimationConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJAnimationConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJAnimationPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJAnimationConstructor::s_info = {"AnimationConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
