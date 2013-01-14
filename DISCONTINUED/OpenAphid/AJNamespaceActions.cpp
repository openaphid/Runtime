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

#include "AJNamespaceActions.h"

#include <runtime/AJFunction.h>
#include <runtime/NativeFunctionWrapper.h>
#include <runtime/PrototypeFunction.h>
#include <runtime/Lookup.h>

#include "AJOABinding.h"
#include "OAUtil.h"
#include "ActionInterval.h"
#include "ActionCamera.h"
#include "ActionInstant.h"
#include "ActionGrid.h"
#include "ActionGrid3D.h"
#include "ActionTiledGrid.h"

//referred
#include "AJAction.h"
#include "AJAnimation.h"
#include "AJEaseAction.h"
#include "AJFiniteTimeAction.h"
#include "AJGridBaseAction.h"
#include "AJSize.h"
#include "AJInstantAction.h"
#include "AJIntervalAction.h"
#include "AJNode.h"
#include "AJPoint.h"
#include "AJRect.h"
#include "AJVector2.h"
//end

namespace Aphid {
	using namespace AJ;
	
#include "AJNamespaceActions_table.in.h"
	
	///-------------------------------------------------------------------------------------------------------------------	
	AJNamespaceActions::AJNamespaceActions(NonNullPassRefPtr<Structure> structure, AJOAGlobalObject* globalObject, PassRefPtr<NamespaceActions> impl)
	: Base(structure, globalObject)
	, m_impl(impl)
	{
		AJ_LEAK_DETECT_INC("AJNamespaceActions");
	}
	
	AJNamespaceActions::~AJNamespaceActions()
	{
		//oa_debug("AJNamespaceActions destructed");
		m_impl->clearWrapper(this);
		AJ_LEAK_DETECT_DEC("AJNamespaceActions");
	}
	///-------------------------------------------------------------------------------------------------------------------
	NamespaceActions* toNamespaceActions(AJValue value)
	{
		return value.inherits(&AJNamespaceActions::s_info) ? ajoa_cast<AJNamespaceActions*>(asObject(value))->impl() : 0;
	}
	
	//TODO: set inline?
	AJValue toAJ(ExecState* exec, AJOAGlobalObject* globalObject, NamespaceActions* impl)
	{
		if (!impl)
			return jsNull();
		
		AJObject* wrapper = impl->wrapper();
		if (wrapper)
			return wrapper;
		ASSERT(!impl->isInheritanceWrapper());
		return createAJOAWrapper<AJNamespaceActions>(exec, globalObject, impl);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue ajNamespaceActionsTYPE_MASK_DEFAULT(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		
		return jsNumber(exec, ActionTypeBase);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_FINITE_TIME(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeFiniteTime);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_INTERVAL(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeInterval);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_INSTANT(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeInstant);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_EASE(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeEase);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_GRID(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeGrid);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_GRID3D(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeGrid3D);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_TILED_GRID3D(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeTiledGrid3D);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_CAMERA(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&) 
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeCamera);
	}
	
	AJ::AJValue ajNamespaceActionsTYPE_MASK_CALL_FUNCTION(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&)
	{
		UNUSED_PARAM(thisValue);
		return jsNumber(exec, ActionTypeCallFunction);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRepeatForever(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//repeatForever(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), RepeatForeverAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "RepeatForever");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSpeed(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//speed(IntervalAction action, float speed)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "speed")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float speed = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), SpeedAction::create(action, speed).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Speed");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFollow(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//follow(Node node, [Rect worldBoundary])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "node", Node)
					) {
				Node* node = ajoa_cast<AJNode*>(asObject(args.at(0)))->impl();
				AJRect* worldBoundary = optObjectArg<AJRect>(exec, args, 1, 0);
				if (worldBoundary)
					return toAJ(exec, thisObject->globalObject(), FollowAction::create(node, worldBoundary->data()).get());
				else 
					return toAJ(exec, thisObject->globalObject(), FollowAction::create(node).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Follow");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSequence(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//sequence(FiniteTimeAction action0, FiniteTimeAction action1,...)
		if (args.size() >= 2) {
			size_t size = args.size();
			bool valid = true;
			FiniteTimeActionVector actions;
			for (size_t i = 0; i < size; i++) {
				AJFiniteTimeAction* action = optObjectArg<AJFiniteTimeAction>(exec, args, i);
				if (action)
					actions.append(action->impl());
				else {
					valid = false;
					OA_ERROR_INVALID_ARGUMENT(exec, formatUString("parameter %d", i).ascii(), "FiniteTimeAction");
					break;
				}
			}
			if (valid)
				return toAJ(exec, thisObject->globalObject(), SequenceAction::create(actions).get());
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Sequence");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRepeat(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//repeat(FiniteTimeAction action, int times)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", FiniteTimeAction)
					&& ensureIntArg(exec, args, 1, "times")
					) {
				FiniteTimeAction* action = ajoa_cast<AJFiniteTimeAction*>(asObject(args.at(0)))->impl();
				int times = args.at(1).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), RepeatAction::create(action, times).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Repeat");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSpawn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//spawn(FiniteTimeAction action0, FiniteTimeAction action1,...)
		if (args.size() >= 2) {
			size_t size = args.size();
			bool valid = true;
			FiniteTimeActionVector actions;
			for (size_t i = 0; i < size; i++) {
				AJFiniteTimeAction* action = optObjectArg<AJFiniteTimeAction>(exec, args, i);
				if (action)
					actions.append(action->impl());
				else {
					valid = false;
					OA_ERROR_INVALID_ARGUMENT(exec, formatUString("parameter %d", i).ascii(), "FiniteTimeAction");
					break;
				}
			}
			if (valid)
				return toAJ(exec, thisObject->globalObject(), SpawnAction::create(actions).get());
		}		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Spawn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRotateTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//rotateTo(float duration, float angle)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "angle")
					) {
				float duration = args.at(0).toFloat(exec);
				float angle = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), RotateToAction::create(duration, angle).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "RotateTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRotateBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//rotateBy(float duration, float deltaAngle)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "deltaAngle")
					) {
				float duration = args.at(0).toFloat(exec);
				float deltaAngle = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), RotateByAction::create(duration, deltaAngle).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "RotateBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionMoveTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//moveTo(float duration, Point position)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "position", Point)
					) {
				float duration = args.at(0).toFloat(exec);
				Point position = ajoa_cast<AJPoint*>(asObject(args.at(1)))->data();
				return toAJ(exec, thisObject->globalObject(), MoveToAction::create(duration, position).get());
			}
		}
		
		exec->clearException();
		//moveTo(float duration, float x, float y)
		if (args.size() >= 3) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "x")
					&& ensureFloatArg(exec, args, 2, "y")
					) {
				float duration = args.at(0).toFloat(exec);
				float x = args.at(1).toFloat(exec);
				float y = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), MoveToAction::create(duration, PointMake(x, y)).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "MoveTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionMoveBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//moveBy(float duration, Point deltaPosition)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "deltaPosition", Point)
					) {
				float duration = args.at(0).toFloat(exec);
				Point deltaPosition = ajoa_cast<AJPoint*>(asObject(args.at(1)))->data();
				return toAJ(exec, thisObject->globalObject(), MoveByAction::create(duration, deltaPosition).get());
			}
		}		
		
		exec->clearException();
		//moveBy(float duration, float deltaX, float deltaY)
		if (args.size() >= 3) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "deltaX")
					&& ensureFloatArg(exec, args, 2, "deltaY")
					) {
				float duration = args.at(0).toFloat(exec);
				float deltaX = args.at(1).toFloat(exec);
				float deltaY = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), MoveByAction::create(duration, PointMake(deltaX, deltaY)).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "MoveBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSkewTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//skewTo(float duration, Vector2 skew)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "skew", Vector2)
					) {
				float duration = args.at(0).toFloat(exec);
				Vector2 skew = ajoa_cast<AJVector2*>(asObject(args.at(1)))->data();
				return toAJ(exec, thisObject->globalObject(), SkewToAction::create(duration, skew).get());
			}
		}
		
		exec->clearException();
		//skewTo(float duration, float skewX, float skewY)
		if (args.size() >= 3) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "skewX")
					&& ensureFloatArg(exec, args, 2, "skewY")
					) {
				float duration = args.at(0).toFloat(exec);
				float skewX = args.at(1).toFloat(exec);
				float skewY = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), SkewToAction::create(duration, Vector2Make(skewX, skewY)).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "SkewTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSkewBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//skewBy(float duration, Vector2 deltaSkew)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "deltaSkew", Vector2)
					) {
				float duration = args.at(0).toFloat(exec);
				Vector2 deltaSkew = ajoa_cast<AJVector2*>(asObject(args.at(1)))->data();
				return toAJ(exec, thisObject->globalObject(), SkewByAction::create(duration, deltaSkew).get());
			}
		}
		
		
		exec->clearException();
		//skewBy(float duration, float deltaSkewX, float deltaSkewY)
		if (args.size() >= 3) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "deltaSkewX")
					&& ensureFloatArg(exec, args, 2, "deltaSkewY")
					) {
				float duration = args.at(0).toFloat(exec);
				float deltaSkewX = args.at(1).toFloat(exec);
				float deltaSkewY = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), SkewByAction::create(duration, Vector2Make(deltaSkewX, deltaSkewY)).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "SkewBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionJumpBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//jumpBy(float duration, Point deltaPosition, float height, int jumps)
		if (args.size() >= 4) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "deltaPosition", Point)
					&& ensureFloatArg(exec, args, 2, "height")
					&& ensureIntArg(exec, args, 3, "jumps")
					) {
				float duration = args.at(0).toFloat(exec);
				Point deltaPosition = ajoa_cast<AJPoint*>(asObject(args.at(1)))->data();
				float height = args.at(2).toFloat(exec);
				int jumps = args.at(3).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), JumpByAction::create(duration, deltaPosition, height, jumps).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "JumpBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionJumpTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//jumpTo(float duration, Point position, float height, int jumps)
		if (args.size() >= 4) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "position", Point)
					&& ensureFloatArg(exec, args, 2, "height")
					&& ensureIntArg(exec, args, 3, "jumps")
					) {
				float duration = args.at(0).toFloat(exec);
				Point position = ajoa_cast<AJPoint*>(asObject(args.at(1)))->data();
				float height = args.at(2).toFloat(exec);
				int jumps = args.at(3).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), JumpToAction::create(duration, position, height, jumps).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "JumpTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionBezierBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//bezierBy(float duration, Point endPosition, Point controlPoint1, Point controlPoint2)
		if (args.size() >= 4) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "endPosition", Point)
					&& ENSURE_OBJ_ARG(exec, args, 2, "controlPoint1", Point)
					&& ENSURE_OBJ_ARG(exec, args, 3, "controlPoint2", Point)
					) {
				float duration = args.at(0).toFloat(exec);
				Point endPosition = ajoa_cast<AJPoint*>(asObject(args.at(1)))->data();
				Point controlPoint1 = ajoa_cast<AJPoint*>(asObject(args.at(2)))->data();
				Point controlPoint2 = ajoa_cast<AJPoint*>(asObject(args.at(3)))->data();
				ccBezierConfig config = {endPosition, controlPoint1, controlPoint2};
				return toAJ(exec, thisObject->globalObject(), BezierByAction::create(duration, config).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "BezierBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionBezierTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//bezierTo(float duration, Point endPosition, Point controlPoint1, Point controlPoint2)
		if (args.size() >= 4) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "endPosition", Point)
					&& ENSURE_OBJ_ARG(exec, args, 2, "controlPoint1", Point)
					&& ENSURE_OBJ_ARG(exec, args, 3, "controlPoint2", Point)
					) {
				float duration = args.at(0).toFloat(exec);
				Point endPosition = ajoa_cast<AJPoint*>(asObject(args.at(1)))->data();
				Point controlPoint1 = ajoa_cast<AJPoint*>(asObject(args.at(2)))->data();
				Point controlPoint2 = ajoa_cast<AJPoint*>(asObject(args.at(3)))->data();
				ccBezierConfig config = {endPosition, controlPoint1, controlPoint2};
				return toAJ(exec, thisObject->globalObject(), BezierToAction::create(duration, config).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "BezierTo");
		return jsUndefined();
	}
	
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionScaleTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//scaleTo(float duration, Vector2 scale)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "scale", Vector2)
					) {
				float duration = args.at(0).toFloat(exec);
				Vector2 scale = ajoa_cast<AJVector2*>(asObject(args.at(1)))->data();
				return toAJ(exec, thisObject->globalObject(), ScaleToAction::create(duration, scale).get());
			}
		}
		
		exec->clearException();
		//scaleTo(float duration, float deltaScaleX, float deltaScaleY)
		if (args.size() >= 3) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "scaleX")
					&& ensureFloatArg(exec, args, 2, "scaleY")
					) {
				float duration = args.at(0).toFloat(exec);
				float scaleX = args.at(1).toFloat(exec);
				float scaleY = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), ScaleToAction::create(duration, Vector2Make(scaleX, scaleY)).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ScaleTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionScaleBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//scaleBy(float duration, float deltaScaleXY)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "deltaScaleXY")
					) {
				float duration = args.at(0).toFloat(exec);
				float deltaScaleXY = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), ScaleByAction::create(duration, Vector2Make(deltaScaleXY, deltaScaleXY)).get());
			}
		}
		
		exec->clearException();
		//scaleBy(float duration, Vector2 deltaScale)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "deltaScale", Vector2)
					) {
				float duration = args.at(0).toFloat(exec);
				Vector2 deltaScale = ajoa_cast<AJVector2*>(asObject(args.at(1)))->data();
				return toAJ(exec, thisObject->globalObject(), ScaleByAction::create(duration, deltaScale).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ScaleBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionBlink(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//blink(float duration, int times)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureIntArg(exec, args, 1, "times")
					) {
				float duration = args.at(0).toFloat(exec);
				int times = args.at(1).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), BlinkAction::create(duration, times).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Blink");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeIn(float duration)
		if (args.size() >= 1) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					) {
				float duration = args.at(0).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FadeInAction::create(duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeOut(float duration)
		if (args.size() >= 1) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					) {
				float duration = args.at(0).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FadeOutAction::create(duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeTo(float duration, int opacity)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureIntArg(exec, args, 1, "opacity")
					) {
				float duration = args.at(0).toFloat(exec);
				int opacity = args.at(1).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), FadeToAction::create(duration, opacity).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTintTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//tintTo(float duration, int r, int g, int b)
		if (args.size() >= 4) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureIntArg(exec, args, 1, "r")
					&& ensureIntArg(exec, args, 2, "g")
					&& ensureIntArg(exec, args, 3, "b")
					) {
				float duration = args.at(0).toFloat(exec);
				int r = args.at(1).toInt32(exec);
				int g = args.at(2).toInt32(exec);
				int b = args.at(3).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), TintToAction::create(duration, r, g, b).get());
			}
		}
		
		exec->clearException();
		//tintTo(float duration, color byColor)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureColorArg(exec, args, 1, "byColor")
					) {
				float duration = args.at(0).toFloat(exec);
				Color byColor = toColor(exec, args.at(1));
				return toAJ(exec, thisObject->globalObject(), TintToAction::create(duration, byColor.red(), byColor.green(), byColor.blue()).get());
			}
		}

		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "TintTo");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTintBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//tintBy(float duration, int r, int g, int b)
		if (args.size() >= 4) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureIntArg(exec, args, 1, "r")
					&& ensureIntArg(exec, args, 2, "g")
					&& ensureIntArg(exec, args, 3, "b")
					) {
				float duration = args.at(0).toFloat(exec);
				int r = args.at(1).toInt32(exec);
				int g = args.at(2).toInt32(exec);
				int b = args.at(3).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), TintByAction::create(duration, r, g, b).get());
			}
		}
		
		exec->clearException();
		//tintBy(float duration, color byColor)
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureColorArg(exec, args, 1, "byColor")
					) {
				float duration = args.at(0).toFloat(exec);
				Color byColor = toColor(exec, args.at(1));
				return toAJ(exec, thisObject->globalObject(), TintByAction::create(duration, byColor.red(), byColor.green(), byColor.blue()).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "TintBy");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionDelay(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//delay(float duration)
		if (args.size() >= 1) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					) {
				float duration = args.at(0).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), DelayTimeAction::create(duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "DelayTime");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionAnimate(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//animate(Animation animation, [bool restoreOriginalFrame])
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "animation", Animation)
					) {
				Animation* animation = ajoa_cast<AJAnimation*>(asObject(args.at(0)))->impl();
				bool restoreOriginalFrame = optBoolArg(exec, args, 1, true);
				return toAJ(exec, thisObject->globalObject(), AnimateAction::create(animation, restoreOriginalFrame).get());
			}
		}
		
		
		exec->clearException();
		//animate(float duration, Animation animation, [bool restoreOriginalFrame])
		if (args.size() >= 2) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ENSURE_OBJ_ARG(exec, args, 1, "animation", Animation)
					) {
				float duration = args.at(0).toFloat(exec);
				Animation* animation = ajoa_cast<AJAnimation*>(asObject(args.at(1)))->impl();
				bool restoreOriginalFrame = optBoolArg(exec, args, 2, true);
				return toAJ(exec, thisObject->globalObject(), AnimateAction::create(duration, animation, restoreOriginalFrame).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Animate");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionOrbitCamera(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//orbitCamera(float duration, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX)
		if (args.size() >= 7) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					&& ensureFloatArg(exec, args, 1, "radius")
					&& ensureFloatArg(exec, args, 2, "deltaRadius")
					&& ensureFloatArg(exec, args, 3, "angleZ")
					&& ensureFloatArg(exec, args, 4, "deltaAngleZ")
					&& ensureFloatArg(exec, args, 5, "angleX")
					&& ensureFloatArg(exec, args, 6, "deltaAngleX")
					) {
				float duration = args.at(0).toFloat(exec);
				float radius = args.at(1).toFloat(exec);
				float deltaRadius = args.at(2).toFloat(exec);
				float angleZ = args.at(3).toFloat(exec);
				float deltaAngleZ = args.at(4).toFloat(exec);
				float angleX = args.at(5).toFloat(exec);
				float deltaAngleX = args.at(6).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), OrbitCameraAction::create(duration, radius, deltaRadius, angleZ, deltaAngleZ, angleX, deltaAngleX).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "OrbitCamera");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBackIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeBackIn(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseBackInAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseBackIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBackOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		//easeBackOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseBackOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseBackOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBackInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeBackInOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseBackInOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseBackInOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBounceIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeBounceIn(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseBounceInAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseBounceIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBounceOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeBounceOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseBounceOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseBounceOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBounceInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeBounceInOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseBounceInOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseBounceInOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseElasticIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeElasticIn(IntervalAction action, float period)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "period")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float period = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), EaseElasticInAction::create(action, period).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseElasticIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseElasticOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeElasticOut(IntervalAction action, float period)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "period")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float period = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), EaseElasticOutAction::create(action, period).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseElasticOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseElasticInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeElasticInOut(IntervalAction action, float period)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "period")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float period = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), EaseElasticInOutAction::create(action, period).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseElasticInOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseExponentialIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeExponentialIn(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseExponentialInAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseExponentialIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseExponentialOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeExponentialOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseExponentialOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseExponentialOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseExponentialInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeExponentialInOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseExponentialInOutAction::create(action).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseExponentialInOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeIn(IntervalAction action, float rate)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "rate")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float rate = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), EaseInAction::create(action, rate).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeOut(IntervalAction action, float rate)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "rate")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float rate = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), EaseOutAction::create(action, rate).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeInOut(IntervalAction action, float rate)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					&& ensureFloatArg(exec, args, 1, "rate")
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				float rate = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), EaseInOutAction::create(action, rate).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseInOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseSineIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeSineIn(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseSineInAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseSineIn");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseSineOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeSineOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseSineOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseSineOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseSineInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//easeSineInOut(IntervalAction action)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", IntervalAction)
					) {
				IntervalAction* action = ajoa_cast<AJIntervalAction*>(asObject(args.at(0)))->impl();
				return toAJ(exec, thisObject->globalObject(), EaseSineInOutAction::create(action).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "EaseSineInOut");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionAccelDeccelAmplitude(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//accelDeccelAmplitude(GridBaseAction action, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", GridBaseAction)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridBaseAction* action = ajoa_cast<AJGridBaseAction*>(asObject(args.at(0)))->impl();
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), AccelDeccelAmplitudeAction::create(action, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "AccelDeccelAmplitude");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionAccelAmplitude(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//accelAmplitude(GridBaseAction action, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", GridBaseAction)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridBaseAction* action = ajoa_cast<AJGridBaseAction*>(asObject(args.at(0)))->impl();
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), AccelAmplitudeAction::create(action, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "AccelAmplitude");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionDeccelAmplitude(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//deccelAmplitude(GridBaseAction action, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "action", GridBaseAction)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridBaseAction* action = ajoa_cast<AJGridBaseAction*>(asObject(args.at(0)))->impl();
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), DeccelAmplitudeAction::create(action, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "DeccelAmplitude");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionStopGrid(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//stopGrid()
		return toAJ(exec, thisObject->globalObject(), StopGridAction::create().get());
		
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionReuseGrid(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//reuseGrid(int times)
		if (args.size() >= 1) {
			if (
					ensureIntArg(exec, args, 0, "times")
					) {
				int times = args.at(0).toInt32(exec);
				return toAJ(exec, thisObject->globalObject(), ReuseGridAction::create(times).get());
			}
		}
		
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ReuseGrid");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionWaves3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//waves3D(int waves, float amplitude, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "waves")
					&& ensureFloatArg(exec, args, 1, "amplitude")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int waves = args.at(0).toInt32(exec);
				float amplitude = args.at(1).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), Waves3DAction::create(waves, amplitude, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Waves3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipX3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//flipX3D(float duration)
		if (args.size() >= 1) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					) {
				float duration = args.at(0).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FlipX3DAction::create(duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FlipX3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipY3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//flipY3D(float duration)
		if (args.size() >= 1) {
			if (
					ensureFloatArg(exec, args, 0, "duration")
					) {
				float duration = args.at(0).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FlipY3DAction::create(duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FlipY3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionLens3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//lens3D(Point pos, float radius, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "pos", Point)
					&& ensureFloatArg(exec, args, 1, "radius")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				Point pos = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				float radius = args.at(1).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), Lens3DAction::create(pos, radius, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Lens3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRipple3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//ripple3D(Point pos, float radius, int waves, float amplitude, GridSize gridSize, float duration)
		if (args.size() >= 6) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "pos", Point)
					&& ensureFloatArg(exec, args, 1, "radius")
					&& ensureIntArg(exec, args, 2, "waves")
					&& ensureFloatArg(exec, args, 3, "amplitude")
					&& ENSURE_OBJ_ARG(exec, args, 4, "gridSize", Size)
					&& ensureFloatArg(exec, args, 5, "duration")
					) {
				Point pos = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				float radius = args.at(1).toFloat(exec);
				int waves = args.at(2).toInt32(exec);
				float amplitude = args.at(3).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(4)))->data());
				float duration = args.at(5).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), Ripple3DAction::create(pos, radius, waves, amplitude, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Ripple3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShaky3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//shaky3D(int range, bool shakeZ, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "range")
					&& ensureBoolArg(exec, args, 1, "shakeZ")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int range = args.at(0).toInt32(exec);
				bool shakeZ = args.at(1).toBoolean(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), Shaky3DAction::create(range, shakeZ, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Shaky3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionLiquid(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//liquid(int waves, float amplitude, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "waves")
					&& ensureFloatArg(exec, args, 1, "amplitude")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int waves = args.at(0).toInt32(exec);
				float amplitude = args.at(1).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), LiquidAction::create(waves, amplitude, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Liquid");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionWaves(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//waves(int waves, float amplitude, bool isHorizontal, bool isVertical, Size gridSize, float duration)
		if (args.size() >= 6) {
			if (
					ensureIntArg(exec, args, 0, "waves")
					&& ensureFloatArg(exec, args, 1, "amplitude")
					&& ensureBoolArg(exec, args, 2, "isHorizontal")
					&& ensureBoolArg(exec, args, 3, "isVertical")
					&& ENSURE_OBJ_ARG(exec, args, 4, "gridSize", Size)
					&& ensureFloatArg(exec, args, 5, "duration")
					) {
				int waves = args.at(0).toInt32(exec);
				float amplitude = args.at(1).toFloat(exec);
				bool isHorizontal = args.at(2).toBoolean(exec);
				bool isVertical = args.at(3).toBoolean(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(4)))->data());
				float duration = args.at(5).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), WavesAction::create(waves, amplitude, isHorizontal, isVertical, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Waves");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTwirl(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//twirl(Point pos, int twirls, float amplitude, Size gridSize, float duration)
		if (args.size() >= 5) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "pos", Point)
					&& ensureIntArg(exec, args, 1, "twirls")
					&& ensureFloatArg(exec, args, 2, "amplitude")
					&& ENSURE_OBJ_ARG(exec, args, 3, "gridSize", Size)
					&& ensureFloatArg(exec, args, 4, "duration")
					) {
				Point pos = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				int twirls = args.at(1).toInt32(exec);
				float amplitude = args.at(2).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(3)))->data());
				float duration = args.at(4).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), TwirlAction::create(pos, twirls, amplitude, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Twirl");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionPageTurn3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//pageTurn3D(Size gridSize, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "gridSize", Size)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(0)))->data());
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), PageTurn3DAction::create(gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "PageTurn3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShakyTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//shakyTiles3D(int range, bool shakeZ, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "range")
					&& ensureBoolArg(exec, args, 1, "shakeZ")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int range = args.at(0).toInt32(exec);
				bool shakeZ = args.at(1).toBoolean(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), ShakyTiles3DAction::create(range, shakeZ, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ShakyTiles3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShatteredTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//shatteredTiles3D(int range, bool shatterZ, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "range")
					&& ensureBoolArg(exec, args, 1, "shatterZ")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int range = args.at(0).toInt32(exec);
				bool shatterZ = args.at(1).toBoolean(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), ShatteredTiles3DAction::create(range, shatterZ, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ShatteredTiles3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShuffleTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//shuffleTilesAction(int seed, Size gridSize, float duration)
		if (args.size() >= 3) {
			if (
					ensureIntArg(exec, args, 0, "seed")
					&& ENSURE_OBJ_ARG(exec, args, 1, "gridSize", Size)
					&& ensureFloatArg(exec, args, 2, "duration")
					) {
				int seed = args.at(0).toInt32(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(1)))->data());
				float duration = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), ShuffleTilesAction::create(seed, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "ShuffleTiles");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutTRTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeOutTRTiles(Size gridSize, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "gridSize", Size)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(0)))->data());
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FadeOutTRTilesAction::create(gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeOutTRTiles");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutBLTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeOutBLTiles(Size gridSize, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "gridSize", Size)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(0)))->data());
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FadeOutBLTilesAction::create(gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeOutBLTiles");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutUpTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeOutUpTiles(Size gridSize, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "gridSize", Size)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(0)))->data());
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FadeOutUpTilesAction::create(gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeOutUpTiles");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutDownTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//fadeOutDownTiles(Size gridSize, float duration)
		if (args.size() >= 2) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "gridSize", Size)
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(0)))->data());
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), FadeOutDownTilesAction::create(gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FadeOutDownTiles");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTurnOffTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//turnOffTiles(int seed, Size gridSize, float duration)
		if (args.size() >= 3) {
			if (
					ensureIntArg(exec, args, 0, "seed")
					&& ENSURE_OBJ_ARG(exec, args, 1, "gridSize", Size)
					&& ensureFloatArg(exec, args, 2, "duration")
					) {
				int seed = args.at(0).toInt32(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(1)))->data());
				float duration = args.at(2).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), TurnOffTilesAction::create(seed, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "TurnOffTiles");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionWavesTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//wavesTiles3D(int waves, float amplitude, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "waves")
					&& ensureFloatArg(exec, args, 1, "amplitude")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int waves = args.at(0).toInt32(exec);
				float amplitude = args.at(1).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), WavesTiles3DAction::create(waves, amplitude, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "WavesTiles3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionJumpTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//jumpTiles3D(int waves, float amplitude, Size gridSize, float duration)
		if (args.size() >= 4) {
			if (
					ensureIntArg(exec, args, 0, "waves")
					&& ensureFloatArg(exec, args, 1, "amplitude")
					&& ENSURE_OBJ_ARG(exec, args, 2, "gridSize", Size)
					&& ensureFloatArg(exec, args, 3, "duration")
					) {
				int waves = args.at(0).toInt32(exec);
				float amplitude = args.at(1).toFloat(exec);
				GridSize gridSize = toGridSize(ajoa_cast<AJSize*>(asObject(args.at(2)))->data());
				float duration = args.at(3).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), JumpTiles3DAction::create(waves, amplitude, gridSize, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "JumpTiles3D");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSplitRows(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//splitRows(int rows, float duration)
		if (args.size() >= 2) {
			if (
					ensureIntArg(exec, args, 0, "rows")
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				int rows = args.at(0).toInt32(exec);
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), SplitRowsAction::create(rows, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "SplitRows");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSplitColumns(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//splitColumns(int cols, float duration)
		if (args.size() >= 2) {
			if (
					ensureIntArg(exec, args, 0, "cols")
					&& ensureFloatArg(exec, args, 1, "duration")
					) {
				int cols = args.at(0).toInt32(exec);
				float duration = args.at(1).toFloat(exec);
				return toAJ(exec, thisObject->globalObject(), SplitColumnsAction::create(cols, duration).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "SplitColumns");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShow(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		//show()
		return toAJ(exec, thisObject->globalObject(), ShowAction::create().get());
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionHide(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//hide()
		return toAJ(exec, thisObject->globalObject(), HideAction::create().get());
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionToggleVisible(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		UNUSED_PARAM(args);
		
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//toggleVisible()
		return toAJ(exec, thisObject->globalObject(), ToggleVisibleAction::create().get());
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipX(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//flipX(bool flipX)
		if (args.size() >= 1) {
			if (
					ensureBoolArg(exec, args, 0, "flipX")
					) {
				bool flipX = args.at(0).toBoolean(exec);
				return toAJ(exec, thisObject->globalObject(), FlipXAction::create(flipX).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FlipX");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipY(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//flipY(bool flipY)
		if (args.size() >= 1) {
			if (
					ensureBoolArg(exec, args, 0, "flipY")
					) {
				bool flipY = args.at(0).toBoolean(exec);
				return toAJ(exec, thisObject->globalObject(), FlipYAction::create(flipY).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "FlipY");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionPlace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		
		//place(Point pos)
		if (args.size() >= 1) {
			if (
					ENSURE_OBJ_ARG(exec, args, 0, "pos", Point)
					) {
				Point pos = ajoa_cast<AJPoint*>(asObject(args.at(0)))->data();
				return toAJ(exec, thisObject->globalObject(), PlaceAction::create(pos).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "Place");
		return jsUndefined();
	}
	
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionCallFunction(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args) 
	{
		AJNamespaceActions* thisObject = ajoa_cast<AJNamespaceActions*>(asObject(thisValue));
		//callFunction(function callback)
		if (args.size() >= 1) {
			if (
					ensureFunctionArg(exec, args, 0, "callback", false)
					) {
				AJObject* callback = asObject(args.at(0));
				return toAJ(exec, thisObject->globalObject(), CallAJFunctionAction::create(callback).get());
			}
		}
		
		if (!exec->hadException())
			OA_ERROR_INVALID_ARGS(exec, "callFunction");
		return jsUndefined();
	}
	
}

