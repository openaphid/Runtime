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

#ifndef OpenAphid_ObjCObject_h
#define OpenAphid_ObjCObject_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

#if !PLATFORM(IPHONE)
#error ObjCObject is only designed for iPhone platform
#endif

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace Aphid {
	class ObjCObject : public RefCounted<ObjCObject> {
	public:
		~ObjCObject();
		
		static PassRefPtr<ObjCObject> create()
		{
			return adoptRef(new ObjCObject());
		}
		
#ifdef __OBJC__
		static PassRefPtr<ObjCObject> create(id impl)
		{
			return adoptRef(new ObjCObject(impl));
		}
		
		id impl() const {return m_impl;}
		
	private:
		ObjCObject(id impl=nil);
		id m_impl;
#else
		static PassRefPtr<ObjCObject> create(void* impl)
		{
			return adoptRef(new ObjCObject(impl));
		}
		
		void* impl() const {return m_impl;}
		
	private:
		ObjCObject(void* impl=0);
		void* m_impl;
#endif		
		
	};
}

#endif
