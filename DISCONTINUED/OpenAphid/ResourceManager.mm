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
#include "ResourceManager.h"
#include "iOSUtils.h"
#include "Diagnostic.h"
#include "OAGlobalObject.h"

#import <UIKit/UIKit.h>

namespace Aphid {
	ResourceManager::ResourceManager()
	{
		m_bundle = [[NSBundle alloc] initWithPath:[[NSBundle mainBundle] resourcePath]];
		m_baseURL = nil;
		m_bundleName = nil;
	}
	
	ResourceManager::~ResourceManager()
	{
		OA_OBJC_RELEASE(m_bundle);
		OA_OBJC_RELEASE(m_baseURL);
		OA_OBJC_RELEASE(m_bundleName);
	}
	
	ResourceManager* ResourceManager::s_instance = 0;
	ResourceManager* ResourceManager::sharedManager()
	{
		if (!s_instance)
			s_instance = new ResourceManager();
		return s_instance;
	}
	
	void ResourceManager::setBaseURL(NSURL *url)
	{
		OA_OBJC_RELEASE(m_baseURL);
		m_baseURL = [url copy];
	}
	
	void ResourceManager::setBundleName(NSString *name)
	{
		OA_OBJC_RELEASE(m_bundle);
		OA_OBJC_RELEASE(m_bundleName);
		if (name) {
			m_bundleName = [name copy];
			
			NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:name];
			BOOL isDirectory = NO;
			if (![[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory] || !isDirectory) {
				Diagnostic::error([NSString stringWithFormat:@"Bundle (%@) doesn't exist at: %@", name, path]);
				return;
			}
			m_bundle = [[NSBundle alloc] initWithPath:path];
		} else
			m_bundle = [[NSBundle alloc] initWithPath:[[NSBundle mainBundle] resourcePath]];
	}
	
	NSData* ResourceManager::loadNSDataInBundle(NSString *name, bool supressLogging)
	{
		NSData* data = nil;
		
		if (OAGlobalObject::s_develop_mode) {
			NSURL* url = [NSURL URLWithString:name relativeToURL:m_baseURL];
			oa_info("(develop mode) loading data '%s' from remote: %s", [name cStringUsingEncoding:NSUTF8StringEncoding], [[url absoluteString] cStringUsingEncoding:NSUTF8StringEncoding]);
			
			if (url) {
				NSError* error = nil;
				data = [NSData dataWithContentsOfURL:url options:NSDataReadingUncached error:&error];
				if (error)
					Diagnostic::warn([NSString stringWithFormat:@"Error occurred when loading from %@: %@", [url absoluteString], error]);
			} else {				
				Diagnostic::error([NSString stringWithFormat:@"Invalid format of url: %@", name]);
			}
		} else
			data = [NSData dataWithContentsOfFile:[[m_bundle bundlePath] stringByAppendingPathComponent:name]];
		
		if (!data && !supressLogging)
			Diagnostic::warn([NSString stringWithFormat:@"Failed to load data for '%@'", name]);
		
		return data;
	}
	
	UIImage* ResourceManager::loadUIImageInBundle(NSString *name)
	{
		UIImage* image = nil;
		if (OAGlobalObject::s_develop_mode) {
			image = [UIImage imageWithData:loadNSDataInBundle(name, true)];			
		} else
			image = [UIImage imageWithContentsOfFile:[[m_bundle bundlePath] stringByAppendingPathComponent:name]];
		
		if (!image)
			Diagnostic::warn([NSString stringWithFormat:@"Failed to load image for '%@'", name]);
			
		return image;
	}
	
	NSString* ResourceManager::loadNSStringInBundle(NSString *name)
	{
		NSString* string = nil;
		if (OAGlobalObject::s_develop_mode) {
			NSData* data = loadNSDataInBundle(name, true);
			if (data)
				string = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
		}
		else
			string = [NSString stringWithContentsOfFile:[[m_bundle bundlePath] stringByAppendingPathComponent:name] encoding:NSUTF8StringEncoding error:NULL];
		
		if (!string)
			Diagnostic::warn([NSString stringWithFormat:@"Failed to load string for '%@'", name]);
		
		return string;
	}
	
	UIImage* ResourceManager::loadUIImageInBundle(const char *name)
	{
		return loadUIImageInBundle([NSString stringWithUTF8String:name]);
	}
	
	String ResourceManager::loadStringInBundle(const char *name)
	{
		NSString* content = loadNSStringInBundle([NSString stringWithUTF8String:name]);
		return toWebCoreString(content);
	}
}