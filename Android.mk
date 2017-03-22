LOCAL_PATH:= $(call my-dir)

include $(LOCAL_PATH)/config.mk


## Build native broadcast lib
ifeq ($(strip $(SUPPORT_NATIVE_BROADCAST)),yes)
include $(CLEAR_VARS)
LOCAL_SRC_FILES += \
	src/NativeBroadcast.cpp

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/inc

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libutils \
	libcutils \
	libbinder

LOCAL_CFLAGS += -DSUPPORT_NATIVE_BROADCAST

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := $(NATIVE_BROADCAST_MODULE)
include $(BUILD_SHARED_LIBRARY)
endif


### Test
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES += \
	main.cpp

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/inc

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libutils \
	libcutils \
	libbinder

## Add native broadcast lib and macro
ifeq ($(strip $(SUPPORT_NATIVE_BROADCAST)),yes)
LOCAL_CFLAGS += -DSUPPORT_NATIVE_BROADCAST
LOCAL_SHARED_LIBRARIES += $(NATIVE_BROADCAST_MODULE)
endif

LOCAL_MODULE := ank-test
include $(BUILD_EXECUTABLE)

