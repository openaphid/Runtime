	const AJOABindingType AJNamespaceCore::s_type = AJNamespaceCoreBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceCore);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceCorePrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJNamespaceCoreTableValues[8] = 
	{
		{"Size", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCoreSizeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Point", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCorePointConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Rect", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCoreRectConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"AffineTransformation", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCoreAffineTransformationConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Vector2", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCoreVector2Constructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Vector3", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCoreVector3Constructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Color", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceCoreColorConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJNamespaceCoreTable = 
	{17, 15, AJNamespaceCoreTableValues, 0};



	const ClassInfo AJNamespaceCore::s_info = {"NamespaceCore", 0, &AJNamespaceCoreTable, 0};
	
	bool AJNamespaceCore::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJNamespaceCore, Base>(exec, &AJNamespaceCoreTable, this, propertyName, slot);
	}
	
	bool AJNamespaceCore::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJNamespaceCore, Base>(exec, &AJNamespaceCoreTable, this, propertyName, slot);
	}

	void AJNamespaceCore::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceCore, Base>(exec, propertyName, value, &AJNamespaceCoreTable, this, slot);
	}

	AJObject* AJNamespaceCore::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceCorePrototype(AJNamespaceCorePrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceCorePrototype::AJNamespaceCorePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceCorePrototype::s_info = {"NamespaceCorePrototype", 0, 0, 0};



#undef THUNK_GENERATOR
