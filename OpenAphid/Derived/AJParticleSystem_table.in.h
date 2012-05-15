	const AJOABindingType AJParticleSystem::s_type = AJParticleSystemBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJParticleSystem);
ASSERT_CLASS_FITS_IN_CELL(AJParticleSystemPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJParticleSystemConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJParticleSystemTableValues[4] = 
	{
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajParticleSystemConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{"texture", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajParticleSystemTexture), (intptr_t)setAJParticleSystemTexture THUNK_GENERATOR(0)},
		{"blendAdditive", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajParticleSystemBlendAdditive), (intptr_t)setAJParticleSystemBlendAdditive THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJParticleSystemTable = 
	{9, 7, AJParticleSystemTableValues, 0};



	const ClassInfo AJParticleSystem::s_info = {"ParticleSystem", &AJNode::s_info, &AJParticleSystemTable, 0};
	
	bool AJParticleSystem::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJParticleSystem, Base>(exec, &AJParticleSystemTable, this, propertyName, slot);
	}
	
	bool AJParticleSystem::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJParticleSystem, Base>(exec, &AJParticleSystemTable, this, propertyName, slot);
	}

	void AJParticleSystem::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJParticleSystem, Base>(exec, propertyName, value, &AJParticleSystemTable, this, slot);
	}

	AJObject* AJParticleSystem::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		AJOAGlobalObject* castedGlobal = ajoa_cast<AJOAGlobalObject*>(globalObject);
		return new(exec)AJParticleSystemPrototype(AJParticleSystemPrototype::createStructure(AJNodePrototype::self(exec, castedGlobal)));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJParticleSystemPrototype::AJParticleSystemPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJParticleSystemPrototype::s_info = {"ParticleSystemPrototype", 0, 0, 0};

	///-------------------------------------------------------------------------------------------------------------------
	AJParticleSystemConstructor::AJParticleSystemConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJParticleSystemConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJParticleSystemPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJParticleSystemConstructor::s_info = {"ParticleSystemConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
