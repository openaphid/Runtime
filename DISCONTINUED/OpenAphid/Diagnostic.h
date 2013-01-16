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

#ifndef OpenAphid_Diagnostic_h
#define OpenAphid_Diagnostic_h

#include <wtf/FastAllocBase.h>
#include <wtf/Noncopyable.h>

#if PLATFORM(IPHONE)
#ifdef __OBJC__
@class UIColor;
@class UIView;
@class UILabel;
#endif
#endif

namespace Aphid {

#if PLATFORM(IPHONE)
#ifndef __OBJC__
	class NSString;
	class UIColor;
	class UIView;
	class UILabel;
#endif
#endif
	
	class String;
	
	class Diagnostic : public Noncopyable {
	public:
#if PLATFORM(IPHONE)
		static void error(NSString* message, bool logInConsole=true);
		static void warn(NSString* message, bool logInConsole=true);
		static void displayNotification(UIColor* color, NSString* message, float expireSeconds=3);
#endif
		
		static void error(const String& message, bool logInConsole=true);
		static void warn(const String& message, bool logInConsole=true);
		static void info(const String& message, bool logInConsole=true);
		
	private:
#if PLATFORM(IPHONE)
		static UILabel* s_notification_view;
		static double s_hide_time;
#endif
	};
}

#endif
