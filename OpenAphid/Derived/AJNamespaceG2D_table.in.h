	const AJOABindingType AJNamespaceG2D::s_type = AJNamespaceG2DBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNamespaceG2D);
ASSERT_CLASS_FITS_IN_CELL(AJNamespaceG2DPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJNamespaceG2DTableValues[16] = 
	{
		{"Node", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"ColorNode", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DColorNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"GradientNode", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DGradientNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Scene", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DSceneConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Camera", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DCameraConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Texture2D", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DTexture2DConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Sprite", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DSpriteConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"SpriteBatchNode", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DSpriteBatchNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"SpriteFrame", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DSpriteFrameConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Animation", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DAnimationConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"Font", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DFontConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"LabelTTF", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DLabelTTFConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"ParticleSystem", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DParticleSystemConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"director", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DDirector), (intptr_t)0 THUNK_GENERATOR(0)},
		{"actions", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNamespaceG2DActions), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJNamespaceG2DTable = 
	{34, 31, AJNamespaceG2DTableValues, 0};



	const ClassInfo AJNamespaceG2D::s_info = {"NamespaceG2D", 0, &AJNamespaceG2DTable, 0};
	
	bool AJNamespaceG2D::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJNamespaceG2D, Base>(exec, &AJNamespaceG2DTable, this, propertyName, slot);
	}
	
	bool AJNamespaceG2D::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJNamespaceG2D, Base>(exec, &AJNamespaceG2DTable, this, propertyName, slot);
	}

	void AJNamespaceG2D::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNamespaceG2D, Base>(exec, propertyName, value, &AJNamespaceG2DTable, this, slot);
	}

	AJObject* AJNamespaceG2D::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNamespaceG2DPrototype(AJNamespaceG2DPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceG2DPrototype::AJNamespaceG2DPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNamespaceG2DPrototype::s_info = {"NamespaceG2DPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
