	const AJOABindingType AJSpriteFrame::s_type = AJSpriteFrameBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJSpriteFrame);
ASSERT_CLASS_FITS_IN_CELL(AJSpriteFramePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJSpriteFrameConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJSpriteFrameTableValues[6] = 
	{
		{"rect", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFrameRect), (intptr_t)setAJSpriteFrameRect THUNK_GENERATOR(0)},
		{"rectInPixels", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFrameRectInPixels), (intptr_t)setAJSpriteFrameRectInPixels THUNK_GENERATOR(0)},
		{"rotated", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFrameRotated), (intptr_t)setAJSpriteFrameRotated THUNK_GENERATOR(0)},
		{"texture", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFrameTexture), (intptr_t)setAJSpriteFrameTexture THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFrameConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJSpriteFrameTable = 
	{16, 15, AJSpriteFrameTableValues, 0};



	const ClassInfo AJSpriteFrame::s_info = {"SpriteFrame", 0, &AJSpriteFrameTable, 0};
	
	bool AJSpriteFrame::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJSpriteFrame, Base>(exec, &AJSpriteFrameTable, this, propertyName, slot);
	}
	
	bool AJSpriteFrame::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJSpriteFrame, Base>(exec, &AJSpriteFrameTable, this, propertyName, slot);
	}

	void AJSpriteFrame::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSpriteFrame, Base>(exec, propertyName, value, &AJSpriteFrameTable, this, slot);
	}

	AJObject* AJSpriteFrame::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJSpriteFramePrototype(AJSpriteFramePrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJSpriteFramePrototype::AJSpriteFramePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJSpriteFramePrototype::s_info = {"SpriteFramePrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJSpriteFrameConstructor::AJSpriteFrameConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJSpriteFrameConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJSpriteFramePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJSpriteFrameConstructor::s_info = {"SpriteFrameConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
