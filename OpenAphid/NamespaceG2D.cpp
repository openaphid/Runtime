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
#include "AJDirector.h"
#include "AJNamespaceActions.h"
#include "NamespaceG2D.h"
#include "AJNamespaceG2D.h"
#include "OAUtil.h"

namespace Aphid {
	using namespace AJ;
	
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceG2D::NamespaceG2D()
	{
		LEAK_DETECT_INC("NamespaceG2D");
	}
	
	NamespaceG2D::~NamespaceG2D()
	{
		LEAK_DETECT_DEC("NamespaceG2D");
	}
	
	float NamespaceG2D::s_content_scale_factor = 1;
	
	void NamespaceG2D::initialize()
	{
		ASSERT(!m_scheduler);
		
		m_director = DirectorIOS::create();
		m_scheduler = Scheduler::create();
		m_actionManager = ActionManager::create();
		m_defaultFont = Font::create(18);
	}
	
	void NamespaceG2D::markObjects(AJ::MarkStack &markStack, unsigned markID)
	{
		if (isMarked(markID))
			return;
		
		DirectWrapper<AJNamespaceG2D>::markObjects(markStack, markID);
		markDirectWrapper(m_director.get(), markStack, markID);
		m_actionManager->markObjects(markStack, markID);
		m_scheduler->markObjects(markStack, markID);
	}
} //namespace Aphid
