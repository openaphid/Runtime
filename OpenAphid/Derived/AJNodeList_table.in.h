	const AJOABindingType AJNodeList::s_type = AJNodeListBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNodeList);
ASSERT_CLASS_FITS_IN_CELL(AJNodeListPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJNodeListTableValues[2] = 
	{
		{"length", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeListLength), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJNodeListTable = 
	{2, 1, AJNodeListTableValues, 0};

	static const HashTableValue AJNodeListPrototypeTableValues[2] = 
	{
		{"item", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodeListPrototypeFunctionItem), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJNodeListPrototypeTable = 
	{2, 1, AJNodeListPrototypeTableValues, 0};


	const ClassInfo AJNodeList::s_info = {"NodeList", 0, &AJNodeListTable, 0};
	
	bool AJNodeList::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJNodeList, Base>(exec, &AJNodeListTable, this, propertyName, slot);
	}
	
	bool AJNodeList::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJNodeList, Base>(exec, &AJNodeListTable, this, propertyName, slot);
	}

	void AJNodeList::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNodeList, Base>(exec, propertyName, value, &AJNodeListTable, this, slot);
	}

	bool AJNodeList::getOwnPropertySlot(AJ::ExecState* exec, unsigned index, AJ::PropertySlot& slot)
	{
		if (index < impl()->size()) {
			slot.setCustomIndex(this, index, indexGetter);
			return true;
		}
		
		return getOwnPropertySlot(exec, Identifier::from(exec, index), slot);
	}

	AJValue AJNodeList::indexGetter(ExecState* exec, AJValue slotBase, unsigned index)
	{
		AJNodeList* thisObject = ajoa_cast<AJNodeList*>(asObject(slotBase));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->at(index));
	}

	AJObject* AJNodeList::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNodeListPrototype(AJNodeListPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNodeListPrototype::AJNodeListPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNodeListPrototype::s_info = {"NodeListPrototype", 0, &AJNodeListPrototypeTable, 0};
	
	bool AJNodeListPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJNodeListPrototypeTable, this, propertyName, slot);
	}
	
	bool AJNodeListPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJNodeListPrototypeTable, this, propertyName, descriptor);
	}

	void AJNodeListPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNodeListPrototype, Base>(exec, propertyName, value, &AJNodeListPrototypeTable, this, slot);
	}



#undef THUNK_GENERATOR
