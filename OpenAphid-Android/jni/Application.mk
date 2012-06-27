#APP_STL := stlport_static
APP_STL := gnustl_static
APP_PLATFORM := android-8
APP_OPTIM := release

APP_ABI := armeabi-v7a

#ifneq ($(APP_OPTIM),debug)
#	APP_CFLAGS += -O3
#endif
