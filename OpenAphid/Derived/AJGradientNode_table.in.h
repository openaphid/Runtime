	const AJOABindingType AJGradientNode::s_type = AJGradientNodeBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJGradientNode);
ASSERT_CLASS_FITS_IN_CELL(AJGradientNodePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJGradientNodeConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJGradientNodeTableValues[6] = 
	{
		{"startColor", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajGradientNodeStartColor), (intptr_t)setAJGradientNodeStartColor THUNK_GENERATOR(0)},
		{"endColor", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajGradientNodeEndColor), (intptr_t)setAJGradientNodeEndColor THUNK_GENERATOR(0)},
		{"vector", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajGradientNodeVector), (intptr_t)setAJGradientNodeVector THUNK_GENERATOR(0)},
		{"compressedInterpolation", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajGradientNodeCompressedInterpolation), (intptr_t)setAJGradientNodeCompressedInterpolation THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajGradientNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJGradientNodeTable = 
	{16, 15, AJGradientNodeTableValues, 0};



	const ClassInfo AJGradientNode::s_info = {"GradientNode", &AJNode::s_info, &AJGradientNodeTable, 0};
	
	bool AJGradientNode::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJGradientNode, Base>(exec, &AJGradientNodeTable, this, propertyName, slot);
	}
	
	bool AJGradientNode::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJGradientNode, Base>(exec, &AJGradientNodeTable, this, propertyName, slot);
	}

	void AJGradientNode::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJGradientNode, Base>(exec, propertyName, value, &AJGradientNodeTable, this, slot);
	}

	AJObject* AJGradientNode::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJGradientNodePrototype(AJGradientNodePrototype::createStructure(AJNodePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJGradientNodePrototype::AJGradientNodePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJGradientNodePrototype::s_info = {"GradientNodePrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJGradientNodeConstructor::AJGradientNodeConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJGradientNodeConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJGradientNodePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJGradientNodeConstructor::s_info = {"GradientNodeConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
