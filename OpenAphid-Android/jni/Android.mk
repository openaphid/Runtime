LOCAL_PATH := ../OpenAphid
OA_AJ_INCLUDES := ../includes-aj
OA_DERIVED := ../OpenAphid/Derived
#$(call my-dir)

OA_LOCAL_CFLAGS := \
	-DHAVE_DTRACE=0 \
	-DUCONFIG_NO_COLLATION=1 \
	-DGL_GLEXT_PROTOTYPES

#.cpp MUST be the first one, otherwise STL won't work
OA_LOCAL_CPP_EXTENSION := .cpp .cxx

OA_LOCAL_CPP_FEATURES := rtti

OA_LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(OA_AJ_INCLUDES)/ \
	$(OA_DERIVED) \
	$(LOCAL_PATH)/Android \
	$(LOCAL_PATH)/Android/Derived \
	$(OA_AJ_INCLUDES)/bytecompiler/ \
	$(OA_AJ_INCLUDES)/icu/ \
	$(OA_AJ_INCLUDES)/icu/unicode/ \
	$(OA_AJ_INCLUDES)/yarr/ \
	$(OA_AJ_INCLUDES)/pcre/ \
	$(OA_AJ_INCLUDES)/bytecode/ \
	$(OA_AJ_INCLUDES)/parser/ \
	$(OA_AJ_INCLUDES)/runtime/ \
	$(OA_AJ_INCLUDES)/API/ \
	$(OA_AJ_INCLUDES)/interpreter/ \
	$(OA_AJ_INCLUDES)/debugger/ \
	$(OA_AJ_INCLUDES)/jit/ \
	$(OA_AJ_INCLUDES)/jni/ \
	$(OA_AJ_INCLUDES)/jni/jsc/ \
	$(OA_AJ_INCLUDES)/profiler/ \
	$(OA_AJ_INCLUDES)/assembler/ \
	$(OA_AJ_INCLUDES)/wtf/ \
	$(OA_AJ_INCLUDES)/wtf/iphone/ \
	$(OA_AJ_INCLUDES)/wtf/unicode/ \
	$(OA_AJ_INCLUDES)/wtf/unicode/glib/ \
	$(OA_AJ_INCLUDES)/wtf/unicode/icu/ \
	$(OA_AJ_INCLUDES)/wtf/android/ \
	$(OA_AJ_INCLUDES)/wtf/text/ \
	$(OA_AJ_INCLUDES)/wtf/url/ \
	$(OA_AJ_INCLUDES)/wtf/url/src/

#------------------------------------------------------------------------------
# PreBuilt libAJ

include $(CLEAR_VARS)

LOCAL_MODULE := AJ-prebuilt

LOCAL_SRC_FILES := $(LOCAL_PATH)/libs/Android/$(APP_OPTIM)/$(TARGET_ARCH_ABI)/libAJ.a

include $(PREBUILT_STATIC_LIBRARY)

#------------------------------------------------------------------------------

include $(CLEAR_VARS)

LOCAL_MODULE := libOpenAphid

TARGET_PLATFORM := android-8

LOCAL_CFLAGS := $(OA_LOCAL_CFLAGS)

LOCAL_CPP_EXTENSION := $(OA_LOCAL_CPP_EXTENSION)

LOCAL_CPP_FEATURES := $(OA_LOCAL_CPP_FEATURES)

LOCAL_C_INCLUDES := $(OA_LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	AJAction.cpp \
	AJAffineTransformation.cpp \
	AJAnimation.cpp \
	AJCallback.cpp \
	AJCamera.cpp \
	AJColor.cpp \
	AJColorNode.cpp \
	AJConsole.cpp \
	AJDirector.cpp \
	AJDynamicBinding.cpp \
	AJEaseAction.cpp \
	AJEvent.cpp \
	AJEventListener.cpp \
	AJFiniteTimeAction.cpp \
	AJFont.cpp \
	AJGradientNode.cpp \
	AJGridBaseAction.cpp \
	AJInstantAction.cpp \
	AJIntervalAction.cpp \
	AJLabelTTF.cpp \
	AJNamespaceActions.cpp \
	AJNamespaceAphid.cpp \
	AJNamespaceCore.cpp \
	AJNamespaceExt.cpp \
	AJNamespaceG2D.cpp \
	AJNamespaceJS.cpp \
	AJNamespaceiOS.cpp \
	AJNode.cpp \
	AJNodeList.cpp \
	AJOABinding.cpp \
	AJOAGlobalObject.cpp \
	AJOAObject.cpp \
	AJParticleSystem.cpp \
	AJPoint.cpp \
	AJProgressEvent.cpp \
	AJRect.cpp \
	AJScene.cpp \
	AJSize.cpp \
	AJSprite.cpp \
	AJSpriteBatchNode.cpp \
	AJSpriteFrame.cpp \
	AJTexture2D.cpp \
	AJTouch.cpp \
	AJTouchEvent.cpp \
	AJTouchList.cpp \
	AJVector2.cpp \
	AJVector3.cpp \
	AJXMLHttpRequest.cpp \
	Action.cpp \
	ActionCamera.cpp \
	ActionEase.cpp \
	ActionGrid.cpp \
	ActionGrid3D.cpp \
	ActionInstant.cpp \
	ActionInterval.cpp \
	ActionManager.cpp \
	ActionTiledGrid.cpp \
	AffineTransformation.cpp \
	Animation.cpp \
	AtlasNode.cpp \
	Camera.cpp \
	Color.cpp \
	Console.cpp \
	Director.cpp \
	Event.cpp \
	Font.cpp \
	Grid.cpp \
	KURL.cpp \
	Label.cpp \
	NamespaceActions.cpp \
	NamespaceAphid.cpp \
	NamespaceCore.cpp \
	NamespaceExt.cpp \
	NamespaceG2D.cpp \
	NamespaceJS.cpp \
	NamespaceiOS.cpp \
	Node.cpp \
	NodeExt.cpp \
	NodeList.cpp \
	OAConfig.cpp \
	OAData.cpp \
	OAGlobalObject.cpp \
	OAUtil.cpp \
	ParticleSystem.cpp \
	PendingObject.cpp \
	Scene.cpp \
	Scheduler.cpp \
	ScriptCallStack.cpp \
	Sprite.cpp \
	SpriteBatchNode.cpp \
	SpriteFrame.cpp \
	TextCodec.cpp \
	TextCodecICU.cpp \
	TextEncoding.cpp \
	Texture2D.cpp \
	TextureAtlas.cpp \
	Touch.cpp \
	XMLHttpRequest.cpp \
	glu.cpp \
	Android/Diagnostic+Android.cpp \
	Android/Director+Android.cpp \
	Android/G2DConfiguration+Android.cpp \
	Android/HttpDownload+Android.cpp \
	Android/Label+Android.cpp \
	Android/Node+Android.cpp \
	Android/ResourceManager+Android.cpp \
	Android/ScriptBridge+Android.cpp \
	Android/Texture2D+Android.cpp \
	Android/Touch+Android.cpp \
	Android/OAICUFix.cpp \
	Android/OAJNIExport.cpp \
	Android/OAJNIUtil.cpp

LOCAL_STATIC_LIBRARIES = AJ-prebuilt

LOCAL_LDLIBS := \
	-llog -ljnigraphics -lGLESv1_CM -ldl -lz -ljnigraphics -L$(LOCAL_PATH)/libs/Android/ -licuuc_static

include $(BUILD_SHARED_LIBRARY)
