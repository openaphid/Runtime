	const AJOABindingType AJSprite::s_type = AJSpriteBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJSprite);
ASSERT_CLASS_FITS_IN_CELL(AJSpritePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJSpriteConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJSpriteTableValues[9] = 
	{
		{"offsetPositionInPixels", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteOffsetPositionInPixels), (intptr_t)0 THUNK_GENERATOR(0)},
		{"textureRect", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteTextureRect), (intptr_t)setAJSpriteTextureRect THUNK_GENERATOR(0)},
		{"flipX", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFlipX), (intptr_t)setAJSpriteFlipX THUNK_GENERATOR(0)},
		{"flipY", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteFlipY), (intptr_t)setAJSpriteFlipY THUNK_GENERATOR(0)},
		{"opacity", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteOpacity), (intptr_t)setAJSpriteOpacity THUNK_GENERATOR(0)},
		{"color", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteColor), (intptr_t)setAJSpriteColor THUNK_GENERATOR(0)},
		{"texture", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteTexture), (intptr_t)setAJSpriteTexture THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJSpriteTable = 
	{17, 15, AJSpriteTableValues, 0};


	static const HashTableValue AJSpriteConstructurTableValues[3] =
	{
		{"debugDrawOutline", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteConstructorDebugDrawOutline), (intptr_t)setAJSpriteConstructorDebugDrawOutline THUNK_GENERATOR(0)},
		{"debugDrawTextureOutline", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajSpriteConstructorDebugDrawTextureOutline), (intptr_t)setAJSpriteConstructorDebugDrawTextureOutline THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJSpriteConstructorTable = 
	{5, 3, AJSpriteConstructurTableValues, 0};

	const ClassInfo AJSprite::s_info = {"Sprite", &AJNode::s_info, &AJSpriteTable, 0};
	
	bool AJSprite::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJSprite, Base>(exec, &AJSpriteTable, this, propertyName, slot);
	}
	
	bool AJSprite::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJSprite, Base>(exec, &AJSpriteTable, this, propertyName, slot);
	}

	void AJSprite::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSprite, Base>(exec, propertyName, value, &AJSpriteTable, this, slot);
	}

	AJObject* AJSprite::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJSpritePrototype(AJSpritePrototype::createStructure(AJNodePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJSpritePrototype::AJSpritePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJSpritePrototype::s_info = {"SpritePrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJSpriteConstructor::AJSpriteConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJSpriteConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJSpritePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJSpriteConstructor::s_info = {"SpriteConstructor", 0, &AJSpriteConstructorTable, 0};
	
	bool AJSpriteConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJSpriteConstructor, Base>(exec, &AJSpriteConstructorTable, this, propertyName, slot);
	}
	
	bool AJSpriteConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJSpriteConstructor, Base>(exec, &AJSpriteConstructorTable, this, propertyName, descriptor);
	}

	void AJSpriteConstructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJSpriteConstructor, Base>(exec, propertyName, value, &AJSpriteConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
