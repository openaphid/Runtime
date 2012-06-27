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
#include <sys/time.h>
#include <stdio.h>

#include "Director.h"
#include "AJDirector.h"
#include "Node.h"
#include "Scene.h"
#include "G2DMacros.h"
#include "OAUtil.h"
#include "OAGlobalObject.h"
#include "glu.h"

namespace Aphid {

#define kDefaultFPS 60.0

  ///-------------------------------------------------------------------------------------------------------------------
  Director::Director()
  : m_runningScene(0)
  , m_nextScene(0)
  , m_notificationNode(0)
  , m_oldAnimationInterval(1 / kDefaultFPS)
  , m_animationInterval(1 / kDefaultFPS)
  , m_projection(kCCDirectorProjectionDefault)
  , m_displayFPS(false)
  , m_totalFrames(0)
  , m_frames(0)
  , m_paused(false)
  , m_winSizeInPixels(SizeZero)
  , m_winSizeInPoints(SizeZero)
  , m_deviceOrientation(kCCDeviceOrientationPortrait)
	, m_contentScaleSupported(false)
  {
  	LEAK_DETECT_INC("Director");
	  
	  NamespaceG2D::s_content_scale_factor = 1;
	  
#if PLATFORM(IPHONE)
	  m_glView = ObjCObject::create();
#endif
  }

  Director::~Director()
  {
	  //TODO:
	  LEAK_DETECT_DEC("Director");
  }

  Director *Director::sharedDirector()
  {
		if (OAGlobalObject::sharedInstance() != 0) {
			Director *d = OAGlobalObject::sharedInstance()->namespaceG2D()->director();
			ASSERT(d);
			return d;
		} else
			return 0;
  }

  //--------------------------------------------------------------------------------------------------------------------
  //Getters and setters
  void Director::puegeCachedData()
  {
	  OA_TODO();
	  /*
				 [CCLabelBMFont purgeCachedData];
				 [[CCTextureCache sharedTextureCache] removeUnusedTextures];
				 */
  }

  float Director::zEye()
  {
	  return m_winSizeInPixels.height / 1.1566f;
  }

  void Director::reshapeProjection(const Size& windowSize)
  {
	  m_winSizeInPixels = m_winSizeInPoints = windowSize;
	  setProjection(m_projection);
  }

	///TODO: review all scene management APIs, try to make them less error-prone
  void Director::runWithScene(PassRefPtr<Scene> scene)
  {
	  ASSERT(scene);
	  
		/*
		ASSERT(!m_runningScene);
	  pushScene(scene);
		startAnimation();
		 */
		replaceScene(scene);
  }

  void Director::replaceScene(PassRefPtr<Scene> prpScene)
  {
	  ASSERT(prpScene);
		
	  RefPtr<Scene> scene = prpScene;
		
		m_sendCleanupToScene = m_runningScene != 0;
	  if (m_sceneStack.size() > 0)
		  m_sceneStack.removeLast();
	  m_sceneStack.append(scene);
	  m_nextScene = scene.get();
		
		//different impl from CC
		if (!m_runningScene)
			startAnimation();
  }

  void Director::pushScene(PassRefPtr<Scene> prpScene)
  {
	  ASSERT(prpScene);

	  RefPtr<Scene> scene = prpScene;

	  m_sendCleanupToScene = false;
	  m_sceneStack.append(scene);
	  m_nextScene = scene.get();
  }

  void Director::popScene()
  {
	  ASSERT(m_runningScene);

	  m_sceneStack.removeLast();

	  size_t c = m_sceneStack.size();

	  if (c == 0)
		  end();
	  else {
		  m_sendCleanupToScene = true;
		  m_nextScene = m_sceneStack.last().get();
	  }
  }

  void Director::end()
  {
		if (m_runningScene) {
			m_runningScene->onExit();
			m_runningScene->cleanup();
			m_runningScene = 0;
		}	  

	  m_nextScene = 0;

	  m_sceneStack.clear();

	  stopAnimation();

	  /*TODO:
				 [projectionDelegate_ release];
				 projectionDelegate_ = nil;
				 */

	  /* TODO: clean cached
				 // Purge bitmap cache
				 [CCLabelBMFont purgeCachedData];

				 // Purge all managers
				 [CCAnimationCache purgeSharedAnimationCache];
				 [CCSpriteFrameCache purgeSharedSpriteFrameCache];
				 [CCScheduler purgeSharedScheduler];
				 [CCActionManager purgeSharedManager];
				 [CCTextureCache purgeSharedTextureCache];
				 */
#if PLATFORM(IPHONE)
	  m_glView = ObjCObject::create();
#endif
  }

  void Director::pause()
  {
	  if (m_paused)
		  return;

	  m_oldAnimationInterval = m_animationInterval;
	  setAnimationInterval(1 / 4.0);
	  m_paused = true;
  }

  void Director::resume()
  {
	  if (!m_paused)
		  return;

	  setAnimationInterval(m_oldAnimationInterval);

	  if (gettimeofday(&m_lastUpdate, NULL) != 0) {
		  oa_error("Director: Error in gettimeofday");
	  }

	  m_paused = false;
	  dt = 0;
  }
	
	float Director::s_fps_interval = 1.0f;

  void Director::showFPS()
  {
	  m_frames++;
	  m_accumDt += dt;

	  if (m_accumDt > s_fps_interval) {
		  m_frameRate = m_frames / m_accumDt;
		  m_frames = 0;
		  m_accumDt = 0;

		  char buf[128];
		  sprintf(buf, "%.2f", m_frameRate);

		  m_fpsTexture = Texture2D::createWithString(buf, SizeZero, 24);
	  }

	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Unneeded states: GL_COLOR_ARRAY
	  if (m_fpsTexture) {
		  glDisableClientState(GL_COLOR_ARRAY);

		  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		  glColor4ub(224, 224, 244, 200);
		  m_fpsTexture->drawAtPoint(PointMake(15, 15));

		  glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

		  // restore default GL state
		  glEnableClientState(GL_COLOR_ARRAY);
	  }
  }

  void Director::markObjects(AJ::MarkStack &markStack, unsigned markID)
  {
		if (isMarked(markID))
			return;
	  DirectWrapper<AJDirector>::markObjects(markStack, markID);
		
		if (m_runningScene)
			m_runningScene->markObjects(markStack, markID);
		
		if (m_nextScene)
			m_nextScene->markObjects(markStack, markID);
		
	  for (SceneStack::const_iterator it = m_sceneStack.begin(); it != m_sceneStack.end(); ++it) {
		  it->get()->markObjects(markStack, markID);
	  }
  }

  //--------------------------------------------------------------------------------------------------------------------
  //Private

  void Director::setGLDefaultValues()
  {
	  setAlphaBlending(true);
	  setDepthTest(true);
	  setProjection(m_projection);

	  // set other opengl default values
	  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  }

  void Director::setAlphaBlending(bool on)
  {
	  if (on) {
		  glEnable(GL_BLEND);
		  glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	  } else
		  glDisable(GL_BLEND);
  }

  void Director::setDepthTest(bool on)
  {
	  if (on) {
		  ccglClearDepth(1.0f);
		  glEnable(GL_DEPTH_TEST);
		  glDepthFunc(GL_LEQUAL);
		  //		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	  } else
		  glDisable(GL_DEPTH_TEST);
  }

  void Director::calculateDeltaTime()
  {
	  //TODO: switch to CFAbsoluteTime??
	  struct timeval now;

	  if (gettimeofday(&now, 0) != 0) {
		  oa_error("error in gettimeofday");
		  dt = 0;
		  return;
	  }

	  // new delta time
	  if (m_nextDeltaTimeZero) {
		  dt = 0;
		  m_nextDeltaTimeZero = false;
	  } else {
		  dt = (now.tv_sec - m_lastUpdate.tv_sec) + (now.tv_usec - m_lastUpdate.tv_usec) / 1000000.0f;
		  dt = fmaxf(0, dt);
	  }

#if OA_DEV
		// If we are debugging our code, prevent big delta time
		if( dt > 0.2f )
			dt = 1/60.0f;
#endif

	  m_lastUpdate = now;
  }

  void Director::setNextScene()
  {
	  bool runningIsTransition = m_runningScene ? m_runningScene->isTransition() : false;
	  bool newIsTransition = m_nextScene->isTransition();

	  // If it is not a transition, call onExit/cleanup
	  if (!newIsTransition && m_runningScene) {
		  m_runningScene->onExit();

		  // issue #709. the root node (scene) should receive the cleanup message too
		  // otherwise it might be leaked.
		  if (m_sendCleanupToScene)
			  m_runningScene->cleanup();
	  }

	  m_runningScene = m_nextScene;
	  m_nextScene = 0;

	  if (!runningIsTransition) {
		  m_runningScene->onEnter();
		  m_runningScene->onEnterTransitionDidFinish();
	  }
  }
  
  void Director::drawScene()
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
		
	  CC_DISABLE_DEFAULT_GL_STATES();
		
	  glPopMatrix();
		
	  m_totalFrames++;
  }
  
  void Director::setProjection(ccDirectorProjection projection)
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
  
  float Director::contentScaleFactor()
  {
	  return NamespaceG2D::s_content_scale_factor;
  }
	
  void Director::setContentScaleFactor(float scaleFactor)
  {
	  if (scaleFactor != NamespaceG2D::s_content_scale_factor) {
			
		  NamespaceG2D::s_content_scale_factor = scaleFactor;
		  m_winSizeInPixels = toPixels(m_winSizeInPoints);
			
#if PLATFORM(IPHONE)
		  if (m_glView->impl() != nil)
			  updateContentScaleFactor();
#endif
			
		  // update projection
		  setProjection(m_projection);
	  }
  }
  
  Point Director::convertToGL(const Point& uiPoint)
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
	      break;
	  }
	  return ret;
  }
	
  Point Director::convertToUI(const Point& glPoint)
  {
	  Size winSize = m_winSizeInPoints;
	  int oppositeX = winSize.width - glPoint.x;
	  int oppositeY = winSize.height - glPoint.y;
	  Point uiPoint = PointZero;
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
	      break;
	  }
	  return uiPoint;
  }
  
  Size Director::winSize() const
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
	
  Size Director::winSizeInPixels() const
  {
		return toPixels(winSize());
  }
  
  void Director::applyOrientation()
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

} //namespace Aphid
