	const AJOABindingType AJNode::s_type = AJNodeBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJNode);
ASSERT_CLASS_FITS_IN_CELL(AJNodePrototype);
ASSERT_CLASS_FITS_IN_CELL(AJNodeConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJNodeTableValues[28] = 
	{
		{"zOrder", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeZOrder), (intptr_t)0 THUNK_GENERATOR(0)},
		{"camera", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeCamera), (intptr_t)0 THUNK_GENERATOR(0)},
		{"children", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeChildren), (intptr_t)0 THUNK_GENERATOR(0)},
		{"anchorInPixels", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeAnchorInPixels), (intptr_t)0 THUNK_GENERATOR(0)},
		{"isRunning", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeIsRunning), (intptr_t)0 THUNK_GENERATOR(0)},
		{"parent", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeParent), (intptr_t)0 THUNK_GENERATOR(0)},
		{"boundingBox", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeBoundingBox), (intptr_t)0 THUNK_GENERATOR(0)},
		{"boundingBoxInPixels", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeBoundingBoxInPixels), (intptr_t)0 THUNK_GENERATOR(0)},
		{"skew", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeSkew), (intptr_t)setAJNodeSkew THUNK_GENERATOR(0)},
		{"rotation", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeRotation), (intptr_t)setAJNodeRotation THUNK_GENERATOR(0)},
		{"scale", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeScale), (intptr_t)setAJNodeScale THUNK_GENERATOR(0)},
		{"position", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodePosition), (intptr_t)setAJNodePosition THUNK_GENERATOR(0)},
		{"positionInPixels", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodePositionInPixels), (intptr_t)setAJNodePositionInPixels THUNK_GENERATOR(0)},
		{"visible", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeVisible), (intptr_t)setAJNodeVisible THUNK_GENERATOR(0)},
		{"anchor", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeAnchor), (intptr_t)setAJNodeAnchor THUNK_GENERATOR(0)},
		{"contentSize", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeContentSize), (intptr_t)setAJNodeContentSize THUNK_GENERATOR(0)},
		{"contentSizeInPixels", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeContentSizeInPixels), (intptr_t)setAJNodeContentSizeInPixels THUNK_GENERATOR(0)},
		{"isRelativeAnchor", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeIsRelativeAnchor), (intptr_t)setAJNodeIsRelativeAnchor THUNK_GENERATOR(0)},
		{"tag", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeTag), (intptr_t)setAJNodeTag THUNK_GENERATOR(0)},
		{"onframeupdate", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeOnframeupdate), (intptr_t)setAJNodeOnframeupdate THUNK_GENERATOR(0)},
		{"ontouchstart", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeOntouchstart), (intptr_t)setAJNodeOntouchstart THUNK_GENERATOR(0)},
		{"ontouchmove", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeOntouchmove), (intptr_t)setAJNodeOntouchmove THUNK_GENERATOR(0)},
		{"ontouchend", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeOntouchend), (intptr_t)setAJNodeOntouchend THUNK_GENERATOR(0)},
		{"ontouchcancel", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeOntouchcancel), (intptr_t)setAJNodeOntouchcancel THUNK_GENERATOR(0)},
		{"touchEnabled", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeTouchEnabled), (intptr_t)setAJNodeTouchEnabled THUNK_GENERATOR(0)},
		{"userInteractionEnabled", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeUserInteractionEnabled), (intptr_t)setAJNodeUserInteractionEnabled THUNK_GENERATOR(0)},
		{"constructor", ReadOnly|DontEnum|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeConstructor), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJNodeTable = 
	{66, 63, AJNodeTableValues, 0};

	static const HashTableValue AJNodePrototypeTableValues[23] = 
	{
		{"addChild", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionAddChild), (intptr_t)1 THUNK_GENERATOR(0)},
		{"removeFromParent", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionRemoveFromParent), (intptr_t)0 THUNK_GENERATOR(0)},
		{"removeChild", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionRemoveChild), (intptr_t)1 THUNK_GENERATOR(0)},
		{"removeChildByTag", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionRemoveChildByTag), (intptr_t)1 THUNK_GENERATOR(0)},
		{"removeAllChildren", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionRemoveAllChildren), (intptr_t)0 THUNK_GENERATOR(0)},
		{"getChildByTag", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionGetChildByTag), (intptr_t)1 THUNK_GENERATOR(0)},
		{"reorderChild", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionReorderChild), (intptr_t)2 THUNK_GENERATOR(0)},
		{"cleanup", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionCleanup), (intptr_t)0 THUNK_GENERATOR(0)},
		{"runAction", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionRunAction), (intptr_t)1 THUNK_GENERATOR(0)},
		{"stopAllActions", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionStopAllActions), (intptr_t)0 THUNK_GENERATOR(0)},
		{"scheduleTimer", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionScheduleTimer), (intptr_t)2 THUNK_GENERATOR(0)},
		{"unscheduleTimer", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionUnscheduleTimer), (intptr_t)1 THUNK_GENERATOR(0)},
		{"unscheduleAllTimers", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionUnscheduleAllTimers), (intptr_t)0 THUNK_GENERATOR(0)},
		{"scheduleUpdate", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionScheduleUpdate), (intptr_t)0 THUNK_GENERATOR(0)},
		{"unscheduleUpdate", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionUnscheduleUpdate), (intptr_t)0 THUNK_GENERATOR(0)},
		{"convertToNodeSpace", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionConvertToNodeSpace), (intptr_t)1 THUNK_GENERATOR(0)},
		{"convertParentToNodeSpace", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionConvertParentToNodeSpace), (intptr_t)1 THUNK_GENERATOR(0)},
		{"convertToWorldSpace", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionConvertToWorldSpace), (intptr_t)1 THUNK_GENERATOR(0)},
		{"convertToNodeSpaceAR", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionConvertToNodeSpaceAR), (intptr_t)1 THUNK_GENERATOR(0)},
		{"convertToWorldSpaceAR", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionConvertToWorldSpaceAR), (intptr_t)1 THUNK_GENERATOR(0)},
		{"locationOfTouch", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionLocationOfTouch), (intptr_t)1 THUNK_GENERATOR(0)},
		{"locationAROfTouch", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajNodePrototypeFunctionLocationAROfTouch), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJNodePrototypeTable = 
	{68, 63, AJNodePrototypeTableValues, 0};

	static const HashTableValue AJNodeConstructurTableValues[2] =
	{
		{"INVALID_TAG", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajNodeConstructorINVALID_TAG), (intptr_t)0 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJNodeConstructorTable = 
	{2, 1, AJNodeConstructurTableValues, 0};

	const ClassInfo AJNode::s_info = {"Node", 0, &AJNodeTable, 0};
	
	bool AJNode::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJNode, Base>(exec, &AJNodeTable, this, propertyName, slot);
	}
	
	bool AJNode::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJNode, Base>(exec, &AJNodeTable, this, propertyName, slot);
	}

	void AJNode::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNode, Base>(exec, propertyName, value, &AJNodeTable, this, slot);
	}

	AJObject* AJNode::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJNodePrototype(AJNodePrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJNodePrototype::AJNodePrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJNodePrototype::s_info = {"NodePrototype", 0, &AJNodePrototypeTable, 0};
	
	bool AJNodePrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticFunctionSlot<AJ::AJObject>(exec, &AJNodePrototypeTable, this, propertyName, slot);
	}
	
	bool AJNodePrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticFunctionDescriptor<AJ::AJObject>(exec, &AJNodePrototypeTable, this, propertyName, descriptor);
	}

	void AJNodePrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNodePrototype, Base>(exec, propertyName, value, &AJNodePrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJNodeConstructor::AJNodeConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJNodeConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJNodePrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJNodeConstructor::s_info = {"NodeConstructor", 0, &AJNodeConstructorTable, 0};
	
	bool AJNodeConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticValueSlot<AJNodeConstructor, Base>(exec, &AJNodeConstructorTable, this, propertyName, slot);
	}
	
	bool AJNodeConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticValueDescriptor<AJNodeConstructor, Base>(exec, &AJNodeConstructorTable, this, propertyName, descriptor);
	}

	void AJNodeConstructor::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJNodeConstructor, Base>(exec, propertyName, value, &AJNodeConstructorTable, this, slot);
	}


#undef THUNK_GENERATOR
