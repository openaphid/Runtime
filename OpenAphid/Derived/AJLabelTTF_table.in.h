	const AJOABindingType AJLabelTTF::s_type = AJLabelTTFBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJLabelTTF);
ASSERT_CLASS_FITS_IN_CELL(AJLabelTTFPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJLabelTTFConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJLabelTTFTableValues[3] = 
	{
		{"text", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajLabelTTFText), (intptr_t)setAJLabelTTFText THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajLabelTTFConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJLabelTTFTable = 
	{4, 3, AJLabelTTFTableValues, 0};



	const ClassInfo AJLabelTTF::s_info = {"LabelTTF", &AJSprite::s_info, &AJLabelTTFTable, 0};
	
	bool AJLabelTTF::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJLabelTTF, Base>(exec, &AJLabelTTFTable, this, propertyName, slot);
	}
	
	bool AJLabelTTF::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJLabelTTF, Base>(exec, &AJLabelTTFTable, this, propertyName, slot);
	}

	void AJLabelTTF::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJLabelTTF, Base>(exec, propertyName, value, &AJLabelTTFTable, this, slot);
	}

	AJObject* AJLabelTTF::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJLabelTTFPrototype(AJLabelTTFPrototype::createStructure(AJSpritePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJLabelTTFPrototype::AJLabelTTFPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJLabelTTFPrototype::s_info = {"LabelTTFPrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJLabelTTFConstructor::AJLabelTTFConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJLabelTTFConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJLabelTTFPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJLabelTTFConstructor::s_info = {"LabelTTFConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
