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
#include "NamespaceCore.h"
#include "AJNamespaceCore.h"
#include "OAUtil.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	
	NamespaceCore::NamespaceCore()
	{
		LEAK_DETECT_INC("NamespaceCore");
	}
	
	NamespaceCore::~NamespaceCore()
	{
		LEAK_DETECT_DEC("NamespaceCore");
	}
	
	KURL NamespaceCore::makeURL(const AJ::UString &relative)
	{
		return KURL(m_baseURL, toString(relative));
	}
} //namespace Aphid
