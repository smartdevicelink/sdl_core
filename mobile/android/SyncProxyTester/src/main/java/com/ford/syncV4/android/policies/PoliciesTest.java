package com.ford.syncV4.android.policies;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.util.Scanner;
import java.util.Vector;

import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.util.DebugTool;
import com.ford.syncV4.exception.SyncException;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class PoliciesTest {

    private static final String TAG = "PoliciesTest";
    private static LogAdapter sMsgAdapter;
    private static ProxyService sProxyService;

    public static void runPoliciesTest(ProxyService syncProxy, LogAdapter logAdapter) {
        if (logAdapter == null) {
            throw new NullPointerException(PoliciesTest.class.getSimpleName() + " LogAdapter can " +
                    "not be null");
        }

        sProxyService = syncProxy;
        sMsgAdapter = logAdapter;

        String url = "";
        String jsonData = "";
        String encodedSyncPDataReceived;
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
            sMsgAdapter.logMessage("Error reading policiesRequest.txt", Log.ERROR, e, true);
        }

        encodedSyncPDataReceived = sendEncodedSyncPDataToUrl(url, jsonData, 10);
        /*
		encodedSyncPDataReceived = sProxyService.sendEncodedSyncPDataToUrl(
				"http://applinkdev1.cloudapp.net/api/Ford", 
				"{\"data\":[\"RW5jb2R1U31uY1A=\"]}");
		*/

        if (encodedSyncPDataReceived != null) {
            sMsgAdapter.logMessage(encodedSyncPDataReceived, Log.DEBUG, true);
            try {
                File out = new File("/sdcard/policiesResponse.txt");
                FileWriter writer = new FileWriter(out);
                writer.flush();
                writer.write(encodedSyncPDataReceived);
                writer.close();
            } catch (Exception e) {
                sMsgAdapter.logMessage("Error writing to policiesResponse.txt", Log.ERROR, e, true);
            }
        } else sMsgAdapter.logMessage("Error communicating with server", Log.ERROR, true);
    }

    public static String sendEncodedSyncPDataToUrl(String urlString, String encodedSyncPData, Integer timeout) {
        try {
            final int CONNECTION_TIMEOUT = timeout * 1000; // in ms

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
		
			/*
			//todo: write to ui
			
			//public boolean post (Runnable action)
			mImageView.post(new Runnable() {
                public void run() {
                    mImageView.setImageBitmap(bitmap);
                }
            });
			*/

            // If response is null, then return
            if (response == null) {
                Log.e(TAG, "Response from server returned null: ");
                return null;
            }

            String returnVal;
            if (response.getStatusLine().getStatusCode() == 200) {
                Log.e(TAG, "Status 200");
                // Convert the response to JSON
                returnVal = EntityUtils.toString(response.getEntity(), "UTF-8");
                JSONObject jsonResponse = new JSONObject(returnVal);

                // Create and send the encodedSyncPData message back to SYNC
                if (jsonResponse.get("data") instanceof JSONArray) {
                    JSONArray jsonArray = jsonResponse.getJSONArray("data");
                    for (int i = 0; i < jsonArray.length(); i++) {
                        if (jsonArray.get(i) instanceof String) {
                            encodedSyncPDataReceived.add(jsonArray.getString(i));
                        }
                    }
                } else if (jsonResponse.get("data") instanceof String) {
                    encodedSyncPDataReceived.add(jsonResponse.getString("data"));
                } else {
                    Log.e(TAG, "sendEncodedSyncPDataToUrl: Data in JSON Object neither an array nor a string.");
                    // Exit method
                    return null;
                }

            } else if (response.getStatusLine().getStatusCode() == 500) {
                returnVal = "Error 500";
            } else {
                returnVal = "Unknown Error";
            }

            //todo: write to ui

            // Send new encodedSyncPDataRequest to SYNC
            EncodedSyncPData encodedSyncPDataRequest = RPCRequestFactory.buildEncodedSyncPData(encodedSyncPDataReceived, 65535);

            //if(getIsConnected() {
			
			/*
			 * // Send new encodedSyncPDataRequest to SYNC
				if (getIsConnected()) {
					sendRPCRequestPrivate(encodedSyncPDataRequest);
				}
			 */
            if (sProxyService != null) {
                //sendRPCRequestPrivate(encodedSyncPDataRequest);
                sProxyService.syncProxySendRPCRequest(encodedSyncPDataRequest);
            }
            return returnVal;
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
}