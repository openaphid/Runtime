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

#ifndef OpenAphid_iOSUtils_h
#define OpenAphid_iOSUtils_h

#ifndef __OBJC__
#error iOSUtil.h should not be included in a cpp file
#endif

#define OA_OBJC_RELEASE(p) { if (p) [(p) release]; p = nil; }

@class UIFont;

namespace AJ {
	class UString;
}

namespace Aphid {
	class String;
}

namespace Aphid {
	class Font;
	class KURL;
	
	Aphid::String toWebCoreString(NSString* string);
	AJ::UString toUString(NSString* string);
	
	NSString* toNSString(const AJ::UString& ustring);
	
	NSString* toNSString(const Aphid::String& string);
	
	UIFont* toUIFont(const Font& font);
	
	NSURL* toNSURL(const KURL& url);
	
	KURL toKURL(NSURL* url);
}

#endif
