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
#include "Director+iOS.h"
#include "Node.h"
#include "Scene.h"

#include "G2DMacros.h"

#import "EAGLView.h"

#include "OAUtil.h"

#include "NamespaceG2D.h"

#include <sys/time.h>

#include "OAGlobalObject.h"
#include "glu.h"

@interface OADisplayLinkBridge : NSObject {
	Aphid::Director *_director;
}
- (id)initWithDirector:(Aphid::Director *)director;

- (void)mainLoop:(id)sender;
@end

@implementation OADisplayLinkBridge

- (id)initWithDirector:(Aphid::Director *)director
{
	self = [super init];
	if (self) {
		_director = director;
	}
	
	return self;
}

- (void)mainLoop:(id)sender
{
	UNUSED_PARAM(sender);
	
	if (_director)
		_director->drawScene();
}

- (void)clear
{
	_director = 0;
}

- (void)drawScene
{
	if (_director)
		_director->drawScene();
}

@end

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// Director iOS Specific Implementations
	void Director::setGLView(id glView)
  {
	  //TODO: check equivalent?
	  m_glView = ObjCObject::create(glView);

	  m_winSizeInPixels = m_winSizeInPoints = [glView bounds].size;

	  setGLDefaultValues();
  }

	CCTexture2DPixelFormat Director::pixelFormat()
  {
	  OAGLViewPixelFormat pixelFormat = [m_glView->impl() pixelFormat];
		switch (pixelFormat) {
			case OAGLViewPixelFormatRGB565:
				return kCCTexture2DPixelFormat_RGB565;
			case OAGLViewPixelFormatRGBA8:
				return kCCTexture2DPixelFormat_RGBA8888;
			default:
				oa_error("Unknown pixel format from glview: %d", pixelFormat);
				return kCCTexture2DPixelFormat_RGB565;
			
		}
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
	
  DirectorIOS::DirectorIOS()
  : m_deviceOrientation(kCCDeviceOrientationPortrait)
  , m_contentScaleSupported(false)
  , m_displayLink(nil)
  {
	  LEAK_DETECT_INC("DirectorIOS");
		
	  NamespaceG2D::s_content_scale_factor = 1;
		
	  m_bridge = [[OADisplayLinkBridge alloc] initWithDirector:this];
		
	  m_runningThread = [NSThread currentThread];
  }
	
  DirectorIOS::~DirectorIOS()
  {
	  LEAK_DETECT_DEC("DirectorIOS");
	  [m_bridge clear];
	  [m_bridge release];
  }
	
  void DirectorIOS::drawScene()
  {
	  Director::drawScene();
		
	  [m_glView->impl() swapBuffers];
  }
	
  void DirectorIOS::setGLView(id view)
  {
	  if (view != m_glView->impl()) {
		  Base::setGLView(view);
			
			m_winSizeInPixels = toPixels(m_winSizeInPoints);
			
		  if (G2D_CONTENT_SCALE_FACTOR() != 1)
			  updateContentScaleFactor();
	  }
  }
	
  void DirectorIOS::updateContentScaleFactor()
  {
	  // Based on code snippet from: http://developer.apple.com/iphone/prerelease/library/snippets/sp2010/sp28.html
	  if ([m_glView->impl() respondsToSelector:@selector(setContentScaleFactor:)]) {
			UIView* glView = m_glView->impl();
		  [glView setContentScaleFactor:NamespaceG2D::s_content_scale_factor];
			[glView setNeedsLayout]; //this is very important to reset backsize of renderer and projection
			
			
		  m_contentScaleSupported = true;
	  }
	  else
		  oa_info("'setContentScaleFactor:' is not supported on this device");
  }
	
  bool DirectorIOS::enableRetinaDisplay(bool enabled)
  {
	  // Already enabled ?
	  if (enabled && G2D_CONTENT_SCALE_FACTOR() == 2)
		  return true;
		
	  // Already disabled
	  if (!enabled && G2D_CONTENT_SCALE_FACTOR() == 1)
		  return true;
		
	  // setContentScaleFactor is not supported
	  if (![m_glView->impl() respondsToSelector:@selector(setContentScaleFactor:)])
		  return false;
		
	  // SD device
	  if ([[UIScreen mainScreen] scale] == 1.0)
		  return false;
		
	  float newScale = enabled ? 2 : 1;
	  setContentScaleFactor(newScale);
		
	  return true;
  }
	
	bool DirectorIOS::multipleTouchEnabled() const
	{
		return ((UIView*)(m_glView->impl())).multipleTouchEnabled;
	}
	
	void DirectorIOS::setMultipleTouchEnabled(bool b)
	{
		[m_glView->impl() setMultipleTouchEnabled:(b ? YES : NO)];
	}
	
  void DirectorIOS::reshapeProjection(const Size& size)
  {
		UNUSED_PARAM(size);
	  m_winSizeInPoints = [m_glView->impl() bounds].size;
	  m_winSizeInPixels = toPixels(m_winSizeInPoints);
	  setProjection(m_projection);
  }
	
  void DirectorIOS::setDeviceOrientation(ccDeviceOrientation orientation)
  {
	  if (m_deviceOrientation != orientation) {
		  m_deviceOrientation = orientation;
		  switch (m_deviceOrientation) {
			  case kCCDeviceOrientationPortrait:
		      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
		      break;
			  case kCCDeviceOrientationPortraitUpsideDown:
		      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortraitUpsideDown animated:NO];
		      break;
			  case kCCDeviceOrientationLandscapeLeft:
		      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
		      break;
			  case kCCDeviceOrientationLandscapeRight:
		      [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft animated:NO];
		      break;
			  default:
		      oa_error("Unknown device orientation");
		      break;
		  }
	  }
  }
	
  void DirectorIOS::end()
  {
	  Base::end();
  }
	
  void DirectorIOS::setAnimationInterval(double interval)
  {
	  m_animationInterval = interval;
	  if (m_displayLink) {
		  stopAnimation();
		  startAnimation();
	  }
  }
	
  void DirectorIOS::startAnimation()
  {
	  ASSERT(!m_displayLink);
	  if (gettimeofday(&m_lastUpdate, 0) != 0) {
		  oa_error("Error on gettimeofday");
	  }
		
	  // approximate frame rate
	  // assumes device refreshes at 60 fps
	  int frameInterval = (int) floor(m_animationInterval * 60.0f);
		
	  oa_debug("Frame interval: %d", frameInterval);
		
	  m_displayLink = [CADisplayLink displayLinkWithTarget:m_bridge selector:@selector(mainLoop:)];
		
	  [m_displayLink setFrameInterval:frameInterval];
	  [m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
  }
	
  void DirectorIOS::stopAnimation()
  {
	  [m_displayLink invalidate];
	  m_displayLink = nil;
  }
	
  void DirectorIOS::avoidLayoutFlicker()
  {
		if ([NSThread isMainThread])
			[m_bridge drawScene];
		else
			[m_bridge performSelectorOnMainThread:@selector(drawScene) withObject:nil waitUntilDone:YES];
  }
	
}
