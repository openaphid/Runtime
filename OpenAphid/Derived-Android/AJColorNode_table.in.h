	const AJOABindingType AJColorNode::s_type = AJColorNodeBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJColorNode);
ASSERT_CLASS_FITS_IN_CELL(AJColorNodePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJColorNodeConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJColorNodeTableValues[4] = 
	{
		{"color", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorNodeColor), (intptr_t)setAJColorNodeColor THUNK_GENERATOR(0)},
		{"opacity", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorNodeOpacity), (intptr_t)setAJColorNodeOpacity THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajColorNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJColorNodeTable = 
	{9, 7, AJColorNodeTableValues, 0};



	const ClassInfo AJColorNode::s_info = {"ColorNode", &AJNode::s_info, &AJColorNodeTable, 0};
	
	bool AJColorNode::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJColorNode, Base>(exec, &AJColorNodeTable, this, propertyName, slot);
	}
	
	bool AJColorNode::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJColorNode, Base>(exec, &AJColorNodeTable, this, propertyName, slot);
	}

	void AJColorNode::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJColorNode, Base>(exec, propertyName, value, &AJColorNodeTable, this, slot);
	}

	AJObject* AJColorNode::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJColorNodePrototype(AJColorNodePrototype::createStructure(AJNodePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJColorNodePrototype::AJColorNodePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJColorNodePrototype::s_info = {"ColorNodePrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJColorNodeConstructor::AJColorNodeConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJColorNodeConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJColorNodePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJColorNodeConstructor::s_info = {"ColorNodeConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
