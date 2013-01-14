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

#ifndef OpenAphid_NamespaceJS_h
#define OpenAphid_NamespaceJS_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <runtime/Completion.h>
#include <runtime/AJGlobalObject.h>

#include "DirectWrapper.h"
#include "AJCallback.h"

namespace Aphid {
	class AJNamespaceJS;
	
	class NamespaceJS : public RefCounted<NamespaceJS>, public DirectWrapper<AJNamespaceJS> {
		typedef NamespaceJS Self;
	public:
		virtual ~NamespaceJS();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		bool gc();
		
		void setOnloadCallback(AJ::ExecState* exec, AJ::AJValue callback);
		AJ::AJObject* onloadCallback() const;
		
		void fireOnload();
		
		static AJ::Completion includeFile(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject, const AJ::UString& filename);
		static AJ::Completion includeScript(AJ::ExecState* exec, AJ::AJGlobalObject* globalObject, const AJ::UString& filename, const AJ::UString& script);
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
	protected:
		NamespaceJS();
		
		RefPtr<MappedAJObject> m_onload;
	};
}

#endif
