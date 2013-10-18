package com.ford.avatar.test.stubs;

import java.io.IOException;
import java.net.Socket;

import org.json.JSONException;
import org.json.JSONObject;

import com.ford.sdlreverse.jsoncontroller.ITcpClient;
import com.ford.sdlreverse.jsonparser.EMBMethods;
import com.ford.sdlreverse.util.RPCConst;

public class TCPClientStub implements ITcpClient{
	
	public TCPClientStub(String adress, int port){
		mAdress = adress;
		mPort = port;
		connect();
	}
	
	@Override
	public void sendMsg(String msg){
		mMessage = msg;
	}
	
	@Override
	public String receiveMsg(){
		if (mMessage != null){
			return composeResponse();
		} else {
			return null;
		}
	}
	
	@Override
	public void connect() {
		mConnected = true;
	}

	@Override
	public void disconnect(){
		mConnected = false;
	}
	
	//Getters
	
	public int getPort(){
		return mPort;
	}
	
	public String getAdress(){
		return mAdress;
	}
	
	public Socket getSocket(){
		return mSocket;
	}
	
	public String getMessage(){
		return mMessage;
	}
	
	public boolean isConnected(){
		return mConnected;
	}
	//===============================================
	//private section
	//===============================================
	
	private String composeResponse(){
		String res = null;
		try {
			JSONObject objMsg = new JSONObject(mMessage);
			String method = objMsg.getString(RPCConst.TAG_METHOD);
			JSONObject objResp = new JSONObject();
			// get method name without component name
			if (method != null) {
				// get method name without component name
				method = method.substring(method.indexOf(".") + 1, method.length());
				try {
					switch (EMBMethods.valueOf(method)) {
					case registerComponent:
						objResp.put(RPCConst.TAG_ID, objMsg.get(RPCConst.TAG_ID));
						objResp.put(RPCConst.TAG_METHOD, objMsg.get(RPCConst.TAG_METHOD));
						objResp.put(RPCConst.TAG_RESULT, 100);
						break;
					case unregisterComponent:
						break;
					case subscribeTo:
						break;
					case unsubscribeFrom:
						break;
					default:
						break;
					}// switch
					res = objResp.toString();
				} catch (IllegalArgumentException e) {
					e.printStackTrace();
				}
			}
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		return res;
	}
	
	private int mPort = -1;
	private String mAdress = null;
	private Socket mSocket = null;
	private String mMessage = null;
	private boolean mConnected = false;

}
