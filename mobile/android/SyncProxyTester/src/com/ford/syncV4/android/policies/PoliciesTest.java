package com.ford.syncV4.android.policies;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Scanner;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.proxy.SyncProxyALM;

public class PoliciesTest {
	private static logAdapter _msgAdapter;
	private static SyncProxyALM _syncProxy;
	
	public static void runPoliciesTest(Context context) {
		_msgAdapter = SyncProxyTester.getMessageAdapter();
		_syncProxy = ProxyService.getProxyInstance();
		
		String url = new String();
		String jsonData = new String();
		String encodedSyncPDataReceived = new String();
		try {
			Scanner scanner = new Scanner(new FileReader("/sdcard/policiesRequest.txt"));
			url = scanner.nextLine();
			Log.e("TestApp", url);
			while (scanner.hasNextLine()) {
				jsonData += scanner.nextLine().replaceAll(" ", "");
				//jsonData += scanner.nextLine();
			}
			Log.e("TestApp", jsonData);
			scanner.close();
		} catch (Exception e) {
			String s = "Error reading policiesRequest.txt";
			_msgAdapter.logMessage(s, Log.ERROR, e, false);
			Toast.makeText(context, s, Toast.LENGTH_LONG).show();
		}
		
		encodedSyncPDataReceived = _syncProxy.sendEncodedSyncPDataToUrl(url, jsonData);
		/*
		encodedSyncPDataReceived = _syncProxy.sendEncodedSyncPDataToUrl(
				"http://applinkdev1.cloudapp.net/api/Ford", 
				"{\"data\":[\"RW5jb2R1U31uY1A=\"]}");
		*/
		
		if (encodedSyncPDataReceived != null) {
			_msgAdapter.logMessage(encodedSyncPDataReceived, Log.DEBUG, true);
			try {
				File out = new File("/sdcard/policiesResponse.txt");
				FileWriter writer = new FileWriter(out);
				writer.flush();
				writer.write(encodedSyncPDataReceived);
				writer.close();
			} catch (Exception e) {
				String s = "Error writing to policiesResponse.txt";
				_msgAdapter.logMessage(s, Log.ERROR, e, false);
				Toast.makeText(context, s, Toast.LENGTH_LONG).show();
			}
		} else {
			String s = "Error communicating with server";
			_msgAdapter.logMessage(s, Log.ERROR, false);
			Toast.makeText(context, s, Toast.LENGTH_LONG).show();
		}
	}
	
	/*
	public String sendEncodedSyncPDataToUrl(String urlString, String encodedSyncPData){
		try{
			final int CONNECTION_TIMEOUT = 30000; // in ms
			
			Vector<String> encodedSyncPDataReceived = new Vector<String>();
		
			// Form the JSON message to send to the cloud
			byte[] bytesToSend = encodedSyncPData.getBytes("UTF-8");
			
			// Send the Bytes to the Cloud and get the Response
			HttpParams httpParams = new BasicHttpParams();
			HttpConnectionParams.setConnectionTimeout(httpParams, CONNECTION_TIMEOUT);
			HttpConnectionParams.setSoTimeout(httpParams, CONNECTION_TIMEOUT);
			HttpClient client = new DefaultHttpClient(httpParams);
			HttpPost request = new HttpPost(urlString);
			request.setHeader("Content-type", "application/json");
			request.setEntity(new ByteArrayEntity(bytesToSend));
			HttpResponse response = client.execute(request);
			
			// If response is null, then return
			if(response == null){
				DebugTool.logError("Response from server returned null: ");
				return null;
			}
			
			String returnVal = new String();
			if (response.getStatusLine().getStatusCode() == 200) {
				Log.e("TestApp", "Status 200");
				// Convert the response to JSON
				returnVal = EntityUtils.toString(response.getEntity(), "UTF-8");
				JSONObject jsonResponse = new JSONObject(returnVal);
							
				// Create and send the encodedSyncPData message back to SYNC
				if(jsonResponse.get("data") instanceof JSONArray){
					JSONArray jsonArray = jsonResponse.getJSONArray("data");
					for(int i=0; i<jsonArray.length(); i++){
						if(jsonArray.get(i) instanceof String){
							encodedSyncPDataReceived.add(jsonArray.getString(i));
						}
					}
				} else if(jsonResponse.get("data") instanceof String){
					encodedSyncPDataReceived.add(jsonResponse.getString("data"));
				} else {
					DebugTool.logError("sendEncodedSyncPDataToUrl: Data in JSON Object neither an array nor a string.");
					// Exit method
					return null;
				}
			} else if (response.getStatusLine().getStatusCode() == 500) returnVal = "Error 500";
			else returnVal = "Unknown Error";
			
			// Send new encodedSyncPDataRequest to SYNC
			EncodedSyncPData encodedSyncPDataRequest = RPCRequestFactory.buildEncodedSyncPData(encodedSyncPDataReceived, getPoliciesReservedCorrelationID());
			if(getIsConnected()){
				sendRPCRequestPrivate(encodedSyncPDataRequest);
			}
			return returnVal;
		} catch (SyncException e){
			DebugTool.logError("sendEncodedSyncPDataToUrl: Could not get data from JSONObject received.", e);
		} catch (JSONException e) {
			DebugTool.logError("sendEncodedSyncPDataToUrl: JSONException: ", e);
		} catch (UnsupportedEncodingException e) {
			DebugTool.logError("sendEncodedSyncPDataToUrl: Could not encode string.", e);
		} catch (ProtocolException e) {
			DebugTool.logError("sendEncodedSyncPDataToUrl: Could not set request method to post.", e);
		} catch (MalformedURLException e) {
			DebugTool.logError("sendEncodedSyncPDataToUrl: URL Exception when sending EncodedSyncPData to an external server.", e);
		} catch (IOException e) {
			DebugTool.logError("sendEncodedSyncPDataToUrl: IOException: ", e);
		} catch (Exception e) {
			DebugTool.logError("sendEncodedSyncPDataToUrl: Unexpected Exception: ", e);
		}
		return null;
	}
	*/
}