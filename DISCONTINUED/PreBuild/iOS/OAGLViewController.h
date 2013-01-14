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

#import <UIKit/UIKit.h>
#import "OAConstants.h"
#import "OABindingProtocol.h"

@interface OAGLViewController : UIViewController
@property(nonatomic, assign, readwrite) int supportedOrientations;
- (void) configBundleName:(NSString*)bundleName baseURL:(NSURL*)baseURL developMode:(BOOL)developMode;
- (void) configGLViewPixelFormat:(OAGLViewPixelFormat)pixelFormat;
- (BOOL) evaluateScript:(NSString*)filename;
- (void) setScriptBinding:(id<OABindingProtocol>)binding name:(NSString*)name iOSOnly:(BOOL)iosOnly;
- (void) terminate;
@end
