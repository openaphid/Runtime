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

#import "OARootViewController.h"

@interface OARootViewController ()

@end

@implementation OARootViewController
@synthesize glViewController;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
	if (self) {
		glViewController = [[OAGLViewController alloc] initWithNibName:nil bundle:nil];
	}
	return self;
}

- (void)dealloc
{
	NSLog(@"dealloc oa root view controller");
	[glViewController release];
	[super dealloc];
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

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return [glViewController shouldAutorotateToInterfaceOrientation:interfaceOrientation];
}

- (void)presentGLViewControllerWithScriptFile:(NSString*)scriptFilename
{
	[self presentModalViewController:self.glViewController animated:NO];
	if (scriptFilename)
		[self.glViewController evaluateScript:scriptFilename];
}

@end
