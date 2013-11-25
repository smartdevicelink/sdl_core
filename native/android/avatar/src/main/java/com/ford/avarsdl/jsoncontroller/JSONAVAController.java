package com.ford.avarsdl.jsoncontroller;

import android.util.Log;

import com.ford.avarsdl.activity.AvatarActivity;
import com.ford.avarsdl.util.Const;
import com.ford.avarsdl.util.RPCConst;

public class JSONAVAController extends JSONController{
	
	private final boolean DEBUG = true;
	private String mJSComponentName = null;
	private final String TAG_NAME = "AVAController"; 
	private AvatarActivity mActivity;
	
	public JSONAVAController(AvatarActivity activity, String cname) {
		super(RPCConst.CN_AVATAR);
		mActivity = activity;
		mJSComponentName = cname;
	}
	
	public JSONAVAController(String cname, ITcpClient client) {
		super(RPCConst.CN_AVATAR,client);
		mJSComponentName = cname;
	}
	
	
	protected void processRequest(String request){
		processNotification(request);
	}
	
	protected String processNotification(String notification){
		logMsg("Process notification");
		mJSONParser.putJSONObject(notification);
		final String func = "FFW.WebSocketSimulator.receive('" + mJSComponentName 
						 + "','" + notification + "')";
		//LogMsg(func);
		logMsg("Send notification to JS");
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mActivity.getWebView().loadUrl("javascript:" + func);
			}
		});
		return null;
	}
	
	protected void processResponse(String response){
		if (!processRegistrationResponse(response)){
			final String func = "FFW.WebSocketSimulator.receive('" + mJSComponentName 
															 + "','" + response + "')";
			logMsg(func);
			mActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					mActivity.getWebView().loadUrl("javascript:" + func);
				}
			});
		}
			
		
	}
	
	public void sendJSMessage(String cName, String jsonMsg){
		logMsg("SendJSMessage : " + jsonMsg);
		mJSONParser.putJSONObject(jsonMsg);
		if(mJSONParser.getId()>=0 && 
				mJSONParser.getResult() == null && 
				mJSONParser.getError() == null){
			mJSComponentName = cName;	
		}
		jsonMsg += System.getProperty("line.separator");
		sendJSONMsg(jsonMsg);
		
	}
	
	private void logMsg(String msg){
		if (DEBUG && Const.DEBUG){
			Log.i(TAG_NAME, msg);
		}
	}

	
}
