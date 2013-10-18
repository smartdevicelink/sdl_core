/*
 *
 *  Created on: 31.05.2012
 *      Author: KBotnar
 */
#include <string.h>
#include <android/log.h>
#include "com_ford_sdlreverse_jsonserver_MBWrapper.h"
#include "../../../../common/MessageBroker/include/CMessageBroker.hpp"
#include "../../../../common/MessageBroker/include/CSender.hpp"


NsMessageBroker::CMessageBroker* pMsgBroker;
JavaVM *pJavaVM; //save to get *env
jobject clObj;// object to invoke the method

class JNISender : public NsMessageBroker::CSender
{
public:
	virtual ssize_t Send(int fd, const std::string& data)
	{
		//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Send message to controller");
		JNIEnv *penv;
		pJavaVM->GetEnv((void **)&penv, JNI_VERSION_1_6);
		pJavaVM->AttachCurrentThread(&penv,NULL);

		jint descriptor = fd;
		jstring msg = penv->NewStringUTF(data.c_str());
		InvokeJSONServerSend(descriptor, msg);
		penv->DeleteLocalRef(msg);
		return data.length();
	}
};


/*
 * Function:    JNI_OnLoad
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "On load JNI");

	pJavaVM = vm; /* cache the JavaVM pointer */

	JNIEnv *env;
	if (vm->GetEnv((void **)&env, JNI_VERSION_1_6))
		return JNI_ERR; /* JNI version not supported */

	return JNI_VERSION_1_6;

}

/*
 * Function:    JNI_OnUnload
 */
void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "On unload JNI");

	JNIEnv  *penv;
    vm->GetEnv((void **)&penv, JNI_VERSION_1_6);

    penv->DeleteGlobalRef(clObj);
}

/*
 * Function:    InvokeJavaMethod
 * invoke method Send from Server
 */
void InvokeJSONServerSend(jint fd, jstring msg)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Invoke JSON server send function");

	JNIEnv *penv;
	pJavaVM->GetEnv((void **)&penv, JNI_VERSION_1_6);

	pJavaVM->AttachCurrentThread(&penv,NULL);

	jclass clMBWrapper = penv->GetObjectClass(clObj);//penv->FindClass("com/ford/sdlreverse/jsonserver/JSONServer");//penv->GetObjectClass(clObj);
	if(clMBWrapper == NULL)
	{
		return;
	}
	jclass gclMBWrapper = (jclass)penv->NewGlobalRef(clMBWrapper);
	if(gclMBWrapper == NULL)
		return;
	jmethodID sendID = penv->GetMethodID(gclMBWrapper,"Send","(ILjava/lang/String;)I");
	if(sendID == NULL)
			return;
	penv->CallNonvirtualIntMethod(clObj,gclMBWrapper,sendID, fd, msg);
	penv->DeleteGlobalRef(gclMBWrapper);
	penv->DeleteLocalRef(clMBWrapper);

}

/*
 * Class:     com_ford_sdlreverse_jsonserver_MBWrapper
 * Method:    CreateInstance
 * Signature: ()Ljava/lang/Object;
 */
JNIEXPORT jboolean JNICALL Java_com_ford_sdlreverse_jsonserver_MBWrapper_CreateInstance
  (JNIEnv *penv, jclass cl)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Create MB Instance");
	pMsgBroker = NsMessageBroker::CMessageBroker::getInstance();

	if(pMsgBroker != NULL)
		return true;
	else
		return false;
}

/*
 * Class:     com_ford_sdlreverse_jsonserver_MBWrapper
 * Method:    StartMessageBroker
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_ford_sdlreverse_jsonserver_MBWrapper_StartMessageBroker
  (JNIEnv *penv, jobject obj, jobject sender)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Start Message Broker");
	clObj = penv->NewGlobalRef(sender);
	JNISender* mysender = new JNISender();
	printf("======== Start MB ======== \n");
	pMsgBroker->startMessageBroker(mysender);
	//delete mysender;

}

/*
 * Class:     com_ford_sdlreverse_jsonserver_MBWrapper
 * Method:    StopMessageBroker
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT void JNICALL Java_com_ford_sdlreverse_jsonserver_MBWrapper_StopMessageBroker
  (JNIEnv *penv, jobject jobject)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Stop Message Broker");
	pMsgBroker->stopMessageBroker();
}

/*
 * Class:     com_ford_sdlreverse_jsonserver_MBWrapper
 * Method:    DestroyMessageBroker
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_ford_sdlreverse_jsonserver_MBWrapper_DestroyMessageBroker
  (JNIEnv *penv, jobject obj)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Destroy Message Broker");
	delete pMsgBroker;
}

/*
 * Class:     com_ford_sdlreverse_jsonserver_MBWrapper
 * Method:    OnMessageReceived
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_ford_sdlreverse_jsonserver_MBWrapper_OnMessageReceived
  (JNIEnv *penv, jobject obj, jint fd, jstring JSONMsg)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "On message receive");
	int fdescr = fd;
	const char* jsonMsg = penv->GetStringUTFChars(JSONMsg,NULL);
	std::string msg = jsonMsg;
	pMsgBroker->onMessageReceived(fdescr,msg);
	penv->ReleaseStringUTFChars(JSONMsg,jsonMsg);
	//delete jsonMsg;
}

/*
 * Class:     com_ford_sdlreverse_jsonserver_MBWrapper
 * Method:    MethodForThread
 * Signature: (Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_ford_sdlreverse_jsonserver_MBWrapper_MethodForThread
  (JNIEnv *penv, jobject obj, jobject arg)
{
	//__android_log_write(ANDROID_LOG_INFO, "NATIVEPART", "Invoke MB method to process msg queue");
	pMsgBroker->MethodForThread(arg);
}

