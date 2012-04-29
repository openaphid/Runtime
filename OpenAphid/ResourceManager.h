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

#ifndef OpenAphid_ResourceManager_h
#define OpenAphid_ResourceManager_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/FastAllocBase.h>
#include <wtf/Noncopyable.h>
#include <WTFString.h>

#ifdef __OBJC__
@class UIImage;
#endif

namespace Aphid {
#ifndef __OBJC__
	class NSString;
	class NSBundle;
	class NSURL;
	class UIImage;
	class NSData;
#endif
	
	class ResourceManager : public Noncopyable {
	public:
		virtual ~ResourceManager();
		
		static ResourceManager* sharedManager();
		
		NSURL* baseURL() const {return m_baseURL;}
		void setBaseURL(NSURL* url);
		
		void setBundleName(NSString *name);
		NSString* bundleName() const {return m_bundleName;}
		
		String loadStringInBundle(const char* name);
		
		UIImage* loadUIImageInBundle(const char* name);
		UIImage* loadUIImageInBundle(NSString *name);
		NSData* loadNSDataInBundle(NSString* name, bool supressLogging=false);
		NSString* loadNSStringInBundle(NSString* name);
		
	protected:
		ResourceManager();
		
		static ResourceManager* s_instance;
		
		NSString* m_bundleName;
		NSBundle* m_bundle;
		NSURL* m_baseURL;
	};
}

#endif
