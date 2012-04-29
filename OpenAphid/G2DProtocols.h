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

#ifndef OpenAphid_G2DProtocols_h
#define OpenAphid_G2DProtocols_h

#include "ccTypes.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "Color.h"

namespace Aphid {
	class Texture2D;
	
	class RGBAProtocol {
	public:
		virtual ~RGBAProtocol(){}
		
		virtual ccColor3B color() const = 0;
		virtual void setColor(const ccColor3B& c) = 0;
		virtual GLubyte opacity() const = 0;
		virtual void setOpacity(GLubyte opacity) = 0;
		
		virtual RGBA rgba() const {return Color(color(), opacity()).rgba();} //TODO: improve it later
		virtual void setRGBA(RGBA color) {
			setColor(toCCColor3B(color));
			setOpacity(Color::alpha(color));
		}
		
		//Optional
		virtual void setOpacityModifyRGB(bool modify)
		{
			UNUSED_PARAM(modify);
		}
		virtual bool doesOpacityModifyRGB() {return false;}
	};
	
	class BlendProtocol {
	public:
		virtual ~BlendProtocol(){}
		
		virtual void setBlendFunc(ccBlendFunc func) = 0;
		virtual ccBlendFunc blendFunc() = 0;
	};
	
	class TextureProtocol {
	public:
		virtual ~TextureProtocol(){}
		
		virtual Texture2D* texture() const = 0;
		virtual void setTexture(PassRefPtr<Texture2D> texture) = 0;
	};
	
	class LabelProtocol {
	public:
		virtual ~LabelProtocol() {}
		virtual void setLabelString(const AJ::UString& string) = 0;
		virtual AJ::UString labelString() const = 0;
	};
	
	class ProjectionProtocol {
	public:
		virtual ~ProjectionProtocol() {}
		virtual void updateProjection() = 0;
	};
}

#endif
