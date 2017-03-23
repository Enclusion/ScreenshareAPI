//Copyright 2017, Ghose, All rights reserved.
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

#include "com_buzznacker_screenshare_ScreenshareTool.h"
#include "dllmain.h"

typedef std::wstring _WSTRING;

DWORD currentSelectedProcessId;

_WSTRING Java_To_WStr(JNIEnv *env, jstring string)
{
	std::wstring value;

	const jchar *raw = env->GetStringChars(string, 0);
	jsize len = env->GetStringLength(string);

	value.assign(raw, raw + len);

	env->ReleaseStringChars(string, raw);

	return value;
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{
	std::cout << "Successfully attached to the JVM." << std::endl;
	return JNI_VERSION_1_8;
}

JNIEXPORT jobjectArray JNICALL
Java_com_buzznacker_screenshare_ScreenshareTool_getProcessHandles(JNIEnv *env, jobject thisObj)
{
	jobjectArray jArray;

	STRING_VEC vec = (GetProcessHandles(currentSelectedProcessId));

	jArray = (jobjectArray)env->NewObjectArray((jsize)vec.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));

	if (NULL == currentSelectedProcessId)
	{
		std::cout << "You must first select a valid PID to execute a string search" << std::endl;
		return jArray;
	}

	if (vec.empty())
	{
		std::cout << "There were no handles found for the target process" << std::endl;
		return jArray;
	}

	std::string* strArr = vec.data();

	for (int i = 0; i < vec.size(); i++)
	{
		env->SetObjectArrayElement(jArray, i, env->NewStringUTF(strArr[i].c_str()));
	}

	return jArray;
	
}

JNIEXPORT void JNICALL
Java_com_buzznacker_screenshare_ScreenshareTool_searchString(JNIEnv *env, jobject thisObj, jobjectArray jArray) 
{
	if (NULL == currentSelectedProcessId)
	{
		std::cout << "You must first select a valid PID to execute a string search" << std::endl;
		return;
	}

	STRING_VEC my_strings;

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

JNIEXPORT jint JNICALL
Java_com_buzznacker_screenshare_ScreenshareTool_getProcessByNameAndWindowTitle(JNIEnv *env, jobject thisObj, jstring str, jstring str2)
{
	_WSTRING wStr;
	_WSTRING wStr2;
	PFPBNAWT_CTX targetProcesses;
	DWORD pId;
	
	wStr = Java_To_WStr(env, str);
	wStr2 = Java_To_WStr(env, str2);
	targetProcesses = FindProcessByNameAndWindowTitle(wStr.c_str(), wStr2.c_str(), FALSE);

	if (NULL != targetProcesses)
	{
		if (targetProcesses->PidCount != 1)
			return -1;
		pId = targetProcesses->Pids[0];
		return (int)pId;
	}
	else
		return -1;
	return -1;
}

JNIEXPORT jint JNICALL
Java_com_buzznacker_screenshare_ScreenshareTool_getSelectedPid(JNIEnv *env, jobject thisObj)
{
	return currentSelectedProcessId;
}

JNIEXPORT jint JNICALL 
Java_com_buzznacker_screenshare_ScreenshareTool_getProcessIdByName(JNIEnv *env, jobject thisObj, jstring str)
{
	_WSTRING wStr = Java_To_WStr(env, str);
	return FindProcessId(wStr);
}


//Copyright 2017, Ghose, All rights reserved.