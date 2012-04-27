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

#ifndef OpenAphid_NamespaceActions_h
#define OpenAphid_NamespaceActions_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

namespace Aphid {
	class AJNamespaceActions;
	
	class NamespaceActions : public RefCounted<NamespaceActions>, public DirectWrapper<AJNamespaceActions> {
		typedef NamespaceActions Self;
	public:
		virtual ~NamespaceActions();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
	protected:
		NamespaceActions();
	};
}

#endif
