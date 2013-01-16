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

#ifndef DIRECTOR_IOS_H_
#define DIRECTOR_IOS_H_

#include "Director.h"

#if !PLATFORM(IPHONE)
#error Director+iOS.h should only be used for iOS
#endif

namespace Aphid {
	class DirectorIOS : public Director {
		typedef Director Base;
	public:
		virtual ~DirectorIOS();
		
		static PassRefPtr<DirectorIOS> create()
		{
			return adoptRef(new DirectorIOS());
		}
		
		virtual void drawScene();
		virtual void setProjection(ccDirectorProjection projection);
		
#ifdef __OBJC__
		virtual void setGLView(id view);
#else
		virtual void setGLView(void* view);
#endif
		
		virtual void applyOrientation();
		virtual void setDeviceOrientation(ccDeviceOrientation orientation);
		virtual ccDeviceOrientation deviceOrientation() const {return m_deviceOrientation;}
		virtual void setContentScaleFactor(float scaleFactor);
		virtual float contentScaleFactor();
		virtual bool enableRetinaDisplay(bool on);
		virtual void reshapeProjection(const Size& windowSize);
		virtual Point convertToGL(const Point& p);
		virtual Point convertToUI(const Point& p);
		virtual Size winSize() const;
		virtual Size winSizeInPixels() const;
		virtual bool multipleTouchEnabled() const;
		virtual void setMultipleTouchEnabled(bool b);
		virtual void end();
		virtual void startAnimation();
		virtual void stopAnimation();
		virtual void setAnimationInterval(double interval);
		virtual void avoidLayoutFlicker();
		
		virtual DirectorIOS* toDirectorIOS() {return this;}
		
	protected:
		DirectorIOS();
		void updateContentScaleFactor();
		
	private:
		
#ifdef __OBJC__
		id m_displayLink;
		id m_bridge;
#else
		void* m_displayLink;
		void* m_bridge;
#endif
	};
}


#endif /* DIRECTOR_IOS_H_ */
