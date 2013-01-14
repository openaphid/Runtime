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

#ifndef OpenAphid_NamespaceCore_h
#define OpenAphid_NamespaceCore_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include "DirectWrapper.h"
#include "KURL.h"
#include "NamespaceJS.h"

namespace Aphid {
	class AJNamespaceCore;
	
	///-------------------------------------------------------------------------------------------------------------------
	class NamespaceCore : public RefCounted<NamespaceCore>, public DirectWrapper<AJNamespaceCore> {
	public:
		virtual ~NamespaceCore();
		
		static PassRefPtr<NamespaceCore> create()
		{
			return adoptRef(new NamespaceCore());
		}
		
		const KURL& baseURL() const {return m_baseURL;} 
		void setBaseURL(const KURL& url) {m_baseURL = url;}
		
		KURL makeURL(const AJ::UString& relative);
		
	private:
		NamespaceCore();
		
		KURL m_baseURL;
		
	}; // class NamespaceCore
	
}// namespace Aphid

#endif
