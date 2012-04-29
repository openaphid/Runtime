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
#include "Director.h"
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
	  /* calculate "global" dt */
	  calculateDeltaTime();
		
	  /* tick before glClear: issue #533 */
	  if (!m_paused) {
		  OAGlobalObject::sharedInstance()->namespaceG2D()->scheduler()->tick(dt);
	  }
		
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	  /* to avoid flickr, nextScene MUST be here: after tick and before draw.
		 TODO: Which bug is this one. It seems that it can't be reproduced with v0.9 */
	  if (m_nextScene)
		  setNextScene();
		
	  glPushMatrix();
		
	  applyOrientation();
		
	  // By default enable VertexArray, ColorArray, TextureCoordArray and Texture2D
		
		CC_ENABLE_DEFAULT_GL_STATES();
		
	  /* draw the scene */
	  if (m_runningScene)
		  m_runningScene->visit();
		
	  if (m_notificationNode)
		  m_notificationNode->visit();
		
	  if (m_displayFPS)
		  showFPS();
		
#if CC_ENABLE_PROFILERS
		[self showProfilers];
#endif
		
	  CC_DISABLE_DEFAULT_GL_STATES();
		
	  glPopMatrix();
		
	  m_totalFrames++;
		
	  [m_glView->impl() swapBuffers];
  }
	
  void DirectorIOS::setProjection(ccDirectorProjection projection)
  {
	  Size size = m_winSizeInPixels;
		
	  switch (projection) {
		  case kCCDirectorProjection2D:
	      glViewport(0, 0, size.width, size.height);
	      glMatrixMode(GL_PROJECTION);
	      glLoadIdentity();
	      ccglOrtho(0, size.width, 0, size.height, -1024 * G2D_CONTENT_SCALE_FACTOR(), 1024 * G2D_CONTENT_SCALE_FACTOR());
	      glMatrixMode(GL_MODELVIEW);
	      glLoadIdentity();
	      break;
				
		  case kCCDirectorProjection3D:
		  {
			  float zeye = zEye();
				
			  glViewport(0, 0, size.width, size.height);
			  glMatrixMode(GL_PROJECTION);
			  glLoadIdentity();
			  //			gluPerspective(60, (GLfloat)size.width/size.height, zeye-size.height/2, zeye+size.height/2 );
			  gluPerspective(60, (GLfloat) size.width / size.height, 0.5f, 1500);
				
			  glMatrixMode(GL_MODELVIEW);
			  glLoadIdentity();
			  gluLookAt(size.width / 2, size.height / 2, zeye,
									size.width / 2, size.height / 2, 0,
									0.0f, 1.0f, 0.0f);
			  break;
		  }
				
		  case kCCDirectorProjectionCustom:
	      OA_TODO();
	      /*
				 if( projectionDelegate_ )
				 [projectionDelegate_ updateProjection];
				 */
	      break;
				
		  default:
	      oa_error("Director: unrecognized projecgtion");
	      break;
	  }
		
	  m_projection = projection;
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
	
  float DirectorIOS::contentScaleFactor()
  {
	  return NamespaceG2D::s_content_scale_factor;
  }
	
  void DirectorIOS::setContentScaleFactor(float scaleFactor)
  {
	  if (scaleFactor != NamespaceG2D::s_content_scale_factor) {
			
		  NamespaceG2D::s_content_scale_factor = scaleFactor;
		  m_winSizeInPixels = toPixels(m_winSizeInPoints);
			
		  if (m_glView->impl() != nil)
			  updateContentScaleFactor();
			
		  // update projection
		  setProjection(m_projection);
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
	
  void DirectorIOS::reshapeProjection(const Size& size)
  {
		UNUSED_PARAM(size);
	  m_winSizeInPoints = [m_glView->impl() bounds].size;
	  m_winSizeInPixels = toPixels(m_winSizeInPoints);
	  setProjection(m_projection);
  }
	
  Point DirectorIOS::convertToGL(const Point& uiPoint)
  {
	  Size s = m_winSizeInPoints;
	  float newY = s.height - uiPoint.y;
	  float newX = s.width - uiPoint.x;
		
	  Point ret = PointZero;
	  switch (m_deviceOrientation) {
		  case kCCDeviceOrientationPortrait:
	      ret = PointMake( uiPoint.x, newY );
	      break;
		  case kCCDeviceOrientationPortraitUpsideDown:
	      ret = PointMake(newX, uiPoint.y);
	      break;
		  case kCCDeviceOrientationLandscapeLeft:
	      ret.x = uiPoint.y;
	      ret.y = uiPoint.x;
	      break;
		  case kCCDeviceOrientationLandscapeRight:
	      ret.x = newY;
	      ret.y = newX;
	      break;
		  default:
	      oa_error("invalid orientation: %d", m_deviceOrientation);
	  }
	  return ret;
  }
	
  Point DirectorIOS::convertToUI(const Point& glPoint)
  {
	  Size winSize = m_winSizeInPoints;
	  int oppositeX = winSize.width - glPoint.x;
	  int oppositeY = winSize.height - glPoint.y;
	  CGPoint uiPoint = CGPointZero;
	  switch (m_deviceOrientation) {
		  case kCCDeviceOrientationPortrait:
	      uiPoint = PointMake(glPoint.x, oppositeY);
	      break;
		  case kCCDeviceOrientationPortraitUpsideDown:
	      uiPoint = PointMake(oppositeX, glPoint.y);
	      break;
		  case kCCDeviceOrientationLandscapeLeft:
	      uiPoint = PointMake(glPoint.y, glPoint.x);
	      break;
		  case kCCDeviceOrientationLandscapeRight:
	      // Can't use oppositeX/Y because x/y are flipped
	      uiPoint = PointMake(winSize.width - glPoint.y, winSize.height - glPoint.x);
	      break;
		  default:
	      oa_error("invalid orientation: %d", m_deviceOrientation);
	  }
	  return uiPoint;
  }
	
  Size DirectorIOS::winSize() const
  {
	  Size s = m_winSizeInPoints;
		
	  if (m_deviceOrientation == kCCDeviceOrientationLandscapeLeft || m_deviceOrientation == kCCDeviceOrientationLandscapeRight) {
		  // swap x,y in landscape mode
		  Size tmp = s;
		  s.width = tmp.height;
		  s.height = tmp.width;
	  }
	  return s;
  }
	
  Size DirectorIOS::winSizeInPixels() const
  {
		return toPixels(winSize());
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
	
  void DirectorIOS::applyOrientation()
  {
	  Size s = m_winSizeInPixels;
	  float w = s.width / 2;
	  float h = s.height / 2;
		
	  // TODO it's using hardcoded values.
	  // What if the the screen size changes in the future?
	  switch (m_deviceOrientation) {
		  case kCCDeviceOrientationPortrait:
	      // nothing
	      break;
		  case kCCDeviceOrientationPortraitUpsideDown:
	      // upside down
	      glTranslatef(w, h, 0);
	      glRotatef(180, 0, 0, 1);
	      glTranslatef(-w, -h, 0);
	      break;
		  case kCCDeviceOrientationLandscapeLeft:
	      glTranslatef(w, h, 0);
	      glRotatef(-90, 0, 0, 1);
	      glTranslatef(-h, -w, 0);
	      break;
		  case kCCDeviceOrientationLandscapeRight:
	      glTranslatef(w, h, 0);
	      glRotatef(90, 0, 0, 1);
	      glTranslatef(-h, -w, 0);
	      break;
		  default:
	      oa_error("Unknown orientation");
	      break;
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
