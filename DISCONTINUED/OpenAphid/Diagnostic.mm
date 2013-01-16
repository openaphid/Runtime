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
#include "Diagnostic.h"
#include "AJOABinding.h"
#include "iOSUtils.h"
#include "Director.h"
#include "OAGlobalObject.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

namespace Aphid {

  void Diagnostic::error(NSString *message, bool logInConsole)
  {
	  if ([message length]) {
		  if (logInConsole)
			  oa_log("(OpenAphid Diagnostic) Error", [message cStringUsingEncoding:NSUTF8StringEncoding]);
		  if (OAGlobalObject::s_develop_mode)
			  displayNotification([UIColor redColor], message, 8);
	  }
  }

  void Diagnostic::warn(NSString *message, bool logInConsole)
  {
	  if (logInConsole)
		  oa_log("(OpenAphid Diagnostic) Warning", [message cStringUsingEncoding:NSUTF8StringEncoding]);
	  if (OAGlobalObject::s_develop_mode)
		  displayNotification([UIColor blueColor], message);
  }

  void Diagnostic::error(const AJ::UString& message, bool logInConsole)
  {
	  error(toNSString(message), logInConsole);
  }

  void Diagnostic::warn(const AJ::UString &message, bool logInConsole)
  {
	  warn(toNSString(message), logInConsole);
  }

  UILabel *Diagnostic::s_notification_view = nil;
  double Diagnostic::s_hide_time = 0;

  void Diagnostic::displayNotification(UIColor *color, NSString *message, float expireSeconds)
  {
	  if (!s_notification_view) {
		  s_notification_view = [[UILabel alloc] init];
		  s_notification_view.font = [UIFont systemFontOfSize:18];
		  s_notification_view.backgroundColor = [UIColor blackColor];
		  s_notification_view.textColor = [UIColor whiteColor];
		  s_notification_view.numberOfLines = 0;
		  s_notification_view.lineBreakMode = UILineBreakModeWordWrap;
			s_notification_view.opaque = YES;
	  }

	  s_notification_view.backgroundColor = color;
	  s_notification_view.text = message;

	  UIView *parentView = Director::sharedDirector()->glView();
		if (!parentView)
			parentView = [UIApplication sharedApplication].keyWindow;
		if (!parentView)
			parentView = [[UIApplication sharedApplication].windows objectAtIndex:0];
		
		Size size = Director::sharedDirector()->winSize();
		
	  CGRect frame = s_notification_view.frame;
	  frame.size = [s_notification_view sizeThatFits:size];
	  frame.size.width = size.width;
		
	  s_notification_view.frame = frame;

	  if (s_notification_view.superview != parentView)
		  [parentView addSubview:s_notification_view];

	  s_notification_view.hidden = NO;

	  s_hide_time = CFAbsoluteTimeGetCurrent() + expireSeconds;

	  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, expireSeconds * NSEC_PER_SEC), dispatch_get_main_queue(), ^
	  {
		  if (CFAbsoluteTimeGetCurrent() >= s_hide_time)
			  s_notification_view.hidden = YES;
	  });
  }
}