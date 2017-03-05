//Copyright 2017, Ghose, All rights reserved.
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

#include "com_buzznacker_screenshare_ScreenshareTool.h"
#include "dllmain.h"

DWORD currentSelectedProcessId;

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{
	std::cout << "Successfully attached to the JVM." << std::endl;
	return JNI_VERSION_1_8;
}

JNIEXPORT void JNICALL
Java_com_buzznacker_screenshare_ScreenshareTool_searchString(JNIEnv *env, jobject thisObj, jobjectArray jArray) {
	if (NULL == currentSelectedProcessId)
	{
		std::cout << "You must first select a valid PID to execute a string search" << std::endl;
		return;
	}

	std::vector<std::string> my_strings;

	int stringCount = env->GetArrayLength(jArray);

	for (int i = 0; i < stringCount; i++) {
		jstring str = (jstring)(env->GetObjectArrayElement(jArray, i));
		const char *rawStr = env->GetStringUTFChars(str, 0);
		my_strings.push_back(rawStr);
	}

	StringSearch(currentSelectedProcessId, my_strings);
}

JNIEXPORT void JNICALL
Java_com_buzznacker_screenshare_ScreenshareTool_setSelectedPid(JNIEnv *env, jobject thisObj, jint jpId)
{
	currentSelectedProcessId = jpId;
	std::cout << "Selected PID [" << jpId << "]" << std::endl;
}
//Copyright 2017, Ghose, All rights reserved.