	const AJOABindingType AJXMLHttpRequest::s_type = AJXMLHttpRequestBindingType;

ASSERT_CLASS_FITS_IN_CELL(AJXMLHttpRequest);
ASSERT_CLASS_FITS_IN_CELL(AJXMLHttpRequestPrototype);
ASSERT_CLASS_FITS_IN_CELL(AJXMLHttpRequestConstructor);

#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

	static const HashTableValue AJXMLHttpRequestTableValues[15] = 
	{
		{"readyState", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestReadyState), (intptr_t)0 THUNK_GENERATOR(0)},
		{"status", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestStatus), (intptr_t)0 THUNK_GENERATOR(0)},
		{"statusText", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestStatusText), (intptr_t)0 THUNK_GENERATOR(0)},
		{"responseText", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestResponseText), (intptr_t)0 THUNK_GENERATOR(0)},
		{"responseXML", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestResponseXML), (intptr_t)0 THUNK_GENERATOR(0)},
		{"withCredentials", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestWithCredentials), (intptr_t)setAJXMLHttpRequestWithCredentials THUNK_GENERATOR(0)},
		{"onloadstart", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnloadstart), (intptr_t)setAJXMLHttpRequestOnloadstart THUNK_GENERATOR(0)},
		{"onprogress", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnprogress), (intptr_t)setAJXMLHttpRequestOnprogress THUNK_GENERATOR(0)},
		{"onabort", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnabort), (intptr_t)setAJXMLHttpRequestOnabort THUNK_GENERATOR(0)},
		{"onerror", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnerror), (intptr_t)setAJXMLHttpRequestOnerror THUNK_GENERATOR(0)},
		{"onload", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnload), (intptr_t)setAJXMLHttpRequestOnload THUNK_GENERATOR(0)},
		{"ontimeout", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOntimeout), (intptr_t)setAJXMLHttpRequestOntimeout THUNK_GENERATOR(0)},
		{"onloadend", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnloadend), (intptr_t)setAJXMLHttpRequestOnloadend THUNK_GENERATOR(0)},
		{"onreadystatechange", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestOnreadystatechange), (intptr_t)setAJXMLHttpRequestOnreadystatechange THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};	

	static JSC_CONST_HASHTABLE HashTable AJXMLHttpRequestTable = 
	{36, 31, AJXMLHttpRequestTableValues, 0};

	static const HashTableValue AJXMLHttpRequestPrototypeTableValues[12] = 
	{
		{"UNSENT", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestPrototypeUNSENT), (intptr_t)0 THUNK_GENERATOR(0)},
		{"OPENED", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestPrototypeOPENED), (intptr_t)0 THUNK_GENERATOR(0)},
		{"HEADERS_RECEIVED", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestPrototypeHEADERS_RECEIVED), (intptr_t)0 THUNK_GENERATOR(0)},
		{"LOADING", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestPrototypeLOADING), (intptr_t)0 THUNK_GENERATOR(0)},
		{"DONE", ReadOnly|DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(ajXMLHttpRequestPrototypeDONE), (intptr_t)0 THUNK_GENERATOR(0)},
		{"open", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajXMLHttpRequestPrototypeFunctionOpen), (intptr_t)2 THUNK_GENERATOR(0)},
		{"setRequestHeader", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajXMLHttpRequestPrototypeFunctionSetRequestHeader), (intptr_t)2 THUNK_GENERATOR(0)},
		{"send", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajXMLHttpRequestPrototypeFunctionSend), (intptr_t)0 THUNK_GENERATOR(0)},
		{"abort", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajXMLHttpRequestPrototypeFunctionAbort), (intptr_t)0 THUNK_GENERATOR(0)},
		{"getAllResponseHeaders", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajXMLHttpRequestPrototypeFunctionGetAllResponseHeaders), (intptr_t)0 THUNK_GENERATOR(0)},
		{"getResponseHeader", Function|DontDelete, (intptr_t)static_cast<NativeFunction>(ajXMLHttpRequestPrototypeFunctionGetResponseHeader), (intptr_t)1 THUNK_GENERATOR(0)},
		{0, 0, 0, 0 THUNK_GENERATOR(0)}
	};
	
	static JSC_CONST_HASHTABLE HashTable AJXMLHttpRequestPrototypeTable = 
	{34, 31, AJXMLHttpRequestPrototypeTableValues, 0};


	const ClassInfo AJXMLHttpRequest::s_info = {"XMLHttpRequest", 0, &AJXMLHttpRequestTable, 0};
	
	bool AJXMLHttpRequest::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) 
	{
		return getStaticValueSlot<AJXMLHttpRequest, Base>(exec, &AJXMLHttpRequestTable, this, propertyName, slot);
	}
	
	bool AJXMLHttpRequest::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& slot)
	{
		return getStaticValueDescriptor<AJXMLHttpRequest, Base>(exec, &AJXMLHttpRequestTable, this, propertyName, slot);
	}

	void AJXMLHttpRequest::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJXMLHttpRequest, Base>(exec, propertyName, value, &AJXMLHttpRequestTable, this, slot);
	}

	AJObject* AJXMLHttpRequest::createPrototype(ExecState* exec, AJGlobalObject* globalObject)
	{
		return new(exec)AJXMLHttpRequestPrototype(AJXMLHttpRequestPrototype::createStructure(globalObject->objectPrototype()));
	}

	///-------------------------------------------------------------------------------------------------------------------	
	AJXMLHttpRequestPrototype::AJXMLHttpRequestPrototype(NonNullPassRefPtr<Structure> structure)
	: Base(structure)
	{
	}
	
	const ClassInfo AJXMLHttpRequestPrototype::s_info = {"XMLHttpRequestPrototype", 0, &AJXMLHttpRequestPrototypeTable, 0};
	
	bool AJXMLHttpRequestPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
	{
		return getStaticPropertySlot<AJXMLHttpRequestPrototype, Base>(exec, &AJXMLHttpRequestPrototypeTable, this, propertyName, slot);
	}
	
	bool AJXMLHttpRequestPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
	{
		return getStaticPropertyDescriptor<AJXMLHttpRequestPrototype, Base>(exec, &AJXMLHttpRequestPrototypeTable, this, propertyName, descriptor);
	}

	void AJXMLHttpRequestPrototype::put(ExecState* exec, const Identifier& propertyName, AJValue value, PutPropertySlot& slot)
	{
		oalookupPut<AJXMLHttpRequestPrototype, Base>(exec, propertyName, value, &AJXMLHttpRequestPrototypeTable, this, slot);
	}

	///-------------------------------------------------------------------------------------------------------------------
	AJXMLHttpRequestConstructor::AJXMLHttpRequestConstructor(ExecState* exec, AJOAGlobalObject* globalObject)
	: Base(AJXMLHttpRequestConstructor::createStructure(globalObject->objectPrototype()), globalObject)
	{
		putDirect(exec->propertyNames().prototype, AJXMLHttpRequestPrototype::self(exec, globalObject), None); //TODO: seems important, find out why?
	}	
	
	const ClassInfo AJXMLHttpRequestConstructor::s_info = {"XMLHttpRequestConstructor", 0, 0, 0};


#undef THUNK_GENERATOR
