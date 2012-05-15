	const AJOABindingType AJTexture2D::s_type = AJTexture2DBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJTexture2D);
ASSERT_CLASS_FITS_IN_CELL(AJTexture2DPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJTexture2DConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJTexture2DTableValues[9] = 
	{
		{"pixelWidth", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DPixelWidth), (intptr_t)0 THUNK_GENERATOR(0)},
		{"pixelHeight", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DPixelHeight), (intptr_t)0 THUNK_GENERATOR(0)},
		{"contentSize", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DContentSize), (intptr_t)0 THUNK_GENERATOR(0)},
		{"contentSizeInPixels", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DContentSizeInPixels), (intptr_t)0 THUNK_GENERATOR(0)},
		{"name", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DName), (intptr_t)0 THUNK_GENERATOR(0)},
		{"hasPremultipliedAlpha", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DHasPremultipliedAlpha), (intptr_t)0 THUNK_GENERATOR(0)},
		{"antialias", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DAntialias), (intptr_t)setAJTexture2DAntialias THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTexture2DConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJTexture2DTable = 
	{17, 15, AJTexture2DTableValues, 0};

	static const HashTableValue AJTexture2DPrototypeTableValues[2] = 
	{
		{"toString", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajTexture2DPrototypeFunctionToString), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJTexture2DPrototypeTable = 
	{2, 1, AJTexture2DPrototypeTableValues, 0};


	const ClassInfo AJTexture2D::s_info = {"Texture2D", 0, &AJTexture2DTable, 0};
	
	bool AJTexture2D::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJTexture2D, Base>(exec, &AJTexture2DTable, this, propertyName, slot);
	}
	
	bool AJTexture2D::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJTexture2D, Base>(exec, &AJTexture2DTable, this, propertyName, slot);
	}

	void AJTexture2D::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJTexture2D, Base>(exec, propertyName, value, &AJTexture2DTable, this, slot);
	}

	AJObject* AJTexture2D::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJTexture2DPrototype(AJTexture2DPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJTexture2DPrototype::AJTexture2DPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJTexture2DPrototype::s_info = {"Texture2DPrototype", 0, &AJTexture2DPrototypeTable, 0};
	
	bool AJTexture2DPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJTexture2DPrototypeTable, this, propertyName, slot);
	}
	
	bool AJTexture2DPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJTexture2DPrototypeTable, this, propertyName, descriptor);
	}

	void AJTexture2DPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJTexture2DPrototype, Base>(exec, propertyName, value, &AJTexture2DPrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJTexture2DConstructor::AJTexture2DConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJTexture2DConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJTexture2DPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJTexture2DConstructor::s_info = {"Texture2DConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
