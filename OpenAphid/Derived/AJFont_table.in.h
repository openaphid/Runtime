	const AJOABindingType AJFont::s_type = AJFontBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJFont);
ASSERT_CLASS_FITS_IN_CELL(AJFontPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJFontConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJFontTableValues[4] = 
	{
		{"fontName", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajFontFontName), (intptr_t)0 THUNK_GENERATOR(0)},
		{"fontSize", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajFontFontSize), (intptr_t)0 THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajFontConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJFontTable = 
	{9, 7, AJFontTableValues, 0};



	const ClassInfo AJFont::s_info = {"Font", 0, &AJFontTable, 0};
	
	bool AJFont::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		//TODO: a rough guess during code generation
		return getStaticValueSlot<AJFont, Base>(exec, &AJFontTable, this, propertyName, slot);
	}
	
	bool AJFont::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		//TODO: a rough guess during code generation
		return getStaticValueDescriptor<AJFont, Base>(exec, &AJFontTable, this, propertyName, slot);
	}

	void AJFont::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJFont, Base>(exec, propertyName, value, &AJFontTable, this, slot);
	}

	AJObject* AJFont::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJFontPrototype(AJFontPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJFontPrototype::AJFontPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJFontPrototype::s_info = {"FontPrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJFontConstructor::AJFontConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJFontConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJFontPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJFontConstructor::s_info = {"FontConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
