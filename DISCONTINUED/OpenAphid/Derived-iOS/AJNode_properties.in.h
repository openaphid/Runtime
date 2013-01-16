	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return zOrder, int
	*/
	AJ::AJValue ajNodeZOrder(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return camera, Camera
	*/
	AJ::AJValue ajNodeCamera(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return children, NodeList
	*/
	AJ::AJValue ajNodeChildren(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return anchorInPixels, Point
	*/
	AJ::AJValue ajNodeAnchorInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return isRunning, bool
	*/
	AJ::AJValue ajNodeIsRunning(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return parent, Node
	*/
	AJ::AJValue ajNodeParent(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return boundingBox, Rect
	*/
	AJ::AJValue ajNodeBoundingBox(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return boundingBoxInPixels, Rect
	*/
	AJ::AJValue ajNodeBoundingBoxInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return skew, Vector2
	*/
	AJ::AJValue ajNodeSkew(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param skew, Vector2
	*/
	void setAJNodeSkew(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return rotation, float
	*/
	AJ::AJValue ajNodeRotation(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param rotation, float
	*/
	void setAJNodeRotation(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return scale, Vector2
	*/
	AJ::AJValue ajNodeScale(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param scale, Vector2
	*/
	void setAJNodeScale(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return position, Point
	*/
	AJ::AJValue ajNodePosition(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param position, Point
	*/
	void setAJNodePosition(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return positionInPixels, Point
	*/
	AJ::AJValue ajNodePositionInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param positionInPixels, Point
	*/
	void setAJNodePositionInPixels(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return visible, bool
	*/
	AJ::AJValue ajNodeVisible(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param visible, bool
	*/
	void setAJNodeVisible(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return anchor, Point
	*/
	AJ::AJValue ajNodeAnchor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param anchor, Point
	*/
	void setAJNodeAnchor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return contentSize, Size
	*/
	AJ::AJValue ajNodeContentSize(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param contentSize, Size
	*/
	void setAJNodeContentSize(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return contentSizeInPixels, Size
	*/
	AJ::AJValue ajNodeContentSizeInPixels(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param contentSizeInPixels, Size
	*/
	void setAJNodeContentSizeInPixels(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return isRelativeAnchor, bool
	*/
	AJ::AJValue ajNodeIsRelativeAnchor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param isRelativeAnchor, bool
	*/
	void setAJNodeIsRelativeAnchor(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return tag, int
	*/
	AJ::AJValue ajNodeTag(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param tag, int
	*/
	void setAJNodeTag(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return onframeupdate, function
	*/
	AJ::AJValue ajNodeOnframeupdate(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param onframeupdate, function
	*/
	void setAJNodeOnframeupdate(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return ontouchstart, function
	*/
	AJ::AJValue ajNodeOntouchstart(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param ontouchstart, function
	*/
	void setAJNodeOntouchstart(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return ontouchmove, function
	*/
	AJ::AJValue ajNodeOntouchmove(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param ontouchmove, function
	*/
	void setAJNodeOntouchmove(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return ontouchend, function
	*/
	AJ::AJValue ajNodeOntouchend(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param ontouchend, function
	*/
	void setAJNodeOntouchend(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return ontouchcancel, function
	*/
	AJ::AJValue ajNodeOntouchcancel(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param ontouchcancel, function
	*/
	void setAJNodeOntouchcancel(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return touchEnabled, bool
	*/
	AJ::AJValue ajNodeTouchEnabled(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param touchEnabled, bool
	*/
	void setAJNodeTouchEnabled(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return userInteractionEnabled, bool
	*/
	AJ::AJValue ajNodeUserInteractionEnabled(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@param userInteractionEnabled, bool
	*/
	void setAJNodeUserInteractionEnabled(AJ::ExecState* exec, AJ::AJObject* thisObject, AJ::AJValue value);
	/*
	@return constructor, Node
	*/
	AJ::AJValue ajNodeConstructor(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param child, Node
	@param z, int
	@param tag, int
	@return Node
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionAddChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param cleanup, bool
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveFromParent(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param child, Node
	@param cleanup, bool
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param tag, int
	@param cleanup, bool
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveChildByTag(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param cleanup, bool
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRemoveAllChildren(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param tag, int
	@return Node
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionGetChildByTag(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param child, Node
	@param z, int
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionReorderChild(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionCleanup(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, Action
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionRunAction(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionStopAllActions(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param callback, function
	@param interval, float
	@param repeatTimes, int
	@param delay, float
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionScheduleTimer(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param callback, function
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionUnscheduleTimer(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionUnscheduleAllTimers(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionScheduleUpdate(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return void
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionUnscheduleUpdate(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param p, Point
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToNodeSpace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param p, Point
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertParentToNodeSpace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param p, Point
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToWorldSpace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param p, Point
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToNodeSpaceAR(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param p, Point
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionConvertToWorldSpaceAR(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param touch, Touch
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionLocationOfTouch(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param touch, Touch
	@return Point
	*/
	AJ::AJValue JSC_HOST_CALL ajNodePrototypeFunctionLocationAROfTouch(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return INVALID_TAG, int
	*/
	AJ::AJValue ajNodeConstructorINVALID_TAG(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
