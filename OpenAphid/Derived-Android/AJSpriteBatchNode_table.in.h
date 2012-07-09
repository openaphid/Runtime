	const AJOABindingType AJSpriteBatchNode::s_type = AJSpriteBatchNodeBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJSpriteBatchNode);
ASSERT_CLASS_FITS_IN_CELL(AJSpriteBatchNodePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJSpriteBatchNodeConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJSpriteBatchNodeTableValues[3] = 
	{
		{"texture", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteBatchNodeTexture), (intptr_t)setAJSpriteBatchNodeTexture THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteBatchNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJSpriteBatchNodeTable = 
	{4, 3, AJSpriteBatchNodeTableValues, 0};

	static const HashTableValue AJSpriteBatchNodePrototypeTableValues[2] = 
	{
		{"addChild", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajSpriteBatchNodePrototypeFunctionAddChild), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJSpriteBatchNodePrototypeTable = 
	{2, 1, AJSpriteBatchNodePrototypeTableValues, 0};


	const ClassInfo AJSpriteBatchNode::s_info = {"SpriteBatchNode", &AJNode::s_info, &AJSpriteBatchNodeTable, 0};
	
	bool AJSpriteBatchNode::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJSpriteBatchNode, Base>(exec, &AJSpriteBatchNodeTable, this, propertyName, slot);
	}
	
	bool AJSpriteBatchNode::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJSpriteBatchNode, Base>(exec, &AJSpriteBatchNodeTable, this, propertyName, slot);
	}

	void AJSpriteBatchNode::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSpriteBatchNode, Base>(exec, propertyName, value, &AJSpriteBatchNodeTable, this, slot);
	}

	AJObject* AJSpriteBatchNode::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJSpriteBatchNodePrototype(AJSpriteBatchNodePrototype::createStructure(AJNodePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJSpriteBatchNodePrototype::AJSpriteBatchNodePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJSpriteBatchNodePrototype::s_info = {"SpriteBatchNodePrototype", 0, &AJSpriteBatchNodePrototypeTable, 0};
	
	bool AJSpriteBatchNodePrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJSpriteBatchNodePrototypeTable, this, propertyName, slot);
	}
	
	bool AJSpriteBatchNodePrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJSpriteBatchNodePrototypeTable, this, propertyName, descriptor);
	}

	void AJSpriteBatchNodePrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSpriteBatchNodePrototype, Base>(exec, propertyName, value, &AJSpriteBatchNodePrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJSpriteBatchNodeConstructor::AJSpriteBatchNodeConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJSpriteBatchNodeConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJSpriteBatchNodePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJSpriteBatchNodeConstructor::s_info = {"SpriteBatchNodeConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
