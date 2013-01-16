/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

 */

#include <config.h>
#include <runtime/Completion.h>
#include <runtime/AJFunction.h>
#include <interpreter/CallFrame.h>
#include <parser/SourceCode.h>
#include <API/APIShims.h>

#include "NamespaceJS.h"
#include "OAUtil.h"
#include "OAGlobalObject.h"
#include "AJNamespaceJS.h"
#include "ResourceManager.h"

namespace Aphid {
	using namespace AJ;
	
	NamespaceJS::NamespaceJS()
	{
		LEAK_DETECT_INC("NamespaceJS");
	}
	
	NamespaceJS::~NamespaceJS()
	{
		LEAK_DETECT_DEC("NamespaceJS");
	}
	
	void NamespaceJS::markObjects(AJ::MarkStack& markStack, unsigned markID)
	{
		if (isMarked(markID))
			return;
		
		DirectWrapper<AJNamespaceJS>::markObjects(markStack, markID);
		if (m_onload)
			m_onload->markObject(markStack, markID);
	}
	
	void NamespaceJS::fireOnload()
	{
		if (m_onload) {
			AJOAGlobalObject* globalObject = OAGlobalObject::sharedInstance()->wrapper();
			ASSERT(globalObject);
			
			ExecState* exec = globalObject->globalExec();
			
			AJLock lock(exec);
			
			AJObject* callback = m_onload->optObject();
			ASSERT(callback);
			CallData callData;
			CallType callType = callback->getCallData(callData);
			if (callType != CallTypeNone) {				
				MarkedArgumentBuffer args;
				
				AJ::call(exec, callback, callType, callData, globalObject, args);
				
				if (exec->hadException())
					reportAJException(exec, exec->exception());
			} else 
				oa_error("Invalid call type for JS.onload: %d", callType);
		}
	}
	
	bool NamespaceJS::gc()
	{
		return OAGlobalObject::sharedInstance()->garbageCollection();
	}
	
	void NamespaceJS::setOnloadCallback(AJ::ExecState* exec, AJ::AJValue callback)
	{
		UNUSED_PARAM(exec);
		
		if (callback.isObject())
			m_onload = AJObjectManager::sharedManager()->putMapped(asObject(callback));
		else {
			m_onload = 0;
		}
	}	
	
	AJ::AJObject* NamespaceJS::onloadCallback() const
	{
		return m_onload ? m_onload->optObject() : 0;
	}
	
	AJ::Completion NamespaceJS::includeFile(AJ::ExecState* exec, AJGlobalObject* globalObject, const AJ::UString& filename)
	{
		String script = ResourceManager::sharedManager()->loadStringInBundle(filename.UTF8String().data());
		
		if (script.isEmpty()) {
			oa_warn("Failed to include file: %s", filename.UTF8String().data());
			return Completion(Normal, jsUndefined());
		}
		
		return includeScript(exec, globalObject, filename, toUString(script));
		
		
	}
	
	AJ::Completion NamespaceJS::includeScript(AJ::ExecState* exec, AJGlobalObject* globalObject, const AJ::UString& filename, const AJ::UString& script)
	{
		SourceCode source = makeSource(script, filename);
		
		APIEntryShim shim(exec);
		
		/*
		CallFrame* frame = exec->callerFrame();
		AJFunction* function = frame->callee();
		ScopeChain& chain = function->scope();
		AJValue thisValue = frame->thisValue();
		AJObject* thisObject = frame->thisValue().toThisObject(frame);
		oa_info("exec.node: %p", exec->scopeChain());
		oa_info("chain.node: %p", chain.node());
		oa_info("global chain.node: %p", globalObject->globalScopeChain().node());
		oa_info("function: %p", function);
		oa_info("globalObject: %p", globalObject);
		Completion completion = evaluate(exec, chain, source, thisObject);
		 */
		return evaluate(exec, globalObject->globalScopeChain(), source);
	}
}