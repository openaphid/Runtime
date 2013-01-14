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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "OAConstants.h"
#import "OABindingProtocol.h"

@interface ScriptBridge : NSObject

@property(nonatomic, copy, readwrite) NSURL* baseURL;
@property(nonatomic, copy, readwrite) NSString* bundleName;
@property(nonatomic, assign, readwrite) BOOL developMode;

@property(nonatomic, assign, readwrite) OAGLViewPixelFormat glViewPixelFormat;
@property(nonatomic, assign, readwrite) GLuint glViewDepthFormat;

- (void) initialize;

- (BOOL) evaluateScriptFromFile:(NSString *)filename;

- (BOOL) evaluateScript:(NSString *)script URLString:(NSString *)urlString repeat:(NSUInteger)repeat;

- (void)setScriptBinding:(id<OABindingProtocol>)receiver name:(NSString *)name iOSOnly:(BOOL)iosOnly;

- (BOOL) garbageCollection;

- (UIView*) createGLView:(UIWindow*)window;
	
@end
