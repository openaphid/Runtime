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

#ifndef OpenAphid_Scene_h
#define OpenAphid_Scene_h

#include "Node.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

namespace Aphid {	
	///-------------------------------------------------------------------------------------------------------------------	
	class Scene : public Node {
	public:
		virtual ~Scene();
		
		static PassRefPtr<Scene> create()
		{
			return adoptRef(new Scene());
		}
		
		virtual bool isTransition() const {return false;}
		
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
	private:
		Scene();
	}; //class Scene
	
} //namespace Aphid

#endif
