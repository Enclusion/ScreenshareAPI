//Copyright 2017, Ghose, All rights reserved.
#include <jni.h>

#ifndef _Included_Main
#define _Included_Main
#ifdef __cplusplus
extern "C" {
#endif
	
	JNIEXPORT void JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_searchString
	(JNIEnv *, jobject, jobjectArray);

	JNIEXPORT void JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_setSelectedPid
	(JNIEnv *, jobject, jint);

	JNIEXPORT jobjectArray JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_getProcessHandles
	(JNIEnv *, jobject);

	JNIEXPORT jint JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_getProcessByNameAndWindowTitle
	(JNIEnv *, jobject, jstring, jstring);

	JNIEXPORT jint JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_getSelectedPid
	(JNIEnv *, jobject);

	JNIEXPORT jint JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_getProcessIdByName
	(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
//Copyright 2017, Ghose, All rights reserved.