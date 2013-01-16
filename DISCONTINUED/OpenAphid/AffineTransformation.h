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

#ifndef OpenAphid_AffineTransformation_h
#define OpenAphid_AffineTransformation_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "DirectWrapper.h"
#include "OAData.h"

namespace Aphid {
	class AJAffineTransformation;
	
	///-------------------------------------------------------------------------------------------------------------------
	class AffineTransformation : public RefCounted<AffineTransformation>, public DirectWrapper<AJAffineTransformation> {
	public:
		virtual ~AffineTransformation();
		
		static PassRefPtr<AffineTransformation> create(AffineTransform initial=AffineTransformIdentity)
		{
			return adoptRef(new AffineTransformation(initial));
		}
		
		AffineTransform transform() const {return m_transform;}
		void setTransform(AffineTransform v) {m_transform = v;}
		
		float a() const {return m_transform.a;}
		void setA(float a) {m_transform.a = a;}
		
		float b() const {return m_transform.b;}
		void setB(float b) {m_transform.b = b;}
		
		float c() const {return m_transform.c;}
		void setC(float c) {m_transform.c = c;}
		
		float d() const {return m_transform.d;}
		void setD(float d) {m_transform.d = d;}
		
		float tx() const {return m_transform.tx;}
		void setTx(float tx) {m_transform.tx = tx;}
		
		float ty() const {return m_transform.ty;}
		void setTy(float ty) {m_transform.ty = ty;}
		
	private:
		AffineTransformation(AffineTransform initial=AffineTransformIdentity);
		
		AffineTransform m_transform;
		
	}; // class AffineTransformation
	
}// namespace Aphid

#endif
