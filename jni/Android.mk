LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include ../includeOpenCV.mk
ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
	#try to load OpenCV.mk from default install location
	include $(TOOLCHAIN_PREBUILT_ROOT)/user/share/OpenCV/OpenCV.mk
else
	include $(OPENCV_MK_PATH)
endif

LOCAL_MODULE    := JniOpenCVDemo

LOCAL_SRC_FILES := BlobContour.cpp ComponentLabeling.cpp viGetPreDefinedBoxes.cpp blob.cpp main.cpp viPreGetCardNumRect.cpp 			BlobOperators.cpp  viCardRecogLib.cpp BlobResult.cpp     viGetCardNumberRect.cpp

LOCAL_LDLIBS +=  -llog -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
