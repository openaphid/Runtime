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

#ifndef OpenAphid_NamespaceG2D_h
#define OpenAphid_NamespaceG2D_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include "DirectWrapper.h"

#include "Scheduler.h"
#include "Director.h"
#include "ActionManager.h"
#include "NamespaceActions.h"

#include "Font.h"

namespace Aphid {
	class AJNamespaceG2D;
	
	///-------------------------------------------------------------------------------------------------------------------
	class NamespaceG2D : public RefCounted<NamespaceG2D>, public DirectWrapper<AJNamespaceG2D> {
	public:
		virtual ~NamespaceG2D();
		
		static PassRefPtr<NamespaceG2D> create()
		{
			return adoptRef(new NamespaceG2D());
		}
		
		static float s_content_scale_factor;
		
		void initialize();
		
		Scheduler* scheduler() const {return m_scheduler.get();}
		
		Director* director() const {return m_director.get();}
		
		ActionManager* actionManager() const {return m_actionManager.get();}
		
		Font* defaultFont() const {return m_defaultFont.get();}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	private:
		NamespaceG2D();
		
		RefPtr<Scheduler> m_scheduler;
		RefPtr<Director> m_director;
		RefPtr<ActionManager> m_actionManager;
		RefPtr<Font> m_defaultFont;
	}; // class NamespaceG2D
	
}// namespace Aphid

#endif
