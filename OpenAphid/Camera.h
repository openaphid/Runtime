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

#ifndef OpenAphid_Camera_h
#define OpenAphid_Camera_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"
#include "OAData.h"

namespace Aphid {
	class AJCamera;
	
	///-------------------------------------------------------------------------------------------------------------------
	class Camera : public RefCounted<Camera>, public DirectWrapper<AJCamera> {
	public:
		Camera();
		virtual ~Camera();
		
		static PassRefPtr<Camera> create()
		{
			return adoptRef(new Camera());
		}
		
		static float zEye() {return FLT_EPSILON;}
		
		bool isDirty() const {return m_dirty;}
		void setDirty(bool v) {m_dirty = v;}
		
		Vector3 eye() const;
		void setEye(const Vector3& v);
		
		Vector3 center() const;
		void setCenter(const Vector3& v);
		
		Vector3 up() const;
		void setUp(const Vector3& v);
		
		void restore();
		void locate();
		
	private:
		Vector3 m_eye;
		Vector3 m_center;
		Vector3 m_up;
		
		bool m_dirty;
		
	}; // class Camera
	
}// namespace Aphid

#endif
