	const AJOABindingType AJTouchList::s_type = AJTouchListBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJTouchList);
ASSERT_CLASS_FITS_IN_CELL(AJTouchListPrototype);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJTouchListTableValues[4] = 
	{
		{"length", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajTouchListLength), (intptr_t)0 THUNK_GENERATOR(0)},
		{"item", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajTouchListFunctionItem), (intptr_t)1 THUNK_GENERATOR(0)},
		{"identifiedTouch", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajTouchListFunctionIdentifiedTouch), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJTouchListTable = 
	{9, 7, AJTouchListTableValues, 0};



	const ClassInfo AJTouchList::s_info = {"TouchList", 0, &AJTouchListTable, 0};
	
	bool AJTouchList::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticPropertySlot<AJTouchList, Base>(exec, &AJTouchListTable, this, propertyName, slot);
	}
	
	bool AJTouchList::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticPropertyDescriptor<AJTouchList, Base>(exec, &AJTouchListTable, this, propertyName, slot);
	}

	void AJTouchList::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJTouchList, Base>(exec, propertyName, value, &AJTouchListTable, this, slot);
	}

	bool AJTouchList::getOwnPropertySlot(AJ::ExecState* exec, unsigned index, AJ::PropertySlot& slot)
	{
		if (index < impl()->size()) {
			slot.setCustomIndex(this, index, indexGetter);
			return true;
		}
		
		return getOwnPropertySlot(exec, Identifier::from(exec, index), slot);
	}

	AJValue AJTouchList::indexGetter(ExecState* exec, AJValue slotBase, unsigned index)
	{
		AJTouchList* thisObject = ajoa_cast<AJTouchList*>(asObject(slotBase));
		return toAJ(exec, thisObject->globalObject(), thisObject->impl()->at(index));
	}

	AJObject* AJTouchList::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJTouchListPrototype(AJTouchListPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJTouchListPrototype::AJTouchListPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJTouchListPrototype::s_info = {"TouchListPrototype", 0, 0, 0};



#undef THUNK_GENERATOR
