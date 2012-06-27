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
#include <runtime/AJLock.h>

#include "OAGlobalObject.h"

#include "OAUtil.h"

#include "Console.h"

#include "AJOAGlobalObject.h"
#include "AJNamespaceAphid.h"
#include "AJNamespaceCore.h"
#include "AJNamespaceG2D.h"
#include "AJNamespaceJS.h"
#include "AJNamespaceActions.h"
#include "AJConsole.h"
#include "AJNamespaceiOS.h"
#include "AJNamespaceExt.h"

#include "AJOABinding.h"

namespace Aphid {
  using namespace AJ;

  bool OAGlobalObject::s_develop_mode = false;

  ///-------------------------------------------------------------------------------------------------------------------
  OAGlobalObject::OAGlobalObject()
  {
	  LEAK_DETECT_INC("OAGlobalObject");
	  /*
	  m_heapStats.lastTime = mach_absolute_time();
	  m_heapStats.stats.size = 0;
	  m_heapStats.stats.free = 0;
	  */
  }

  OAGlobalObject::~OAGlobalObject()
  {
	  LEAK_DETECT_DEC("OAGlobalObject");

	  oa_debug("destroyed OAGlobalObject");
  }

  RefPtr<OAGlobalObject> OAGlobalObject::s_shared_instance = 0;
  bool OAGlobalObject::s_destroyed = false;

  void OAGlobalObject::createInstance()
  {
  	ASSERT(!s_shared_instance);
  	
  	s_destroyed = false;
  	s_shared_instance = adoptRef(new OAGlobalObject());
  }
	
  OAGlobalObject *OAGlobalObject::sharedInstance()
  {
	  if (!s_destroyed) {
	  if (!s_shared_instance)
				createInstance();

	  return s_shared_instance.get();
		} else
			return 0;
  }

  void OAGlobalObject::destroyInstance()
  {
		RefPtr<Director> lockDirector = Director::sharedDirector(); //TODO: this is a temporary solution to hold director a bit longer, should impl a better exit process
	  s_shared_instance = 0;
	  s_destroyed = true;
  }

  void OAGlobalObject::initialize()
  {
	  m_namespaceAphid = NamespaceAphid::create();
	  m_namespaceG2D = NamespaceG2D::create();

	  m_namespaceG2D->initialize();
  }

  Console *OAGlobalObject::console() const
  {
	  if (!m_console)
		  m_console = Console::create();
	  return m_console.get();
  }

  NamespaceCore *OAGlobalObject::namespaceCore() const
  {
	  if (!m_namespaceCore)
		  m_namespaceCore = NamespaceCore::create();
	  return m_namespaceCore.get();
  }

  NamespaceJS *OAGlobalObject::namespaceJS() const
  {
	  if (!m_namespaceJS)
		  m_namespaceJS = NamespaceJS::create();
	  return m_namespaceJS.get();
  }

  NamespaceActions *OAGlobalObject::namespaceActions() const
  {
	  if (!m_namespaceActions)
		  m_namespaceActions = NamespaceActions::create();
	  return m_namespaceActions.get();
  }

  NamespaceiOS *OAGlobalObject::namespaceiOS() const
  {
	  if (!m_namespaceiOS)
		  m_namespaceiOS = NamespaceiOS::create();
	  return m_namespaceiOS.get();
  }

	NamespaceExt* OAGlobalObject::namespaceExt() const
	{
		if (!m_namespaceExt)
			m_namespaceExt = NamespaceExt::create();
		return m_namespaceExt.get();
	}
	
	NamespaceExt* OAGlobalObject::namespaceExtIOS() const
	{
		if (!m_namespaceExtIOS)
			m_namespaceExtIOS = NamespaceExt::create();
		return m_namespaceExtIOS.get();
	}
	
  bool OAGlobalObject::s_explicit_gc = false;

  bool OAGlobalObject::garbageCollection()
  {
	  AJOAGlobalObject *globalObject = wrapper();
	  if (globalObject) {
		  ExecState *exec = globalObject->globalExec();
		  s_explicit_gc = true;

		  AJLock lock(exec);
		  AJGlobalData& globalData = exec->globalData();
		  if (!globalData.heap.isBusy()) {
			  globalData.heap.collectAllGarbage();
			  s_explicit_gc = false;
			  return true;
		  }
	  }
	  return false;
  }

  void OAGlobalObject::markObjects(AJ::MarkStack& markStack, unsigned markID)
  {
	  if (isMarked(markID))
		  return;

	  DirectWrapper<AJOAGlobalObject>::markObjects(markStack, markID);

	  markDirectWrapper(namespaceAphid(), markStack, markID);
	  markDirectWrapper(namespaceG2D(), markStack, markID);
	  markDirectWrapper(optNamespaceCore(), markStack, markID);
	  markDirectWrapper(optNamespaceJS(), markStack, markID);
	  markDirectWrapper(optNamespaceActions(), markStack, markID);
		markDirectWrapper(optNamespaceExt(), markStack, markID);
		markDirectWrapper(optNamespaceExtIOS(), markStack, markID);

	  markDirectWrapper(optConsole(), markStack, markID);

	  AJObjectManager::sharedManager()->markObjects(markStack, markID);

	  TouchDispatcher::sharedDispatcher()->markTouchObjects(markStack, markID);
  }
} //namespace Aphid
