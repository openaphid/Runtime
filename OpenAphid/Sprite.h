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

#ifndef OpenAphid_Sprite_h
#define OpenAphid_Sprite_h

#include "Node.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "ccTypes.h"
#include "Texture2D.h"
#include "G2DProtocols.h"
#include "SpriteFrame.h"

namespace Aphid {
	
	class TextureAtlas;
	class SpriteBatchNode;
	
	#define G2DSpriteIndexNotInitialized 0xffffffff
	
	enum G2DHonorParentTransform{
		//! Translate with it's parent
		G2D_HONOR_PARENT_TRANSFORM_TRANSLATE =  1 << 0,
		//! Rotate with it's parent
		G2D_HONOR_PARENT_TRANSFORM_ROTATE	=  1 << 1,
		//! Scale with it's parent
		G2D_HONOR_PARENT_TRANSFORM_SCALE		=  1 << 2,
		//! Skew with it's parent
		G2D_HONOR_PARENT_TRANSFORM_SKEW		=  1 << 3,
		
		//! All possible transformation enabled. Default value.
		G2D_HONOR_PARENT_TRANSFORM_ALL		=  G2D_HONOR_PARENT_TRANSFORM_TRANSLATE | G2D_HONOR_PARENT_TRANSFORM_ROTATE | G2D_HONOR_PARENT_TRANSFORM_SCALE | G2D_HONOR_PARENT_TRANSFORM_SKEW,
	};
	
	struct transformValues_ {
		Point pos;		// position x and y
		Vector2	scale;		// scale x and y
		float	rotation;
		Vector2 skew;		// skew x and y
		Point ap;			// anchor point in pixels
		bool	visible;
	};

	
	///-------------------------------------------------------------------------------------------------------------------	
	class Sprite : public Node, public RGBAProtocol, public TextureProtocol {
		typedef Node Base;
		
		friend class NodeList;
	public:
		Sprite();
		virtual ~Sprite();

#if OA_NATIVE_BENCHMARK
		// for Native benchmark
		virtual void update(float dt);
		//TODO: remove 
		float _vx, _vy;
		//TODO
		virtual void handleTouchEvent(Aphid::EventFlag flag, const PlatformTouchVector &touches, Aphid::PlatformTouchEvent *event);
#endif
		
		
		static PassRefPtr<Sprite> create()
		{
			return adoptRef(new Sprite());
		}
		
		static bool s_debug_draw_outline;
		static bool s_debug_draw_texture_outline;
		
		static PassRefPtr<Sprite> create(PassRefPtr<Texture2D> texture);
		static PassRefPtr<Sprite> create(PassRefPtr<Texture2D> texture, const Rect& rect);
		static PassRefPtr<Sprite> createWithFrame(PassRefPtr<SpriteFrame> frame);
		
		Rect textureRect() const {return m_rect;}
		void setTextureRect(const Rect& r);
		void setTextureRectInPixels(const Rect& rect, bool rotated, const Size& untrimmedSize);
		Point offsetPositionInPixels() const {return m_offsetPositionInPixels;}
		
		bool flipX() const {return m_flipX;}
		void setFlipX(bool b);
		
		bool flipY() const {return m_flipY;}
		void setFlipY(bool b);
		
		ccV3F_C4B_T2F_Quad quad() const {return m_quad;}
		
		virtual void draw();
		
		//Overrides Node
	  virtual void addChild(NonNullPassRefPtr<Aphid::Node> prpChild, int z, int tag);
	  virtual void reorderChild(PassRefPtr<Node> prpChild, int z);
	  virtual void removeChild(Node* node, bool cleanup);
	  virtual void removeAllChildren(bool cleanup);

		virtual void setPosition(const Point& p);
		virtual void setPositionInPixels(const Point& p);
		virtual void setRotation(float v);
		virtual void setSkew(const Vector2& v);
		virtual void setSkew(float x, float y);
		virtual void setSkewX(float x);
		virtual void setSkewY(float y);
		virtual void setScale(const Vector2& v);
		virtual void setScaleX(float x);
		virtual void setScaleY(float y);
		virtual void setVertexZ(float v);
		virtual void setAnchor(const Point& p);
		virtual void setRelativeAnchor(bool b);
		virtual void setVisible(bool v);
		virtual void setContentSizeInPixels(const Size& s);
		
		//RGBAProtocol
		virtual ccColor3B color() const;
		virtual void setColor(const ccColor3B& c);
		virtual GLubyte opacity() const {return m_opacity;}
		virtual void setOpacity(GLubyte opacity);
		virtual void setOpacityModifyRGB(bool modify);
		virtual bool doesOpacityModifyRGB() {return m_opacityModifyRGB;}
		
		//TextureProtocol
		virtual Texture2D* texture() const {return m_texture.get();}
		virtual void setTexture(PassRefPtr<Texture2D> texture);
		
		//Overrides Node
		virtual RGBAProtocol* toRGBAProtocol() {return this;}
		virtual TextureProtocol* toTextureProtocol() {return this;}
		virtual Sprite* toSprite() {return this;}
		
		//Overrides
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		//Frames & Animation
		void setDisplayFrame(PassRefPtr<SpriteFrame> frame);
		bool isFrameDisplayed(SpriteFrame* frame);
		PassRefPtr<SpriteFrame> displayedFrame();
		
		//BatchNode
		void updateTransform();
		void getTransformValues(struct transformValues_* tv);
		void useSelfRender();
		void useBatchNode(SpriteBatchNode* batchNode);
	  void setDirtyRecursively(bool b);
		void setDirty(bool b) {m_dirty = b;}
		
		size_t atlasIndex() const {return m_atlasIndex;}
		void setAtlasIndex(size_t index) {m_atlasIndex = index;}
		bool usesBatchMode() const {return m_usesBatchNode;}
		void setUsesBatchMode(bool v) {m_usesBatchNode = v;}
		TextureAtlas* textureAtlas() const {return m_textureAtlas;}
		void setTextureAtlas(TextureAtlas* atlas) {m_textureAtlas = atlas;}
		SpriteBatchNode* batchNode() const {return m_batchNode;}
		void setBatchNode(SpriteBatchNode* node) {m_batchNode = node;}
		G2DHonorParentTransform honorParentTransform() const {return m_honorParentTransform;}
		void setHonorParentTransform(G2DHonorParentTransform transform) {m_honorParentTransform = transform;}
		
	protected:
		void applyDirtyRecursively();
		
		TextureAtlas* m_textureAtlas;
		size_t m_atlasIndex;
		SpriteBatchNode* m_batchNode;
		G2DHonorParentTransform m_honorParentTransform;
		bool m_dirty;
		bool m_recursiveDirty;
		bool m_hasChildren;
		
		ccBlendFunc m_blendFunc;		
		RefPtr<Texture2D> m_texture;
		
		bool m_usesBatchNode;
		
		Rect m_rect;
		Rect m_rectInPixels;
		bool m_rectRotated;
		
		Point m_offsetPositionInPixels;
		Point m_unflippedOffsetPositionFromCenter;
		
		ccV3F_C4B_T2F_Quad m_quad;
		
		GLubyte m_opacity;
		ccColor3B m_color;
		ccColor3B m_colorUnmodified;
		bool m_opacityModifyRGB;
		
		bool m_flipX;
		bool m_flipY;
		
	private:
		
		void updateTextureCoords(const Rect& rect);
		void updateBlendFunc();
		void updateColor();
		void setDirtyRecursively();
		
	}; //class Sprite
} //namespace Aphid

#endif
