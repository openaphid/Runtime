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
#include "Camera.h"
#include "AJCamera.h"
#include "OAUtil.h"

#include "glu.h"

#include "NamespaceG2D.h"

namespace Aphid {
  ///-------------------------------------------------------------------------------------------------------------------
  Camera::Camera()
  {
	  restore();
	  LEAK_DETECT_INC("Camera");
  }

  Camera::~Camera()
  {
	  //oa_debug("camera destructed");
	  LEAK_DETECT_DEC("Camera");
  }

  void Camera::restore()
  {
	  m_eye.x = m_eye.y = 0;
	  m_eye.z = Camera::zEye();

	  m_center = Vector3Zero;

	  m_up.x = 0.0f;
	  m_up.y = 1.0f;
	  m_up.z = 0.0f;

	  m_dirty = false;
  }

  void Camera::locate()
  {
	  if (m_dirty)
		  gluLookAt(m_eye.x, m_eye.y, m_eye.z, m_center.x, m_center.y, m_center.z, m_up.x, m_up.y, m_up.z);
  }

  Vector3 Camera::eye() const
  {
	  return m_eye / NamespaceG2D::s_content_scale_factor;
  }

  void Camera::setEye(const Vector3& v)
  {
	  m_eye = v * NamespaceG2D::s_content_scale_factor;
	  m_dirty = true;
  }

  Vector3 Camera::center() const
  {
	  return m_center / NamespaceG2D::s_content_scale_factor;
  }

  void Camera::setCenter(const Vector3& v)
  {
	  m_center = v * NamespaceG2D::s_content_scale_factor;
	  m_dirty = true;
  }

  Vector3 Camera::up() const
  {
	  return m_up / NamespaceG2D::s_content_scale_factor;
  }

  void Camera::setUp(const Vector3& v)
  {
	  m_up = v * NamespaceG2D::s_content_scale_factor;
	  m_dirty = true;
  }
} //namespace Aphid
