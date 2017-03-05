//Copyright 2017, Ghose, All rights reserved.
#include <jni.h>

#ifndef _Included_Main
#define _Included_Main
#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     com.buzznacker.screenshare.ScreenshareTool
	* Method:    searchString
	* Signature: ([Ljava/lang/String;)V
	*/
	JNIEXPORT void JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_searchString
	(JNIEnv *, jobject, jobjectArray);

	/*
	* Class:     com.buzznacker.screenshare.ScreenshareTool
	* Method:    selectProcess
	* Signature: (I)V
	*/
	JNIEXPORT void JNICALL Java_com_buzznacker_screenshare_ScreenshareTool_setSelectedPid
	(JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
//Copyright 2017, Ghose, All rights reserved.