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
#include <runtime/AJObject.h>
#include "Node.h"

#include "Camera.h"

#include "G2DMacros.h"

#include "glu.h"

#include "OAUtil.h"

#include "Scheduler.h"

#include "AJNode.h"
#include "AJCamera.h"
#include "AJOABinding.h"
#include "OAGlobalObject.h"
#include "Diagnostic.h"

namespace Aphid {
	using namespace AJ;
	
#if CC_COCOSNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL (NSInteger)
#endif
	
  ///-------------------------------------------------------------------------------------------------------------------
  Node::Node()
  : m_isRunning(false)
  , m_skew(Vector2Zero)
  , m_rotation(0.0f)
  , m_scale(Vector2Identity)
  , m_position(PointZero)
  , m_positionInPixels(PointZero)
  , m_anchor(PointZero)
  , m_anchorInPixels(PointZero)
  , m_contentSize(SizeZero)
  , m_contentSizeInPixels(SizeZero)
  , m_isRelativeAnchor(true)
  , m_isTransformDirty(true)
  , m_isInverseDirty(true)
#if CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
	, m_isTransformGLDirty(true)
#endif
  , m_vertexZ(0)
  , m_grid(0)
  , m_visible(true)
  , m_tag(NodeTagInvalid)
  , m_zOrder(0)
  , m_camera(0)
  , m_children(0)
  , m_parent(0)
  {
	  LEAK_DETECT_INC("Node");
  }
	
  Node::~Node()
  {
	  if (m_children)
		  m_children->setAllParent(0);
	  LEAK_DETECT_DEC("Node");
  }
	
  void Node::cleanup()
  {
	  stopAllActions();
	  unscheduleAllCallbacks();
		unscheduleUpdate();
		
	  if (m_children)
		  m_children->allCleanup();
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  //Getters and setters
	
  void Node::setSkew(const Aphid::Vector2 &v)
  {
	  m_skew = v;
	  setTransformDirty();
  }
	
  void Node::setSkew(float x, float y)
  {
	  m_skew.x = x;
	  m_skew.y = y;
	  setTransformDirty();
  }
	
  void Node::setSkewX(float v)
  {
	  m_skew.x = v;
	  setTransformDirty();
  }
	
  void Node::setSkewY(float v)
  {
	  m_skew.y = v;
	  setTransformDirty();
  }
	
  void Node::setRotation(float v)
  {
	  m_rotation = v;
	  setTransformDirty();
  }
	
  void Node::setScale(const Vector2& v)
  {
	  m_scale = v;
	  setTransformDirty();
  }
	
  void Node::setScaleX(float x)
  {
	  m_scale.x = x;
	  setTransformDirty();
  }
	
  void Node::setScaleY(float y)
  {
	  m_scale.y = y;
	  setTransformDirty();
  }
	
  void Node::setPosition(const Point& p)
  {
	  m_position = p;
		m_positionInPixels = toPixels(m_position);
	  setTransformDirty();
  }
	
  void Node::setPositionInPixels(const Point& p)
  {
	  m_positionInPixels = p;
		m_position = toPoints(m_positionInPixels);
	  setTransformDirty();
  }
	
  void Node::setRelativeAnchor(bool b)
  {
	  if (m_isRelativeAnchor != b) {
		  m_isRelativeAnchor = b;
		  setTransformDirty();
	  }
  }
	
  void Node::setAnchor(const Point& p)
  {
	  if (!PointEqual(p, m_anchor)) {
		  m_anchor = p;
		  m_anchorInPixels = PointMake(m_contentSizeInPixels.width * m_anchor.x, m_contentSizeInPixels.height * m_anchor.y);
		  setTransformDirty();
	  }
  }
	
  void Node::setContentSize(const Size& s)
  {
	  if (!SizeEqual(s, m_contentSize)) {
			setContentSizeInPixels(toPixels(s));
	  }
  }
	
  void Node::setContentSizeInPixels(const Size& s)
  {
	  if (!SizeEqual(s, m_contentSizeInPixels)) {
		  m_contentSizeInPixels = s;
		  m_contentSize = toPoints(m_contentSizeInPixels);
			
		  m_anchorInPixels = PointMake(m_contentSizeInPixels.width * m_anchor.x, m_contentSizeInPixels.height * m_anchor.y);
		  setTransformDirty();
	  }
  }
	
  Rect Node::boundingBox()
  {
	  Rect r = boundingBoxInPixels();
		return toPoints(r);
  }
	
  Rect Node::boundingBoxInPixels()
  {
	  Rect rect = RectMake(0, 0, m_contentSizeInPixels.width, m_contentSizeInPixels.height);
	  return RectApplyAffineTransform(rect, nodeToParentTransform());
  }
	
  float Node::vertexZ() const
  {
	  return m_vertexZ / G2D_CONTENT_SCALE_FACTOR();
  }
	
  void Node::setVertexZ(float v)
  {
	  m_vertexZ = v * G2D_CONTENT_SCALE_FACTOR();
  }
	
  NodeList *Node::children()
  {
	  if (!m_children)
		  lazyCreateChildren();
	  return m_children.get();
  }
	
  size_t Node::childrenSize() const
  {
	  return m_children ? m_children->size() : 0;
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Composition
	
  Camera *Node::camera() const
  {
	  if (!m_camera)
		  m_camera = Camera::create();
		
	  return m_camera.get();
  }
	
  Node *Node::getChildByTag(int tag)
  {
	  if (m_children)
		  return m_children->getNodeByTag(tag);
		
	  return 0;
  }
	
  void Node::addChild(NonNullPassRefPtr<Node> prpNode)
  {
		RefPtr<Node> node = prpNode;
	  addChild(node, node->zOrder(), node->tag());
  }
	
  void Node::addChild(NonNullPassRefPtr<Node> prpNode, int z)
  {
		RefPtr<Node> node = prpNode;
	  addChild(node, z, node->tag());
  }
	
  void Node::addChild(NonNullPassRefPtr<Node> prpNode, int z, int tag)
  {
	  RefPtr<Node> child = prpNode;
		
		if (child.get() == this) {
			Diagnostic::error("The subnode is the same instance as the parent node?!");
			return;
		}
		
		if (child->parent()) {
			if (child->parent() == this) {
				child->setTag(tag);
				this->reorderChild(child, z);
				return;
			} else {
				child->removeFromParentAndCleanup(false);
				if (OAGlobalObject::s_develop_mode)
					Diagnostic::warn(formatUString("Adding a subnode which already has a parent node."));
			}
		}
		
	  ASSERT(!child->parent());
	  ASSERT(child.get() != this);
		
	  if (!m_children)
		  lazyCreateChildren();
		
	  m_children->insert(child, z);
		
	  child->setTag(tag);
	  child->setParent(this);
		
	  if (m_isRunning) {
		  child->onEnter();
		  child->onEnterTransitionDidFinish();
	  }
  }
	
  void Node::removeFromParentAndCleanup(bool cleanup)
  {
	  if (m_parent)
		  m_parent->removeChild(this, cleanup);
		else if (cleanup) //impl is different from CC here
			this->cleanup();
  }
	
  void Node::removeChild(Node *child, bool cleanup)
  {
	  if (child && m_children && m_children->contains(child)) {
		  detachChild(child, cleanup);
	  }
  }
	
  void Node::removeChildAt(size_t index, bool cleanup)
  {
	  if (m_children)
		  removeChild(m_children->at(index), cleanup);
  }
	
  void Node::removeChildByTag(int tag, bool cleanup)
  {
	  Node *child = getChildByTag(tag);
	  if (child)
		  removeChild(child, cleanup);
  }
	
  void Node::removeAllChildren(bool cleanup)
  {
	  if (m_children) {
		  NodeVector& internal = m_children->internalStorage();
		  for (NodeVector::const_iterator iter = internal.begin(); iter != internal.end(); ++iter) {
			  Node *child = iter->get();
			  if (m_isRunning)
				  child->onExit();
			  if (cleanup)
				  child->cleanup();
			  child->setParent(0);
		  }
			
		  m_children->removeAll();
	  }
  }
	
  void Node::reorderChild(PassRefPtr<Node> prpNode, int z)
  {
	  ASSERT(prpNode);
		
	  RefPtr<Node> node = prpNode;
		
	  if (m_children) {
		  m_children->remove(node.get());
		  m_children->insert(node, z);
	  } else {
		  OA_TODO(); //different impl from CC
		  addChild(node, z);
	  }
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Draw
	
  void Node::visit()
  {
	  // quick return if not visible
	  if (!m_visible)
		  return;
		
	  glPushMatrix();
		
	  if (m_grid && m_grid->active()) {
		  m_grid->beforeDraw();
		  transformAncestors();
	  }
		
	  transform();
		
	  if (m_children) {
		  NodeVector& vector = m_children->internalStorage();
		  size_t i = 0;
		  size_t size = vector.size();
			
		  // draw children zOrder < 0
		  for (; i < size; i++) {
			  Node *child = vector.at(i).get();
			  if (child->zOrder() < 0)
				  child->visit();
			  else
				  break;
		  }
			
		  draw();
			
		  // draw children zOrder >= 0
		  for (; i < size; i++) {
			  Node *child = vector.at(i).get();
			  child->visit();
		  }
	  } else
		  draw();
		
	  if (m_grid && m_grid->active())
		  m_grid->afterDraw(this);
		
	  glPopMatrix();
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Transformations
	
  void Node::transformAncestors()
  {
	  if (m_parent) {
		  m_parent->transformAncestors();
		  m_parent->transform();
	  }
  }
	
  void Node::transform()
  {
#if CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
		// BEGIN alternative -- using cached transform
		//
		if( isTransformGLDirty_ ) {
			AffineTransform t = [self nodeToParentTransform];
			CGAffineToGL(&t, transformGL_);
			isTransformGLDirty_ = NO;
		}
		
		glMultMatrixf(transformGL_);
		if( m_vertexZ )
			glTranslatef(0, 0, m_vertexZ);
		
		// TODO: Expensive calls. Camera should be integrated into the cached affine matrix
		if ( camera_ && !(grid_ && grid_.active) )
		{
			BOOL translate = (m_anchorInPixels.x != 0.0f || m_anchorInPixels.y != 0.0f);
			
			if( translate )
				ccglTranslate(RENDER_IN_SUBPIXEL(m_anchorInPixels.x), RENDER_IN_SUBPIXEL(m_anchorInPixels.y), 0);
			
			[camera_ locate];
			
			if( translate )
				ccglTranslate(RENDER_IN_SUBPIXEL(-m_anchorInPixels.x), RENDER_IN_SUBPIXEL(-m_anchorInPixels.y), 0);
		}
		
		
		// END alternative
		
#else
	  // translate
	  if (m_isRelativeAnchor && (m_anchorInPixels.x != 0 || m_anchorInPixels.y != 0))
		  glTranslatef(RENDER_IN_SUBPIXEL(-m_anchorInPixels.x), RENDER_IN_SUBPIXEL(-m_anchorInPixels.y), 0);
		
	  if (m_anchorInPixels.x != 0 || m_anchorInPixels.y != 0)
		  glTranslatef(RENDER_IN_SUBPIXEL(m_positionInPixels.x + m_anchorInPixels.x), RENDER_IN_SUBPIXEL(m_positionInPixels.y + m_anchorInPixels.y), m_vertexZ);
	  else if (m_positionInPixels.x != 0 || m_positionInPixels.y != 0 || m_vertexZ != 0)
		  glTranslatef(RENDER_IN_SUBPIXEL(m_positionInPixels.x), RENDER_IN_SUBPIXEL(m_positionInPixels.y), m_vertexZ);
		
	  // rotate
	  if (m_rotation != 0.0f)
		  glRotatef(-m_rotation, 0.0f, 0.0f, 1.0f);
		
	  // skew
	  if ((m_skew.x != 0.0f) || (m_skew.y != 0.0f)) {
		  AffineTransform skewMatrix = AffineTransformMake( 1.0f, tanf(CC_DEGREES_TO_RADIANS(m_skew.y)), tanf(CC_DEGREES_TO_RADIANS(m_skew.x)), 1.0f, 0.0f, 0.0f );
		  GLfloat glMatrix[16];
		  AffineToGL(&skewMatrix, glMatrix);
		  glMultMatrixf(glMatrix);
	  }
		
	  // scale
	  if (m_scale.x != 1.0f || m_scale.y != 1.0f)
		  glScalef(m_scale.x, m_scale.y, 1.0f);
		
	  if (m_camera && !(m_grid && m_grid->active()))
		  m_camera->locate();
		
	  // restore and re-position point
	  if (m_anchorInPixels.x != 0.0f || m_anchorInPixels.y != 0.0f)
		  glTranslatef(RENDER_IN_SUBPIXEL(-m_anchorInPixels.x), RENDER_IN_SUBPIXEL(-m_anchorInPixels.y), 0);
#endif
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // Scene management
	
  void Node::onEnter()
  {
	  if (m_children)
		  m_children->allOnEnter();
	  resumeSchedulerAndActions();
	  m_isRunning = true;
		
  }
	
  void Node::onEnterTransitionDidFinish()
  {
	  if (m_children)
		  m_children->allOnEnterTransitionDidFinish();
  }
	
  void Node::onExit()
  {
	  pauseSchedulerAndActions();
	  m_isRunning = false;
	  if (m_children)
		  m_children->allOnExit();
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // actions
  Action *Node::runAction(PassRefPtr<Action> prpAction)
  {
	  RefPtr<Action> action = prpAction;
	  ActionManager::sharedManager()->addAction(action, this, !m_isRunning);
	  return action.get();
  }
	
  void Node::stopAllActions()
  {
	  ActionManager::sharedManager()->removeActions(this);
  }
	
  void Node::stopAction(Action *action)
  {
	  ActionManager::sharedManager()->removeAction(action);
  }
	
  void Node::stopActionByTag(int tag)
  {
	  ActionManager::sharedManager()->removeActionByTag(tag, this);
  }
	
  Action *Node::getActionByTag(int tag)
  {
	  return ActionManager::sharedManager()->getActionByTag(tag, this);
  }
	
  int Node::numberOfRunningActions()
  {
	  return ActionManager::sharedManager()->runningActionsCount(this);
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // scheduler
  void Node::scheduleUpdate(int priority)
  {
	  Scheduler::sharedScheduler()->scheduleUpdate(this, priority, !m_isRunning);
  }
	
  void Node::unscheduleUpdate()
  {
	  Scheduler::sharedScheduler()->unscheduleUpdate(this);
  }
	
  void Node::schedule(TimerFunction callback, TimeSec interval, int repeat, TimeSec delay)
  {
	  Scheduler::sharedScheduler()->scheduleCallback(this, callback, interval, !m_isRunning, repeat, delay);
  }
	
	void Node::schedule(AJ::AJObject* callback, TimeSec interval, int repeat, TimeSec delay)
  {
	  Scheduler::sharedScheduler()->scheduleAJCallback(this, callback, interval, !m_isRunning, repeat, delay);
  }
	
  void Node::unschedule(TimerFunction callback)
  {
	  Scheduler::sharedScheduler()->unscheduleCallback(this, callback);
  }
	
	void Node::unschedule(AJ::AJObject *callback)
	{
		Scheduler::sharedScheduler()->unscheduleCallback(this, callback);
	}
	
  void Node::unscheduleAllCallbacks()
  {
	  Scheduler::sharedScheduler()->unscheduleCallbacks(this);
  }
	
  void Node::resumeSchedulerAndActions()
  {
	  Scheduler::sharedScheduler()->resume(this);
	  ActionManager::sharedManager()->resume(this);
  }
	
  void Node::pauseSchedulerAndActions()
  {
	  Scheduler::sharedScheduler()->pause(this);
	  ActionManager::sharedManager()->pause(this);
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // transform
  AffineTransform Node::nodeToParentTransform()
  {
	  if (m_isTransformDirty) {
			
		  m_transform = AffineTransformIdentity;
			
		  if (!m_isRelativeAnchor && !PointEqual(m_anchorInPixels, PointZero))
			  m_transform = AffineTransformTranslate(m_transform, m_anchorInPixels.x, m_anchorInPixels.y);
			
		  if (!PointEqual(m_positionInPixels, PointZero))
			  m_transform = AffineTransformTranslate(m_transform, m_positionInPixels.x, m_positionInPixels.y);
			
		  if (m_rotation != 0)
			  m_transform = AffineTransformRotate(m_transform, -CC_DEGREES_TO_RADIANS(m_rotation));
			
		  if (m_skew.x != 0 || m_skew.y != 0) {
			  // create a skewed coordinate system
			  AffineTransform skew = AffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_skew.y)), tanf(CC_DEGREES_TO_RADIANS(m_skew.x)), 1.0f, 0.0f, 0.0f);
			  // apply the skew to the transform
			  m_transform = AffineTransformConcat(skew, m_transform);
		  }
			
		  if (!(m_scale.x == 1 && m_scale.y == 1))
			  m_transform = AffineTransformScale(m_transform, m_scale.x, m_scale.y);
			
		  if (!PointEqual(m_anchorInPixels, PointZero))
			  m_transform = AffineTransformTranslate(m_transform, -m_anchorInPixels.x, -m_anchorInPixels.y);
			
		  m_isTransformDirty = false;
	  }
		
	  return m_transform;
  }
	
  AffineTransform Node::parentToNodeTransform()
  {
	  if (m_isInverseDirty) {
		  m_inverse = AffineTransformInvert(nodeToParentTransform());
		  m_isInverseDirty = false;
	  }
		
	  return m_inverse;
  }
	
  AffineTransform Node::nodeToWorldTransform()
  {
	  AffineTransform t = nodeToParentTransform();
		
	  for (Node *p = m_parent; p != nil; p = p->parent())
		  t = AffineTransformConcat(t, p->nodeToParentTransform());
		
	  return t;
  }
	
  AffineTransform Node::worldToNodeTransform()
  {
	  return AffineTransformInvert(nodeToWorldTransform());
  }
	
	Point Node::convertParentToNodeSpace(const Point& parentPoint)
  {
	  Point ret;
	  if (G2D_CONTENT_SCALE_FACTOR() == 1)
		  ret = PointApplyAffineTransform(parentPoint, parentToNodeTransform());
	  else {
		  ret = toPixels(parentPoint);
		  ret = PointApplyAffineTransform(ret, parentToNodeTransform());
		  ret = toPoints(ret);
	  }
		
	  return ret;
  }
	
  Point Node::convertToNodeSpace(const Point& worldPoint)
  {
	  Point ret;
	  if (G2D_CONTENT_SCALE_FACTOR() == 1)
		  ret = PointApplyAffineTransform(worldPoint, worldToNodeTransform());
	  else {
		  ret = toPixels(worldPoint);
		  ret = PointApplyAffineTransform(ret, worldToNodeTransform());
		  ret = toPoints(ret);
	  }
		
	  return ret;
  }
	
  Point Node::convertToWorldSpace(const Point& nodePoint)
  {
	  Point ret;
	  if (G2D_CONTENT_SCALE_FACTOR() == 1)
		  ret = PointApplyAffineTransform(nodePoint, nodeToWorldTransform());
	  else {
		  ret = toPixels(nodePoint);
		  ret = PointApplyAffineTransform(ret, nodeToWorldTransform());
		  ret = toPoints(ret);
	  }
		
	  return ret;
  }
	
  Point Node::convertToNodeSpaceAR(const Point& worldPoint)
  {
	  Point nodePoint = convertToNodeSpace(worldPoint);
	  Point anchorInPoints = toPoints(m_anchorInPixels);
		
	  return g2d_sub(nodePoint, anchorInPoints);
  }
	
  Point Node::convertToWorldSpaceAR(const Point& nodePoint)
  {
	  Point anchorInPoints = toPoints(m_anchorInPixels);
		
	  Point p = g2d_add(nodePoint, anchorInPoints);
	  return convertToWorldSpace(p);
  }
	
  Point Node::convertToWindowSpace(const Point& nodePoint)
  {
	  Point worldPoint = convertToWorldSpace(nodePoint);
	  return Director::sharedDirector()->convertToUI(worldPoint);
  }
	
  //--------------------------------------------------------------------------------------------------------------------
  // InheritanceWrapper
  const AJ::ClassInfo *Node::wrapperClassInfo() const
  {
	  return &AJNode::s_info;
  }
	
  AJ::AJObject* Node::createWrapper(AJ::ExecState *exec, AJOAGlobalObject *globalObject)
  {
	  return createAJOAWrapper<AJNode>(exec, globalObject, this);
  }
	
	void Node::markObjects(AJ::MarkStack& markStack, unsigned markID)
	{
		if (isMarked(markID))
			return;
		
		InheritanceWrapper::markObjects(markStack, markID);
		
		markTimerTargetCallbacks(markStack, markID);
		markEventListeners(markStack, markID);
		
		if (m_children)
			m_children->markObjects(markStack, markID);
		
		if (m_parent && !m_parent->isMarked(markID))
			m_parent->markObjects(markStack, markID);
		
		if (m_camera)
			m_camera->markObjects(markStack, markID);
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	// TouchEventTarget
	//TODO: consider clip, point-2-pixels, etc
	bool Node::pointInside(const Point& p, PlatformTouchEvent* event)
	{
		Rect bound = {PointZero, contentSize()};
		if (!RectEqual(bound, RectZero))
			return RectContainsPoint(bound, p);
		else {
			if (m_children)
				return m_children->pointInside(p, event);
		}
		
		return false;
	}
	
	Node* Node::hitTest(const Point &p, Aphid::PlatformTouchEvent *event)
	{
		if (!isUserInteractionEnabled() || !visible() || !pointInside(p, event))
			return 0;
		
		if (m_children) {
			Node* hit = m_children->hitTest(p, event);
			if (hit)
				return hit;
		}
		
		return isTouchEnabled() ? this : 0;
	}
	
	void Node::handleTouchEvent(Aphid::EventFlag flag, const PlatformTouchVector &touches, Aphid::PlatformTouchEvent *event)
	{
		if (isTouchEnabled())
#if OA_DEV
			switch (flag) {
				case EventFlagTouchStart:
					oa_debug("%p touchesStart, event %p", this, event);
					break;
				case EventFlagTouchMove:
					oa_debug("%p touchesMove, event %p", this, event);
					break;
				case EventFlagTouchEnd:
					oa_debug("%p touchesEnd, event %p", this, event);
					break;
				case EventFlagTouchCancel:
					oa_debug("%p touchesCancel, event %p", this, event);
					break;
				default:
					oa_error("unknown event flag: %d", flag);
			}
#endif
		TouchEventTarget::handleTouchEvent(flag, touches, event);
	}
	
  //----------------------------------------------------------------------------------------------------------
  // Private methods
	
  void Node::lazyCreateChildren()
  {
	  ASSERT(!m_children);
	  m_children = NodeList::create(4);
  }
	
  void Node::detachChild(Node* child, bool cleanup)
  {
	  ASSERT(child);
	  ASSERT(m_children);
		
	  if (m_isRunning)
		  child->onExit();
		
	  if (cleanup)
		  child->cleanup();
		
	  child->setParent(0);
		
	  m_children->remove(child);
  }
	
} //namespace Aphid