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
#include <wtf/text/AtomicString.h>
#include <wtf/ByteArray.h>
#include <wtf/PassRefPtr.h>

#if PLATFORM(IPHONE)
#ifdef __OBJC__
@class UIImage;
#endif
#endif

#if PLATFORM(ANDROID)
#include "OAJNIUtil.h"
#endif

namespace Aphid {
	
#if PLATFORM(IPHONE)
#ifndef __OBJC__
	class NSString;
	class NSBundle;
	class NSURL;
	class UIImage;
	class NSData;
#endif
#endif //PLATFORM(IPHONE)
	
	class ResourceManager : public Noncopyable {
	public:
		virtual ~ResourceManager();
		
		static ResourceManager* sharedManager();
		
#if PLATFORM(IPHONE)
		NSURL* baseURL() const {return m_baseURL;}
		void setBaseURL(NSURL* url);
		
		//TODO: .mm file should be fixed to use new methods
		//void setBundleName(NSString *name);
		//NSString* bundleName() const {return m_bundleName;}
		
		UIImage* loadUIImageInBundle(const char* name);
		UIImage* loadUIImageInBundle(NSString *name);
		NSData* loadNSDataInBundle(NSString* name, bool supressLogging=false);
		NSString* loadNSStringInBundle(NSString* name);
#endif
		
		//void setBundleName(const String& name);
		//String bundleName() const {return m_bundleName;}
		
		PassRefPtr<ATF::ByteArray> loadDataInBundle(const char* name, bool supressLogging=false);		
		String loadStringInBundle(const char* name);
		
#if PLATFORM(ANDROID)
		PassOwnPtr<JNI::LocalObject> loadBitmapInBundle(const char* name);
#endif
		
	protected:
		ResourceManager();
		
		static ResourceManager* s_instance;
		
		AtomicString m_bundleName;

#if PLATFORM(IPHONE)
		//NSString* m_bundleName;
		NSBundle* m_bundle;
		NSURL* m_baseURL;
#endif
	};
}

#endif
