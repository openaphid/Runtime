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

#ifndef OpenAphid_ActionCamera_h
#define OpenAphid_ActionCamera_h

#include "Action.h"
#include "Camera.h"

namespace Aphid {
	class CameraAction : public IntervalAction {
		typedef IntervalAction Base;
	public:
		virtual ~CameraAction() {}
		
		virtual void startWithTarget(Node* target);
		
		virtual int baseType() const {return ActionTypeCamera | Base::baseType();}
		
	protected:
		CameraAction(TimeSec duration)
		: Base(duration)
		, m_centerOrig(Vector3Zero)
		, m_eyeOrig(Vector3Zero)
		, m_upOrig(Vector3Zero)
		{
		}
		
		Vector3 m_centerOrig;
		Vector3 m_eyeOrig;
		Vector3 m_upOrig;
	};
	
	class OrbitCameraAction : public CameraAction {
		typedef CameraAction Base;
		typedef OrbitCameraAction Self;
	public:
		virtual ~OrbitCameraAction();
		
		static ATF::PassRefPtr<Self> create(TimeSec duration, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX)
		{
			return adoptRef(new Self(duration, radius, deltaRadius, angleZ, deltaAngleZ, angleX, deltaAngleX));
		}
		
		virtual void startWithTarget(Node* target);
		virtual void update(TimeSec t);
		
		virtual const char* name() const {return "OrbitCameraAction";}		
		
	protected:
		OrbitCameraAction(TimeSec duration, float r, float dr, float z, float dz, float x, float dx);
		
		void sphericalRadius(float* newRadius, float* zenith, float* azimuth);
		
		virtual Self* doCreateClone()
		{
			return new Self(m_duration, m_radius, m_deltaRadius, m_angleZ, m_deltaAngleZ, m_angleX, m_deltaAngleX);
		}
		
		virtual void postSetupReverse(Action* reversed)
		{
			reversed->setTimeReversed(!isTimeReversed());
		}
		
		float m_radius;
		float m_deltaRadius;
		float m_angleZ;
		float m_deltaAngleZ;
		float m_angleX;
		float m_deltaAngleX;
		
		float m_radZ;
		float m_radDeltaZ;
		float m_radX;
		float m_radDeltaX;
	};
}

#endif
