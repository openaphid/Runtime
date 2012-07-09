	///-------------------------------------------------------------------------------------------------------------------
	/*
	@return TYPE_MASK_DEFAULT, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_DEFAULT(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_FINITE_TIME, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_FINITE_TIME(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_INTERVAL, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_INTERVAL(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_INSTANT, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_INSTANT(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_EASE, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_EASE(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_GRID, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_GRID(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_GRID3D, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_GRID3D(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_TILED_GRID3D, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_TILED_GRID3D(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_CAMERA, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_CAMERA(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	/*
	@return TYPE_MASK_CALL_FUNCTION, int
	*/
	AJ::AJValue ajNamespaceActionsTYPE_MASK_CALL_FUNCTION(AJ::ExecState* exec, AJ::AJValue thisValue, const AJ::Identifier&);
	///-------------------------------------------------------------------------------------------------------------------
	/*
	@param action, IntervalAction
	@return Action
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRepeatForever(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param speed, float
	@return Action
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSpeed(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param node, Node
	@param worldBoundary, Rect
	@return Action
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFollow(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action0, FiniteTimeAction
	@param action1, FiniteTimeAction
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSequence(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, FiniteTimeAction
	@param times, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRepeat(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action0, FiniteTimeAction
	@param action1, FiniteTimeAction
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSpawn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param angle, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRotateTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param deltaAngle, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRotateBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param position, Point
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionMoveTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param deltaPosition, Point
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionMoveBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param skew, Vector2
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSkewTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param deltaSkew, Vector2
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSkewBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param deltaPosition, Point
	@param height, float
	@param jumps, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionJumpBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param position, Point
	@param height, float
	@param jumps, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionJumpTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param endPosition, Point
	@param controlPoint1, Point
	@param controlPoint2, Point
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionBezierBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param endPosition, Point
	@param controlPoint1, Point
	@param controlPoint2, Point
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionBezierTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param scale, Vector2
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionScaleTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param deltaScaleXY, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionScaleBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param times, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionBlink(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param opacity, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param r, int
	@param g, int
	@param b, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTintTo(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param r, int
	@param g, int
	@param b, int
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTintBy(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionDelay(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param animation, Animation
	@param restoreOriginalFrame, bool
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionAnimate(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@param radius, float
	@param deltaRadius, float
	@param angleZ, float
	@param deltaAngleZ, float
	@param angleX, float
	@param deltaAngleX, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionOrbitCamera(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBackIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBackOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBackInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBounceIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBounceOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseBounceInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param period, float
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseElasticIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param period, float
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseElasticOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param period, float
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseElasticInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseExponentialIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseExponentialOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseExponentialInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param rate, float
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param rate, float
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@param rate, float
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseSineIn(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseSineOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, IntervalAction
	@return EaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionEaseSineInOut(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, GridBaseAction
	@param duration, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionAccelDeccelAmplitude(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, GridBaseAction
	@param duration, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionAccelAmplitude(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param action, GridBaseAction
	@param duration, float
	@return IntervalAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionDeccelAmplitude(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionStopGrid(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param times, int
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionReuseGrid(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param waves, int
	@param amplitude, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionWaves3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipX3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipY3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param pos, Point
	@param radius, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionLens3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param pos, Point
	@param radius, float
	@param waves, int
	@param amplitude, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionRipple3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param range, int
	@param shakeZ, bool
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShaky3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param waves, int
	@param amplitude, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionLiquid(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param waves, int
	@param amplitude, float
	@param isHorizontal, bool
	@param isVertical, bool
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionWaves(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param pos, Point
	@param twirls, int
	@param amplitude, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTwirl(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionPageTurn3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param range, int
	@param shakeZ, bool
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShakyTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param range, int
	@param shatterZ, bool
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShatteredTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param seed, int
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShuffleTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutTRTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutBLTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutUpTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFadeOutDownTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param seed, int
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionTurnOffTiles(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param waves, int
	@param amplitude, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionWavesTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param waves, int
	@param amplitude, float
	@param gridSize, Size
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionJumpTiles3D(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param rows, int
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSplitRows(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param cols, int
	@param duration, float
	@return GridBaseAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionSplitColumns(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionShow(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionHide(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionToggleVisible(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param flipX, bool
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipX(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param flipY, bool
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionFlipY(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param pos, Point
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionPlace(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
	/*
	@param callback, function
	@return InstantAction
	*/
	AJ::AJValue JSC_HOST_CALL ajNamespaceActionsPrototypeFunctionCallFunction(AJ::ExecState* exec, AJ::AJObject*, AJ::AJValue thisValue, const AJ::ArgList& args);
