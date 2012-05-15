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
#import "OAGLViewController.h"
#import "ScriptBridge.h"
#import "Director.h"

using namespace Aphid;

@interface OAGLViewController () {
	ScriptBridge* _scriptBridge;
	UIView* _glView;
}

- (void) willResignActive;
- (void) didBecomeActive;
- (void) didEnterBackground;
- (void) willEnterForeground;
- (void) willTerminate;
@end

@implementation OAGLViewController
@synthesize supportedOrientations;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
	if (self) {
		self.wantsFullScreenLayout = YES;
		self.supportedOrientations = OAOrientationMaskLandscapeLeft | OAOrientationMaskPortrait;
		// Custom initialization
		_scriptBridge = [[ScriptBridge alloc] init];
		[_scriptBridge initialize];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(willResignActive) name:UIApplicationWillResignActiveNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackground) name:UIApplicationDidEnterBackgroundNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(willEnterForeground) name:UIApplicationWillEnterForegroundNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(willTerminate) name:UIApplicationWillTerminateNotification object:nil];
	}
	return self;
}

- (void)dealloc
{
	[self terminate];
	[super dealloc];
}

- (void)configBundleName:(NSString *)bundleName baseURL:(NSURL *)baseURL developMode:(BOOL)developMode
{
	_scriptBridge.bundleName = bundleName;
	_scriptBridge.baseURL = baseURL;
	_scriptBridge.developMode = developMode;
}

- (void)configGLViewPixelFormat:(OAGLViewPixelFormat)format
{
	_scriptBridge.glViewPixelFormat = format;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	// Release any retained subviews of the main view.
}

- (void)loadView
{
	if (!_glView) {
		UIWindow *window = [UIApplication sharedApplication].keyWindow;
		if (!window)
			window = [[UIApplication sharedApplication].windows objectAtIndex:0];
		_glView = [_scriptBridge createGLView:window];
	}
	
	self.view = _glView;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	switch(interfaceOrientation){
		case UIInterfaceOrientationPortrait:
			return (self.supportedOrientations & OAOrientationMaskPortrait) > 0;
		case UIInterfaceOrientationPortraitUpsideDown:
			return (self.supportedOrientations & OAOrientationMaskPortraitUpsideDown) > 0;
		case UIInterfaceOrientationLandscapeLeft:
			return (self.supportedOrientations & OAOrientationMaskLandscapeLeft) > 0;
		case UIInterfaceOrientationLandscapeRight:
			return (self.supportedOrientations & OAOrientationMaskLandscapeRight) > 0;
		default:
			NSLog(@"Invalid UIInterfaceOrientation: %d", interfaceOrientation);
			return UIInterfaceOrientationIsPortrait(interfaceOrientation);
	}		
	
	return NO;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
	UNUSED_PARAM(duration);
	
	CGRect screenRect = [[UIScreen mainScreen] bounds];
	CGRect rect = UIInterfaceOrientationIsPortrait(toInterfaceOrientation) ? screenRect : CGRectMake(0, 0, screenRect.size.height, screenRect.size.width);
	
	/*Director* director = Director::sharedDirector();
	if (director) {
		rect.size.width *= director->contentScaleFactor();
		rect.size.height *= director->contentScaleFactor();
	}*/
	
	_glView.frame = rect;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	UNUSED_PARAM(fromInterfaceOrientation);
	[[UIApplication sharedApplication] setStatusBarOrientation:self.interfaceOrientation animated:NO];
}

- (BOOL)evaluateScript:(NSString *)filename
{
	return [_scriptBridge evaluateScriptFromFile:filename];
}

- (void)setScriptBinding:(id<OABindingProtocol>)binding name:(NSString *)name iOSOnly:(BOOL)iosOnly
{
	[_scriptBridge setScriptBinding:binding name:name iOSOnly:iosOnly];
}

- (void)terminate
{
	[_scriptBridge release];
	_scriptBridge = nil;
	[_glView removeFromSuperview];
	_glView = nil;
	
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)willResignActive
{
	if (_glView)
		Director::sharedDirector()->pause();
}

- (void)didBecomeActive
{
	if (_glView)
		Director::sharedDirector()->resume();
}

- (void)didEnterBackground
{
	if (_glView)
		Director::sharedDirector()->stopAnimation();
}

- (void)willEnterForeground
{
	if (_glView)
		Director::sharedDirector()->startAnimation();
}

- (void)willTerminate
{
	[self terminate];
}

@end
