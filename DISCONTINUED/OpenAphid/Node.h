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

#ifndef OpenAphid_Node_h
#define OpenAphid_Node_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>
#include "NodeList.h"
#include "InheritanceWrapper.h"
#include "OAData.h"
#include "G2DConfig.h"
#include "G2DProtocols.h"
#include "AJCallback.h"
#include "Action.h"
#include "Grid.h"

#include "Touch.h"

#if PLATFORM(IPHONE)
#ifdef __OBJC__
@class UITouch;
#else
class UITouch;
#endif
#endif

namespace AJ {
	class MarkStack;
}

namespace Aphid {
	class AJNode;
	class Camera;
	class Node;
	class Sprite;
	
	enum {
		NodeTagInvalid = -1,
	};
	
	class Node : public RefCounted<Node>, public AJTimerTarget, public InheritanceWrapper, public TouchEventTarget {
		friend class NodeList;
		
	public:
		virtual ~Node();
		
		static PassRefPtr<Node> create() 
		{
			return adoptRef(new Node());
		}
		
		//TODO: some of them are overrided in Sprite, set to virtual
		
		int tag() const {return m_tag;}
		void setTag(int v) {m_tag = v;}
		
		Node* parent() const {return m_parent;}
		void setParent(Node* v) {m_parent = v;}
		
		int zOrder() const {return m_zOrder;}
		
		float vertexZ() const;
		virtual void setVertexZ(float v);
		
		Vector2 skew() const {return m_skew;}
		virtual void setSkew(const Vector2& v);
		virtual void setSkew(float x, float y);
		
		float skewX() const {return m_skew.x;}
		virtual void setSkewX(float v);
		
		float skewY() const {return m_skew.y;}
		virtual void setSkewY(float v);
		
		float rotation() const {return m_rotation;}
		virtual void setRotation(float v);
		
		Vector2 scale() const {return m_scale;}
		virtual void setScale(const Vector2& v);
		virtual void setScaleX(float x);
		virtual void setScaleY(float y);
		
		Point position() const {return m_position;}
		virtual void setPosition(const Point& p);
		
		Point positionInPixels() const {return m_positionInPixels;}
		virtual void setPositionInPixels(const Point& p);
		
		Camera* camera() const;
		GridBase* grid() const {return m_grid.get();}
		void setGrid(PassRefPtr<GridBase> grid) {m_grid = grid;}
		
		NodeList* children();
		size_t childrenSize() const;
		
		bool visible() const {return m_visible;}
		virtual void setVisible(bool visible) {m_visible = visible;} //Overrided in Sprite
		
		Point anchor() const {return m_anchor;}
		virtual void setAnchor(const Point& p);
		
		Point anchorInPixels() const {return m_anchorInPixels;}
		
		Size contentSize() const {return m_contentSize;}
		virtual void setContentSize(const Size& s);
		
		Size contentSizeInPixels() const {return m_contentSizeInPixels;}
		virtual void setContentSizeInPixels(const Size& s);
		
		bool isRunning() const {return m_isRunning;}
		
		bool isRelativeAnchor() const {return m_isRelativeAnchor;}
		virtual void setRelativeAnchor(bool b);
		
		
	public:
		//TODO: not all methods should be virtual!
		
		virtual void onEnter();
		virtual void onEnterTransitionDidFinish();
		virtual void onExit();
		virtual void addChild(NonNullPassRefPtr<Node> node);
		virtual void addChild(NonNullPassRefPtr<Node> node, int z);
		virtual void addChild(NonNullPassRefPtr<Node> node, int z, int tag);
		virtual void removeFromParentAndCleanup(bool cleanup);
		virtual void removeChild(Node* node, bool cleanup);
		virtual void removeChildAt(size_t index, bool cleanup);
		virtual void removeChildByTag(int tag, bool cleanup);
		virtual void removeAllChildren(bool cleanUp);
		virtual Node* getChildByTag(int tag);
		virtual void reorderChild(PassRefPtr<Node> node, int z);
		virtual void cleanup();
		virtual void draw() {}
		virtual void visit();
		virtual void transform();
		virtual void transformAncestors();
		
		virtual Rect boundingBox();
		virtual Rect boundingBoxInPixels();
		
		virtual Action* runAction(PassRefPtr<Action> action);
		virtual void stopAllActions();
		virtual void stopAction(Action* action);
		virtual void stopActionByTag(int tag);
		virtual Action* getActionByTag(int tag);
		virtual int numberOfRunningActions();
		
		virtual void scheduleUpdate(int priority=0);
		virtual void unscheduleUpdate();
		virtual void schedule(TimerFunction callback, TimeSec interval=0, int repeat=OA_REPEAT_FOREVER, TimeSec delay=0);
		virtual void schedule(AJ::AJObject* callback, TimeSec interval=0, int repeat=OA_REPEAT_FOREVER, TimeSec delay=0);
		virtual void unschedule(TimerFunction callback);
		virtual void unschedule(AJ::AJObject* callback);
		virtual void unscheduleAllCallbacks();
		virtual void resumeSchedulerAndActions();
		virtual void pauseSchedulerAndActions();
		
		AffineTransform nodeToParentTransform();
		AffineTransform parentToNodeTransform();
		AffineTransform nodeToWorldTransform();
		AffineTransform worldToNodeTransform();
		
		Point convertParentToNodeSpace(const Point& point);
		Point convertToNodeSpace(const Point& worldPoint);
		Point convertToWorldSpace(const Point& nodePoint);
		Point convertToNodeSpaceAR(const Point& worldPoint);
		Point convertToWorldSpaceAR(const Point& nodePoint);
		Point convertToWindowSpace(const Point& nodePoint);
		
#if PLATFORM(IPHONE)
		Point convertTouchToNodeSpace(UITouch* touch);
		Point convertTouchToNodeSpaceAR(UITouch* touch);
#elif PLATFORM(ANDROID)
//#warning TODO: convert touch to node coordination system
#else
#error Unsupported platform
#endif //PLATFORM(IPHONE)
		
		virtual RGBAProtocol* toRGBAProtocol() {return 0;}
		virtual TextureProtocol* toTextureProtocol() {return 0;}
		virtual Sprite* toSprite() {return 0;}
		
		//InheritanceWrapper
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		
		//Overrides AJTimerTarget
		virtual void setAJUpdateCallback(AJ::ExecState* exec, AJ::AJObject* callback)
		{
			AJTimerTarget::setAJUpdateCallback(exec, callback);
			if (!callback)
				unscheduleUpdate();
		}
		
		//Overrides InheritanceWrapper
		virtual void markObjects(AJ::MarkStack &markStack, unsigned markID);
		
		//Overrides TouchEventTarget
		virtual bool pointInside(const Point& p, PlatformTouchEvent* event);
		virtual Node* hitTest(const Point& p, PlatformTouchEvent* event);
		virtual void handleTouchEvent(EventFlag flag, const PlatformTouchVector& touches, PlatformTouchEvent* event);
		virtual EventTarget* nextResponder() {return parent();}
		virtual Node* toNode() {return this;}
		
		using RefCounted<Node>::ref;
		using RefCounted<Node>::deref;
	protected:
		Node();

		void lazyCreateChildren();
		void detachChild(Node* child, bool cleanup);
		
		virtual void refTimerTarget() {ref();}
		virtual void derefTimerTarget() {deref();}
		
		virtual void refEventTarget() {ref();}
		virtual void derefEventTarget() {deref();}
		
		OA_RELEASE_INLINE void setTransformDirty()
		{
			m_isTransformDirty = m_isInverseDirty = true;
#if CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
			m_isTransformGLDirty = true;
#endif
		}
		
	protected:
		float m_rotation;
		
		Vector2 m_scale;
		
		Point m_position;
		Point m_positionInPixels;
		
		Vector2 m_skew;
		
		bool m_visible;
		
		Point m_anchor;
		Point m_anchorInPixels;
		
		bool m_isRelativeAnchor;
		
		Size m_contentSize;
		Size m_contentSizeInPixels;
		
		AffineTransform m_transform, m_inverse;
#if CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
		float m_transformGL[16];
#endif
		
		float m_vertexZ;
		
		mutable RefPtr<Camera> m_camera;
		
		RefPtr<GridBase> m_grid;
		
		int m_zOrder;
		
		Node* m_parent;
		
		int m_tag;
		
		//TODO: userData??
		
		bool m_isRunning;
		
		bool m_isTransformDirty:1;
		bool m_isInverseDirty:1;
		
#if	CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
		bool m_isTransformGLDirty:1;
#endif
		
		mutable RefPtr<NodeList> m_children;
		
	}; // class Node
	
}// namespace Aphid

#endif
