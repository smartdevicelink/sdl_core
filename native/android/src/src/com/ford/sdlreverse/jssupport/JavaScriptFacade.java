package com.ford.sdlreverse.jssupport;

import java.util.HashMap;

import android.util.Log;

import com.ford.sdlreverse.activity.AvatarActivity;
import com.ford.sdlreverse.jsoncontroller.JSONAVAController;
import com.ford.sdlreverse.util.Const;

public class JavaScriptFacade {

	private final boolean DEBUG = true;
	private final static String TAG = JavaScriptFacade.class.getSimpleName();
	private AvatarActivity mVideoActivity;
	
	private HashMap<String, JSONAVAController> mJSComponents;
	
	public JavaScriptFacade(AvatarActivity activity) {
		this.mVideoActivity = activity;
		mJSComponents = new HashMap<String, JSONAVAController>();
	}

	// /////////////RPC//////////////////////////////////////
	public void send(String cName, String jsonMsg){
		logMsg("send from JS");
		if(mJSComponents.containsKey(cName))
			mJSComponents.get(cName).sendJSMessage(cName, jsonMsg);
		else{
			JSONAVAController ctrl = new JSONAVAController(mVideoActivity, cName); 
			mJSComponents.put(cName, ctrl);
			ctrl.sendJSMessage(cName, jsonMsg);
		}
		
	}
	
	// ////////////////// OTHER ////////////////////

	public void print(String str) {
		logMsg("Print = " + str);
	}

	private void logMsg(String text) {
		if (DEBUG && Const.DEBUG) {
			Log.d(TAG, text);
		}
	}
}
