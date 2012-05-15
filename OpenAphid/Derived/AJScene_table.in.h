	const AJOABindingType AJScene::s_type = AJSceneBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJScene);
ASSERT_CLASS_FITS_IN_CELL(AJScenePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJSceneConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJSceneTableValues[2] = 
	{
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSceneConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJSceneTable = 
	{2, 1, AJSceneTableValues, 0};



	const ClassInfo AJScene::s_info = {"Scene", &AJNode::s_info, &AJSceneTable, 0};
	
	bool AJScene::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJScene, Base>(exec, &AJSceneTable, this, propertyName, slot);
	}
	
	bool AJScene::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJScene, Base>(exec, &AJSceneTable, this, propertyName, slot);
	}

	void AJScene::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJScene, Base>(exec, propertyName, value, &AJSceneTable, this, slot);
	}

	AJObject* AJScene::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJScenePrototype(AJScenePrototype::createStructure(AJNodePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJScenePrototype::AJScenePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJScenePrototype::s_info = {"ScenePrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJSceneConstructor::AJSceneConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJSceneConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJScenePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJSceneConstructor::s_info = {"SceneConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
