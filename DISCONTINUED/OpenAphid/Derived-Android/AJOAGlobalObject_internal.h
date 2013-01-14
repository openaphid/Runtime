AJ::Structure* AJOAGlobalObject::createOAStructure(AJOABindingType type, AJ::ExecState* exec, AJOAGlobalObject* globalObject)
{
	AJOAStructureVector& structures = globalObject->structures();
	ASSERT(!structures.at((unsigned)type).get());
	Structure* s = 0;
	switch(type) {
		case AJActionBindingType:
			s = AJAction::createStructure(AJAction::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJAffineTransformationBindingType:
			s = AJAffineTransformation::createStructure(AJAffineTransformation::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJAnimationBindingType:
			s = AJAnimation::createStructure(AJAnimation::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJCameraBindingType:
			s = AJCamera::createStructure(AJCamera::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJColorBindingType:
			s = AJColor::createStructure(AJColor::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJColorNodeBindingType:
			s = AJColorNode::createStructure(AJColorNode::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJConsoleBindingType:
			s = AJConsole::createStructure(AJConsole::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJDirectorBindingType:
			s = AJDirector::createStructure(AJDirector::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJDynamicBindingBindingType:
			s = AJDynamicBinding::createStructure(AJDynamicBinding::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJEaseActionBindingType:
			s = AJEaseAction::createStructure(AJEaseAction::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJEventBindingType:
			s = AJEvent::createStructure(AJEvent::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJFiniteTimeActionBindingType:
			s = AJFiniteTimeAction::createStructure(AJFiniteTimeAction::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJFontBindingType:
			s = AJFont::createStructure(AJFont::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJGradientNodeBindingType:
			s = AJGradientNode::createStructure(AJGradientNode::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJGridBaseActionBindingType:
			s = AJGridBaseAction::createStructure(AJGridBaseAction::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJInstantActionBindingType:
			s = AJInstantAction::createStructure(AJInstantAction::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJIntervalActionBindingType:
			s = AJIntervalAction::createStructure(AJIntervalAction::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJLabelTTFBindingType:
			s = AJLabelTTF::createStructure(AJLabelTTF::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceActionsBindingType:
			s = AJNamespaceActions::createStructure(AJNamespaceActions::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceAndroidBindingType:
			s = AJNamespaceAndroid::createStructure(AJNamespaceAndroid::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceAphidBindingType:
			s = AJNamespaceAphid::createStructure(AJNamespaceAphid::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceCoreBindingType:
			s = AJNamespaceCore::createStructure(AJNamespaceCore::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceExtBindingType:
			s = AJNamespaceExt::createStructure(AJNamespaceExt::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceG2DBindingType:
			s = AJNamespaceG2D::createStructure(AJNamespaceG2D::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNamespaceJSBindingType:
			s = AJNamespaceJS::createStructure(AJNamespaceJS::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNodeBindingType:
			s = AJNode::createStructure(AJNode::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJNodeListBindingType:
			s = AJNodeList::createStructure(AJNodeList::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJParticleSystemBindingType:
			s = AJParticleSystem::createStructure(AJParticleSystem::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJPointBindingType:
			s = AJPoint::createStructure(AJPoint::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJProgressEventBindingType:
			s = AJProgressEvent::createStructure(AJProgressEvent::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJRectBindingType:
			s = AJRect::createStructure(AJRect::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJSceneBindingType:
			s = AJScene::createStructure(AJScene::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJSizeBindingType:
			s = AJSize::createStructure(AJSize::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJSpriteBindingType:
			s = AJSprite::createStructure(AJSprite::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJSpriteBatchNodeBindingType:
			s = AJSpriteBatchNode::createStructure(AJSpriteBatchNode::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJSpriteFrameBindingType:
			s = AJSpriteFrame::createStructure(AJSpriteFrame::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJTexture2DBindingType:
			s = AJTexture2D::createStructure(AJTexture2D::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJTouchBindingType:
			s = AJTouch::createStructure(AJTouch::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJTouchEventBindingType:
			s = AJTouchEvent::createStructure(AJTouchEvent::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJTouchListBindingType:
			s = AJTouchList::createStructure(AJTouchList::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJVector2BindingType:
			s = AJVector2::createStructure(AJVector2::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJVector3BindingType:
			s = AJVector3::createStructure(AJVector3::createPrototype(exec, globalObject)).releaseRef();
			break;
		case AJXMLHttpRequestBindingType:
			s = AJXMLHttpRequest::createStructure(AJXMLHttpRequest::createPrototype(exec, globalObject)).releaseRef();
			break;
		default:
			oa_error("unknown structure type: %d", (unsigned)type);
			break;
	}

	ASSERT(s);

	AJOAStructureVector::iterator it = structures.begin() + (unsigned)type;
	*it = adoptRef(s);
	return s;
}

AJ::AJObject * AJOAGlobalObject::createOAConstructor(AJOABindingType type, AJ::ExecState* exec, const AJOAGlobalObject* globalObject)
{
	AJOAConstructorVector& constructors = globalObject->constructors();
	AJObject* constructor = 0;
	ASSERT(!constructors.at((unsigned)type));
		switch(type) {
		case AJAffineTransformationBindingType:
			constructor = new(exec) AJAffineTransformationConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJAnimationBindingType:
			constructor = new(exec) AJAnimationConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJCameraBindingType:
			constructor = new(exec) AJCameraConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJColorBindingType:
			constructor = new(exec) AJColorConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJColorNodeBindingType:
			constructor = new(exec) AJColorNodeConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJFontBindingType:
			constructor = new(exec) AJFontConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJGradientNodeBindingType:
			constructor = new(exec) AJGradientNodeConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJLabelTTFBindingType:
			constructor = new(exec) AJLabelTTFConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJNodeBindingType:
			constructor = new(exec) AJNodeConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJParticleSystemBindingType:
			constructor = new(exec) AJParticleSystemConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJPointBindingType:
			constructor = new(exec) AJPointConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJRectBindingType:
			constructor = new(exec) AJRectConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJSceneBindingType:
			constructor = new(exec) AJSceneConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJSizeBindingType:
			constructor = new(exec) AJSizeConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJSpriteBindingType:
			constructor = new(exec) AJSpriteConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJSpriteBatchNodeBindingType:
			constructor = new(exec) AJSpriteBatchNodeConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJSpriteFrameBindingType:
			constructor = new(exec) AJSpriteFrameConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJTexture2DBindingType:
			constructor = new(exec) AJTexture2DConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJVector2BindingType:
			constructor = new(exec) AJVector2Constructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJVector3BindingType:
			constructor = new(exec) AJVector3Constructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		case AJXMLHttpRequestBindingType:
			constructor = new(exec) AJXMLHttpRequestConstructor(exec, const_cast<AJOAGlobalObject*>(globalObject));
			break;
		default:
			oa_error("unknown constructor type: %d", (unsigned)type);
			break;
	}

	AJOAConstructorVector::iterator it = constructors.begin() + (unsigned)type;
	*it = constructor;
	return constructor;
}