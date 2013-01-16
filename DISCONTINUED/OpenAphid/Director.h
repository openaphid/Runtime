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

#ifndef OpenAphid_Director_h
#define OpenAphid_Director_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"

#include "G2DConfig.h"
#include "G2DProtocols.h"

#include "Texture2D.h"

#include "Node.h"
#include "Scene.h"

#if PLATFORM(IPHONE)
#include "ObjCObject.h"
#ifdef __OBJC__
#import <UIKit/UIKit.h>
#endif
#endif

namespace Aphid {
	class AJDirector;
	class Scene;
	class LabelAtlas;
	class Node;
	class DirectorIOS;
	
	enum ccDirectorProjection{
		/// sets a 2D projection (orthogonal projection).
		kCCDirectorProjection2D = 1,
		
		/// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
		kCCDirectorProjection3D = 2,
		
		/// it calls "updateProjection" on the projection delegate.
		kCCDirectorProjectionCustom = 3,
		
		/// Detault projection is 3D projection
		kCCDirectorProjectionDefault = kCCDirectorProjection3D,
		
		/*
		// backward compatibility stuff
		CCDirectorProjection2D = kCCDirectorProjection2D,
		CCDirectorProjection3D = kCCDirectorProjection3D,
		CCDirectorProjectionCustom = kCCDirectorProjectionCustom,
		*/
		
	};
	
#if PLATFORM(IPHONE)
#ifdef __OBJC__
	typedef enum {
		/// Device oriented vertically, home button on the bottom
		kCCDeviceOrientationPortrait = UIDeviceOrientationPortrait,	
		/// Device oriented vertically, home button on the top
    kCCDeviceOrientationPortraitUpsideDown = UIDeviceOrientationPortraitUpsideDown,
		/// Device oriented horizontally, home button on the right
    kCCDeviceOrientationLandscapeLeft = UIDeviceOrientationLandscapeLeft,
		/// Device oriented horizontally, home button on the left
    kCCDeviceOrientationLandscapeRight = UIDeviceOrientationLandscapeRight,
	} ccDeviceOrientation;
#else
	typedef enum {
		/// Device oriented vertically, home button on the bottom
		kCCDeviceOrientationPortrait = 1, //UIDeviceOrientationPortrait,	
		/// Device oriented vertically, home button on the top
    kCCDeviceOrientationPortraitUpsideDown = 2, //UIDeviceOrientationPortraitUpsideDown,
		/// Device oriented horizontally, home button on the right
    kCCDeviceOrientationLandscapeLeft = 3, //UIDeviceOrientationLandscapeLeft,
		/// Device oriented horizontally, home button on the left
    kCCDeviceOrientationLandscapeRight = 4, //UIDeviceOrientationLandscapeRight,
	} ccDeviceOrientation;
#endif //__OBJC__
#else
	enum ccDeviceOrientation{
		/// Device oriented vertically, home button on the bottom
		kCCDeviceOrientationPortrait = 1, //UIDeviceOrientationPortrait,	
		/// Device oriented vertically, home button on the top
    kCCDeviceOrientationPortraitUpsideDown = 2, //UIDeviceOrientationPortraitUpsideDown,
		/// Device oriented horizontally, home button on the right
    kCCDeviceOrientationLandscapeLeft = 3, //UIDeviceOrientationLandscapeLeft,
		/// Device oriented horizontally, home button on the left
    kCCDeviceOrientationLandscapeRight = 4, //UIDeviceOrientationLandscapeRight,
	} ;
#endif
	
	
	
	typedef enum {
		/** Will use a Director that triggers the main loop from an NSTimer object
		 *
		 * Features and Limitations:
		 * - Integrates OK with UIKit objects
		 * - It the slowest director
		 * - The invertal update is customizable from 1 to 60
		 */
		kCCDirectorTypeNSTimer,
		
		/** will use a Director that triggers the main loop from a custom main loop.
		 *
		 * Features and Limitations:
		 * - Faster than NSTimer Director
		 * - It doesn't integrate well with UIKit objecgts
		 * - The interval update can't be customizable
		 */
		kCCDirectorTypeMainLoop,
		
		/** Will use a Director that triggers the main loop from a thread, but the main loop will be executed on the main thread.
		 *
		 * Features and Limitations:
		 * - Faster than NSTimer Director
		 * - It doesn't integrate well with UIKit objecgts
		 * - The interval update can't be customizable
		 */
		kCCDirectorTypeThreadMainLoop,
		
		/** Will use a Director that synchronizes timers with the refresh rate of the display.
		 *
		 * Features and Limitations:
		 * - Faster than NSTimer Director
		 * - Only available on 3.1+
		 * - Scheduled timers & drawing are synchronizes with the refresh rate of the display
		 * - Integrates OK with UIKit objects
		 * - The interval update can be 1/60, 1/30, 1/15
		 */	
		kCCDirectorTypeDisplayLink,
		
		/** Default director is the NSTimer directory */
		kCCDirectorTypeDefault = kCCDirectorTypeNSTimer,
		
		// backward compatibility stuff
		CCDirectorTypeNSTimer = kCCDirectorTypeNSTimer,
		CCDirectorTypeMainLoop = kCCDirectorTypeMainLoop,
		CCDirectorTypeThreadMainLoop = kCCDirectorTypeThreadMainLoop,
		CCDirectorTypeDisplayLink = kCCDirectorTypeDisplayLink,
		CCDirectorTypeDefault = kCCDirectorTypeDefault,		
	} ccDirectorType;
	
	///-------------------------------------------------------------------------------------------------------------------
	class Director : public RefCounted<Director> , public DirectWrapper<AJDirector> {
		typedef ATF::Vector<RefPtr<Scene> > SceneStack;
		
	public:
		virtual ~Director();
		
		static Director* sharedDirector();
		
		static float s_fps_interval;
		
		virtual void drawScene();
		virtual void setProjection(ccDirectorProjection p);
		
#if PLATFORM(IPHONE)
#ifdef __OBJC__
		virtual void setGLView(id glView);
#else
		virtual void setGLView(void* glView);
#endif
#endif
		
#if PLATFORM(ANDROID)
		virtual void setSurfaceSize(int width, int height) = 0;
#endif
		
		virtual void reshapeProjection(const Size& windowSize);
		virtual Point convertToGL(const Point& p);
		virtual Point convertToUI(const Point& p);
		virtual Size winSize() const;
		virtual Size winSizeInPixels() const;
		virtual void end();
		virtual void startAnimation() = 0;
		virtual void stopAnimation() = 0;
		virtual void setAnimationInterval(double interval) = 0;
		virtual void avoidLayoutFlicker() {}
		
		Point uiPointInNode(const Point& p, Node* node)
		{
			ASSERT(node);
			return node->convertToNodeSpace(convertToGL(p));
		}
		
		void puegeCachedData();
		float zEye();
		
		Size displaySizeInPixels() const {return m_winSizeInPixels;}
		
		void runWithScene(PassRefPtr<Scene> scene);
		void replaceScene(PassRefPtr<Scene> prpScene);
		void pushScene(PassRefPtr<Scene> prpScene);
		void popScene();
		
		void pause();
		void resume();
		
		virtual void applyOrientation();
		virtual void setDeviceOrientation(ccDeviceOrientation orientation) = 0;
		virtual ccDeviceOrientation deviceOrientation() const {return m_deviceOrientation;}
		virtual void setContentScaleFactor(float scaleFactor);
		virtual float contentScaleFactor();
		virtual bool enableRetinaDisplay(bool on) = 0;
		virtual bool multipleTouchEnabled() const = 0;
		virtual void setMultipleTouchEnabled(bool b) = 0;

		//Getters and setters
#if PLATFORM(IPHONE)
#ifdef __OBJC__
		NSThread* runningThread() const {return m_runningThread;}
		id glView() const {return m_glView->impl();}
#else
		void* runningThread() const {return m_runningThread;}
		void* glView() const {return m_glView->impl();}
#endif
#endif
		
		Scene* runningScene() const {return m_runningScene.get();}
		
		double animationInterval() const {return m_animationInterval;}		
		
		bool displayFPS() const {return m_displayFPS;}
		void setDisplayFPS(bool display) {m_displayFPS = display;}		
		
		bool nextDeltaTimeZero() const {return m_nextDeltaTimeZero;}
		void setNexDeltaTimeZero(bool isZero) {m_nextDeltaTimeZero = isZero;}
		
		bool isPaused() const {return m_paused;}
		
		ccDirectorProjection projection() const {return m_projection;}		
		
		size_t totalFrames() const {return m_totalFrames;}
		
		bool sendCleanupToScene() const {return m_sendCleanupToScene;}
		
		Node* notificationNode() const {return m_notificationNode.get();}
		void setNotificationNode(PassRefPtr<Node> node) {m_notificationNode = node;}
		
		CCTexture2DPixelFormat pixelFormat();
		
		void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		virtual DirectorIOS* toDirectorIOS() {return 0;}
		
	protected:
		Director();
		
	protected:
		void setGLDefaultValues();
		void setNextScene();
		void showFPS();
		void calculateDeltaTime();
		void setAlphaBlending(bool on);
		void setDepthTest(bool on);
		
	protected:
#if PLATFORM(IPHONE)
		RefPtr<ObjCObject> m_glView;
		
#ifdef __OBJC__
		NSThread* m_runningThread;
#else
		void* m_runningThread;
#endif
		
		// profiler
#if CC_ENABLE_PROFILERS
		ccTime accumDtForProfiler_;
#endif		
		//iOS

#endif //PLATFORM(IPHONE)
		
		ccDeviceOrientation m_deviceOrientation;
		bool m_contentScaleSupported;
		
		double m_animationInterval;
		double m_oldAnimationInterval;	
		
		/* display FPS ? */
		bool m_displayFPS;
		
		size_t m_frames;
		size_t m_totalFrames;
		
		ccTime m_accumDt;
		ccTime m_frameRate;
		
		/* is the running scene paused */
		bool m_paused;
		
		/* The running scene */
		RefPtr<Scene> m_runningScene;
		
		/* This object will be visited after the scene. Useful to hook a notification node */
		RefPtr<Node> m_notificationNode; //TODO: change to a more flexible protocol type?
		
		/* will be the next 'runningScene' in the next frame
		 nextScene is a weak reference. */
		Scene* m_nextScene;
		
		/* If YES, then "old" scene will receive the cleanup message */
		bool m_sendCleanupToScene;
		
		/* scheduled scenes */
		SceneStack m_sceneStack;
		
		/* last time the main loop was updated */
		struct timeval m_lastUpdate;
		/* delta time since last tick to main loop */
		ccTime dt;
		/* whether or not the next delta time will be zero */
		bool m_nextDeltaTimeZero;
		
		/* projection used */
		ccDirectorProjection m_projection;
		
		/* Projection protocol delegate */
		//TODO: ProjectionProtocol*	m_projectionDelegate;
		
		/* window size in points */
		Size	m_winSizeInPoints;
		
		/* window size in pixels */
		Size	m_winSizeInPixels;
		
		RefPtr<Texture2D> m_fpsTexture;
	}; // class Director
	
}// namespace Aphid

#endif
