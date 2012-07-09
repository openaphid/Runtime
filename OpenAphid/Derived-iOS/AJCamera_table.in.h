	const AJOABindingType AJCamera::s_type = AJCameraBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJCamera);
ASSERT_CLASS_FITS_IN_CELL(AJCameraPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJCameraConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJCameraTableValues[5] = 
	{
		{"eye", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajCameraEye), (intptr_t)setAJCameraEye THUNK_GENERATOR(0)},
		{"center", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajCameraCenter), (intptr_t)setAJCameraCenter THUNK_GENERATOR(0)},
		{"up", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajCameraUp), (intptr_t)setAJCameraUp THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajCameraConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJCameraTable = 
	{8, 7, AJCameraTableValues, 0};



	const ClassInfo AJCamera::s_info = {"Camera", 0, &AJCameraTable, 0};
	
	bool AJCamera::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJCamera, Base>(exec, &AJCameraTable, this, propertyName, slot);
	}
	
	bool AJCamera::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJCamera, Base>(exec, &AJCameraTable, this, propertyName, slot);
	}

	void AJCamera::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJCamera, Base>(exec, propertyName, value, &AJCameraTable, this, slot);
	}

	AJObject* AJCamera::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJCameraPrototype(AJCameraPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJCameraPrototype::AJCameraPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJCameraPrototype::s_info = {"CameraPrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJCameraConstructor::AJCameraConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJCameraConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJCameraPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJCameraConstructor::s_info = {"CameraConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
