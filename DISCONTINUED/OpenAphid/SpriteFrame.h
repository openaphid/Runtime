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

#ifndef OpenAphid_SpriteFrame_h
#define OpenAphid_SpriteFrame_h

#include "Node.h"
#include "Texture2D.h"

namespace Aphid {
	class AJSpriteFrame;
	
  class SpriteFrame : public ATF::RefCounted<SpriteFrame>, public DirectWrapper<AJSpriteFrame> {
	public:
		virtual ~SpriteFrame();
		
		static PassRefPtr<SpriteFrame> create(PassRefPtr<Texture2D> texture, const Rect& rectInPixels);
		
		static PassRefPtr<SpriteFrame> create(PassRefPtr<Texture2D> texture, const Rect& rectInPixels, bool rotated, const Point& offset, const Size& originalSizeInPixels)
		{
			return adoptRef(new SpriteFrame(texture, rectInPixels, rotated, offset, originalSizeInPixels));
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		//Getters and setters
		Rect rect() const {return m_rect;}
		void setRect(const Rect& rect);
		
		Rect rectInPixels() const {return m_rectInPixels;}
		void setRectInPixels(const Rect& rect);
		
		bool rotated() const {return m_rotated;}
		void setRotated(bool b) {m_rotated = b;}
		
		Point offsetInPixels() const {return m_offsetInPixels;}
		void setOffsetInPixels(const Point& p) {m_offsetInPixels = p;}
		
		Size originalSizeInPixels() const {return m_originalSizeInPixels;}
		void setOriginalSizeInPixels(const Size& s){m_originalSizeInPixels = s;}
		
		Texture2D* texture() const {return m_texture.get();}
		void setTexture(PassRefPtr<Texture2D> texture) {m_texture = texture;}
		
		
	protected:
		SpriteFrame(PassRefPtr<Texture2D> texture, const Rect& rectInPixels, bool rotated, const Point& offset, const Size& originalSizeInPixels);
		
		Rect m_rect;
		Rect m_rectInPixels;
		bool m_rotated;
		Point m_offsetInPixels;
		Size m_originalSizeInPixels;
		RefPtr<Texture2D> m_texture;
	};
}

#endif
