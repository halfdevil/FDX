#
APP_OPTIM := release
APP_PLATFORM := android-21
APP_STL := gnustl_static
APP_CPPFLAGS += -frtti 
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -DANDROID
APP_ABI := x86_64
APP_MODULES := FreeImage
NDK_TOOLCHAIN_VERSION := clang
