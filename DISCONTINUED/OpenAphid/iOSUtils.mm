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
#include "iOSUtils.h"
#include <runtime/UString.h>
#include <WTFString.h>
#include "Font.h"
#include "KURL.h"
#include "AJOABinding.h"

#import <UIKit/UIKit.h>

namespace Aphid {
	Aphid::String toWebCoreString(NSString* string)
	{
		if (string)
			return Aphid::String::fromUTF8([string cStringUsingEncoding:NSUTF8StringEncoding]);
		return Aphid::String();
	}
	
	AJ::UString toUString(NSString* string)
	{
		return toUString(toWebCoreString(string));
	}
	
  NSString* toNSString(const AJ::UString& ustring)
	{
		if (ustring.isNull())
			return nil;
		return [NSString stringWithCString:ustring.UTF8String().data() encoding:NSUTF8StringEncoding];
	}
	
	NSString* toNSString(const Aphid::String& string)
	{
		if (string.isNull())
			return nil;
		return [NSString stringWithCString:string.utf8().data() encoding:NSUTF8StringEncoding];
	}
	
	UIFont* toUIFont(const Font& font)
	{
		return [UIFont fontWithName:toNSString(font.fontName()) size:font.fontSizeInPixels()]; //TODO check later
	}
	
	KURL toKURL(NSURL* url)
	{
		if (url)
			return KURL(KURL(), toWebCoreString([url absoluteString]));
		return KURL();
	}
	
	NSURL* toNSURL(const KURL& url)
	{
		return [NSURL URLWithString:toNSString(url.string())]; //TODO: rough impl, check it later
	}
}
