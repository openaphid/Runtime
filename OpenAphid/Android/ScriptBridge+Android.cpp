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
#include <runtime/InitializeThreading.h>
#include <runtime/AJGlobalData.h>
#include <runtime/AJLock.h>
#include <API/APIShims.h>
#include <parser/SourceCode.h>

#include "Diagnostic.h"
#include "ScriptBridge+Android.h"

#include "OAUtil.h"
#include "Diagnostic.h"

#include "OAGlobalObject.h"
#include "AJOAGlobalObject.h"
#include "AJOABinding.h"
#include "NamespaceJS.h"

#include "KURL.h"
#include "ResourceManager.h"

#include "OAJNIUtil.h"

using namespace AJ;

namespace Aphid {
	ScriptBridge::ScriptBridge()
	{

	}

	ScriptBridge::~ScriptBridge()
	{
		destroy();
	}

	void ScriptBridge::initialize()
	{
#if PLATFORM(ANDROID)
		JNIEnv* env = JNI::getJNIEnv();
		RELEASE_ASSERT(env);
		
		JNI::Cache::initialize(env);
#endif

		AJ::initializeThreading();

		//TODO: setup default pixel format of glview

		AJLock lock(LockForReal);

		OAGlobalObject::createInstance();

		RefPtr<AJGlobalData> globalData = AJGlobalData::create(AJ::ThreadStackTypeSmall);

		APIEntryShim shim(globalData.get(), false);

		RefPtr<Structure> structure = AJOAGlobalObject::createStructure(AJ::jsNull());
		_globalObject = new (globalData.get()) AJOAGlobalObject(structure, OAGlobalObject::sharedInstance());
		gcProtect(_globalObject->globalExec()->dynamicGlobalObject());
		globalData->ref();

		OAGlobalObject::sharedInstance()->initialize();

		oa_debug("script bridge is initialized");
	}

	void ScriptBridge::destroy()
	{
		if (_globalObject) {
			ExecState* exec = _globalObject->globalExec();
			AJLock lock(exec);

			AJGlobalData& globalData = exec->globalData();
			AJGlobalObject* dgo = exec->dynamicGlobalObject();
			IdentifierTable* savedIdentifierTable = wtfThreadData().setCurrentIdentifierTable(globalData.identifierTable);

			// One reference is held by AJGlobalObject, another added by AJGlobalContextRetain().
			bool releasingContextGroup = globalData.refCount() == 2;
			bool releasingGlobalObject = Heap::heap(dgo)->unprotect(dgo);
			// If this is the last reference to a global data, it should also
			// be the only remaining reference to the global object too!
			ASSERT(!releasingContextGroup || releasingGlobalObject);

			// An API 'AJGlobalContextRef' retains two things - a global object and a
			// global data (or context group, in API terminology).
			// * If this is the last reference to any contexts in the given context group,
			//   call destroy on the heap (the global data is being  freed).
			// * If this was the last reference to the global object, then unprotecting
			//   it may  release a lot of GC memory - run the garbage collector now.
			// * If there are more references remaining the the global object, then do nothing
			//   (specifically that is more protects, which we assume come from other AJGlobalContextRefs).
			if (releasingContextGroup)
				globalData.heap.destroy();
			else if (releasingGlobalObject)
				globalData.heap.collectAllGarbage();

			globalData.deref();

			wtfThreadData().setCurrentIdentifierTable(savedIdentifierTable);

			_globalObject = NULL;

			OAGlobalObject::destroyInstance();

			ALIVE_REPORT();

			oa_debug("script bridge is destroyed");
		}
	}

	void ScriptBridge::enableDeveloperMode(bool mode)
	{
		OAGlobalObject::s_develop_mode = mode;
	}

	void ScriptBridge::setBaseUrlString(const String& urlString)
	{
		if (urlString.isEmpty())
			OAGlobalObject::sharedInstance()->namespaceCore()->setBaseURL(KURL());
		else
			OAGlobalObject::sharedInstance()->namespaceCore()->setBaseURL(KURL(KURL(), urlString));
	}

	bool ScriptBridge::evaluateScriptFile(const String& filename)
	{
		if (OAGlobalObject::s_develop_mode)
			Diagnostic::info(String::format("Loading '%s' in developer mode", filename.utf8().data()));

		String content = ResourceManager::sharedManager()->loadStringInBundle(filename.utf8().data());
		return evaluateScriptString(content, filename, 1);
	}

	bool ScriptBridge::evaluateScriptString(const String& script, const String& source, int repeat)
	{
		if (!script.isEmpty()) {
			if (OAGlobalObject::s_develop_mode)
				Diagnostic::info(String::format("Evaluating '%s' in developer mode", source.utf8().data()));
			ExecState* exec = _globalObject->globalExec();
			for (int i = 0; i < repeat; i++) {
				Completion completion = NamespaceJS::includeScript(exec, exec->dynamicGlobalObject(), toUString(source),
						toUString(script));

				if (completion.complType() == Throw) {
					reportAJException(exec, completion.value());
					return false;
				}
			}

			OAGlobalObject::sharedInstance()->namespaceJS()->fireOnload();
			return true;
		} else {
			Diagnostic::error(String::format("Script file '%s' is empty?!", source.utf8().data()));
			return false;
		}

		return false;
	}
}
