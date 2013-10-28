package com.ford.avatar.jsonserver;

import com.ford.avatar.util.Const;

import android.util.Log;

public class MBWrapper {

	private static MBWrapper mWraper = null;
	//load native libraries
	static{
		System.loadLibrary("jsoncpp");
		System.loadLibrary("MsgBroker");
		System.loadLibrary("nativepart");
	}

	public static MBWrapper CreateMessageBroker(){
		Log.i("MBWrapper", "Create Message Broker");
//		if(!mIsMsgBrokerCreated){
//			if(CreateInstance()){
//				mIsMsgBrokerCreated = true;
//				return new MBWrapper();
//			}
//		}
//		return null;
		CreateInstance();
		if (mWraper == null)
			mWraper = new MBWrapper();
		return mWraper;
	}
	
	//native functions wrappers
	public void start(JSONServer sender){
		LogMsg("Start MessageBroker");
		StartMessageBroker(sender);
		runMsgProcessThread();
	}
	
	public void stop(){
		StopMessageBroker();
	}
	
	public void destroy(){
		LogMsg("Destroy MB");
		DestroyMessageBroker();
		mIsMsgBrokerCreated = false;
		mWraper = null;
	}
	
	//fd - descriptor
	public void onMsgReceived(int fd, String JSONMsg){
		LogMsg("Invoke OnMsgReceived for fd = " + fd + "; with msg = " + JSONMsg);
		OnMessageReceived(fd, JSONMsg);
	}

	
	public void testMethodForJNI(){
		String testField = "!!!!!!!!!!!!!!!!@@@@@@@@@@";
		LogMsg(testField);
	}
	
	private void runMsgProcessThread(){
		LogMsg("RunMsgProcessThread");
		new Thread(new Runnable() {
			
			public void run() {
				LogMsg("Invoke MethodForThread in new thread");
				MethodForThread(new Object());
			}
		}).start();
	}
	
	//private String testField;
	//==================================================================================
	//private section
	//==================================================================================
	private final String TAG_NAME = "MBWrapper";
	private final boolean DEBUG = false;
	private static boolean mIsMsgBrokerCreated = false;
	
	private MBWrapper(){
		
	}

	//native methods
	private static native boolean CreateInstance();
	private native void StartMessageBroker(JSONServer sender);
	private native void StopMessageBroker();
	private native void DestroyMessageBroker();
	private native void OnMessageReceived(int fd, String JSONMsg);
	private native void MethodForThread(Object arg);
	
	private void LogMsg(String msg){
		if (DEBUG && Const.DEBUG){
			Log.i(TAG_NAME, msg);
		}
	}
	
	
	
}
