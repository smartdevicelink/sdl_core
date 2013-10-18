package com.ford.sdlreverse.jsoncontroller;

import android.os.Message;
import android.util.Log;

import com.ford.sdlreverse.jsonparser.EBEMethods;
import com.ford.sdlreverse.jsonparser.ERateMethods;
import com.ford.sdlreverse.rater.AppRater;
import com.ford.sdlreverse.util.Const;
import com.ford.sdlreverse.util.MessageConst;
import com.ford.sdlreverse.util.RPCConst;

public class JSONRateController extends JSONController {
	
	private final boolean DEBUG = true;
	private final String TAG = JSONRateController.class.getSimpleName();
	private AppRater mAppRater;
	
	public JSONRateController(AppRater rater){
		super(RPCConst.CN_RATE);
		mAppRater = rater;
	}
	
	public void sendRateAppRequest(){
//		String method = RPCConst.CN_RATE_CLIENT + "." 
//				+ ERateMethods.rateApp.toString();
//		mJSONParser.putEmptyJSONObject();
//		sendRequest(method, mJSONParser.getJSONObject());
	}

	public void sendErrorNotification(int errCode, String msg){
//		String method = RPCConst.CN_RATE + "." 
//				+ ERateMethods.showError.toString();
//		mJSONParser.putEmptyJSONObject();
//		mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE, errCode);
//		mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE, msg);
//		sendNotification(method, mJSONParser.getJSONObject());
	}
	
	@Override
	protected void processResponse(String response){
		logMsg("Process response");
		if (!processRegistrationResponse(response)) {
			mJSONParser.putJSONObject(response);
			mRequestResponse = mJSONParser.getResult();
			mJSONParser.putJSONObject(mRequestResponse);
			String decision = mJSONParser.getStringParam(RPCConst.TAG_RATE_CHOICE);
			if (decision.compareTo("OK")==0){//user choose Rate Now
				mAppRater.loadGooglePlay(this);
			}
		}

	}
	protected void processRequest(String request){
		String result = processNotification(request);
		mJSONParser.putJSONObject(request);
		sendResponse(mJSONParser.getId(), result);
	}
	
	protected String processNotification(String notification){
		mJSONParser.putJSONObject(notification);
		String method = mJSONParser.getMethod();
		method = method.substring(method.indexOf('.')+1, method.length());
		String out = null;
		switch(ERateMethods.valueOf(method)){
		case rateApp:
			out = rateApp();
			break;
		default:
			mJSONParser.putEmptyJSONRPCObject();
			mJSONParser.putStringValue(RPCConst.TAG_ERROR, "Rate Controller does not support function : " + method);
			out = mJSONParser.getJSONObjectAsString();
			break;
		}
		return out;
	}
	
	private String rateApp(){
		int res = mAppRater.loadGooglePlay(this);
		if (res == 0){//succeed
			mJSONParser.putEmptyJSONObject();
			mJSONParser.putStringValue("result", "OK");
			return mJSONParser.getJSONObjectAsString();
		} else if (res == Const.RATE_NO_NETWORK_ERR_CODE){
			return composeErrorResponse(res, MessageConst.NET_NET_NOT_AVAILABLE);
		} else if (res == Const.RATE_GOOGLEPLAY_ERR_CODE){
			return composeErrorResponse(res, MessageConst.RATE_GOOGLEPLAY_IS_UNAVAILABLE);
		}
		return null;
		
	}
	
	public String composeErrorResponse(int errCode, String msg){
		mJSONParser.putEmptyJSONObject();
		mJSONParser.putStringValue(RPCConst.TAG_ERROR, "");
		mJSONParser.putIntValue(RPCConst.TAG_ERROR_CODE, errCode);
		mJSONParser.putStringValue(RPCConst.TAG_ERROR_MESSAGE, msg);
		return mJSONParser.getJSONObjectAsString();
	}


	private void logMsg(String msg){
		if (DEBUG && Const.DEBUG){
			Log.i(TAG, msg);
		}
	}
}
