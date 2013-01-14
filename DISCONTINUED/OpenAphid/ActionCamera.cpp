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
#include "ActionCamera.h"
#include "Node.h"
#include "G2DMacros.h"

namespace Aphid {
	void CameraAction::startWithTarget(Aphid::Node* target)
	{
		Base::startWithTarget(target);
		Camera* camera = target->camera();
		m_centerOrig = camera->center();
		m_eyeOrig = camera->eye();
		m_upOrig = camera->up();
	}
	
  ///-------------------------------------------------------------------------------------------------------------------
	/// OrbitCameraAction
	OrbitCameraAction::OrbitCameraAction(TimeSec duration, float r, float dr, float z, float dz, float x, float dx)
	: Base(duration)
	, m_radius(r)
	, m_deltaRadius(dr)
	, m_angleZ(z)
	, m_deltaAngleZ(dz)
	, m_angleX(x)
	, m_deltaAngleX(dx)
	{
		m_radDeltaZ = CC_DEGREES_TO_RADIANS(dz);
		m_radDeltaX = CC_DEGREES_TO_RADIANS(dx);
		
		LEAK_DETECT_INC("OrbitCameraAction");
	}
	
	OrbitCameraAction::~OrbitCameraAction()
	{
		LEAK_DETECT_DEC("OrbitCameraAction");
	}
	
	void OrbitCameraAction::startWithTarget(Aphid::Node *target)
	{
		Base::startWithTarget(target);
		float r, zenith, azimuth;
		sphericalRadius(&r, &zenith, &azimuth);
		
		m_radZ = CC_DEGREES_TO_RADIANS(m_angleZ);
		m_radX = CC_DEGREES_TO_RADIANS(m_angleX);
	}
	
	void OrbitCameraAction::update(TimeSec dt)
	{
		float r = (m_radius + m_deltaRadius * dt) * Camera::zEye();
		float za = m_radZ + m_radDeltaZ * dt;
		float xa = m_radX + m_radDeltaX * dt;
		
		float i = sinf(za) * cosf(xa) * r + m_centerOrig.x;
		float j = sinf(za) * sinf(xa) * r + m_centerOrig.y;
		float k = cosf(za) * r + m_centerOrig.z;
		
		m_target->camera()->setEye(Vector3Make(i, j, k));
	}
	
	void OrbitCameraAction::sphericalRadius(float *newRadius, float *zenith, float *azimuth)
	{
		Vector3 e, c;
		
		float x, y, z;
		float r; // radius
		float s;
		
		Camera* camera = m_target->camera();
		e = camera->eye();
		c = camera->center();
		
		x = e.x-c.x;
		y = e.y-c.y;
		z = e.z-c.z;
		
		r = sqrtf(x * x + y * y + z * z);
		s = sqrtf(x * x + y * y);
		if(s==0.0f)
			s = FLT_EPSILON;
		if(r==0.0f)
			r = FLT_EPSILON;
		
		*zenith = acosf( z/r);
		if( x < 0 )
			*azimuth = (float)M_PI - asinf(y/s);
		else
			*azimuth = asinf(y/s);
		
		*newRadius = r / Camera::zEye();
	}
}