package com.ford.syncV4.net;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 20.03.14
 * Time: 17:11
 */

import android.util.Log;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.util.Base64;

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

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.util.Vector;

/**
 * This class provides a functionality to send and receive SyncP Data over network
 */
public class SyncPDataSender {

    /**
     * This interface provides callback function to the {@link com.ford.syncV4.net.SyncPDataSender}
     * object
     */
    public interface SyncPDataSenderCallback {

        /**
         * Call when download is complete
         * @param rpcRequest {@link com.ford.syncV4.proxy.RPCRequest} object as result
         */
        void onComplete(RPCRequest rpcRequest);

        /**
         * Call when error occur
         * @param message error message
         */
        void onError(String message);
    }

    private static final String TAG = SyncPDataSender.class.getSimpleName();

    private int mPoliciesCorrelationId;

    /**
     * Constructor
     * @param policiesCorrelationId correlation id of the {@link com.ford.syncV4.proxy.rpc.SyncPData} and
     *                              {@link com.ford.syncV4.proxy.rpc.EncodedSyncPData} requests
     */
    public SyncPDataSender(int policiesCorrelationId) {
        mPoliciesCorrelationId = policiesCorrelationId;
    }

    /**
     * Sends PData as a raw bytes array to provided URL. This method encode data into Base64 and
     * and set it in the vector
     *
     * @param url      URL to perform HTTP request
     * @param data     array of the bytes (SyncP packet)
     * @param timeOut  time out in milliseconds to perform HTTP request
     * @param callback {@link com.ford.syncV4.net.SyncPDataSender.SyncPDataSenderCallback}
     *                 implementation
     */
    public void sendPData(String url, byte[] data, int timeOut, SyncPDataSenderCallback callback) {
        Vector<String> vector = new Vector<String>();

        // base64 encode the binary syncp packet before sending to cloud
        vector.add(Base64.encodeBytes(data));

        send(url, vector, timeOut, false, callback);
    }

    /**
     * Sends Encoded PData packets as an array to provided URL.
     *
     * @param url      URL to perform HTTP request
     * @param data     vector of SyncP packets
     * @param timeOut  time out in milliseconds to perform HTTP request
     * @param callback {@link com.ford.syncV4.net.SyncPDataSender.SyncPDataSenderCallback}
     *                  implementation
     */
    public void sendEncodedPData(String url, Vector<String> data, int timeOut,
                                 SyncPDataSenderCallback callback) {
        send(url, data, timeOut, true, callback);
    }

    /**
     * <b>This method is for Unit Testing only</b><br/>
     * Sends Single Encoded PData packet as a PData packet to provided URL.
     *
     * @param url      URL to perform HTTP request
     * @param data     encoded Base 64 packet
     * @param timeOut  time out in milliseconds to perform HTTP request
     * @param callback {@link com.ford.syncV4.net.SyncPDataSender.SyncPDataSenderCallback}
     *                 implementation
     */
    protected void sendPData(String url, String data, int timeOut, SyncPDataSenderCallback callback) {
        Vector<String> vector = new Vector<String>();

        // base64 encoded SyncP packet
        vector.add(data);

        send(url, vector, timeOut, false, callback);
    }

    private void send(String url, Vector<String> data, int timeOut, boolean isEncoded,
                      SyncPDataSenderCallback callback) {
        Log.i(TAG, "Try to send SyncP Data, encoded:" + isEncoded);
        final int CONNECTION_TIMEOUT = timeOut * 1000; // in ms
        try {
            // Form the JSON message to send to the cloud
            JSONArray jsonArrayOfSyncPPackets = new JSONArray();
            for (String string : data) {
                jsonArrayOfSyncPPackets.put(string);
            }
            Log.i(TAG, "SyncP JSONArray length: " + jsonArrayOfSyncPPackets.length());

            JSONObject jsonObjectToSendToServer = new JSONObject();
            jsonObjectToSendToServer.put("data", jsonArrayOfSyncPPackets);
            Log.i(TAG, "SyncP JSONObject: " + jsonObjectToSendToServer.toString());

            // Convert a JSON message to bytes array
            String valid_json = jsonObjectToSendToServer.toString().replace("\\", "");
            byte[] bytesToSend = valid_json.getBytes("UTF-8");
            Log.i(TAG, "SyncP bytes length: " + bytesToSend.length);

            // Send the Bytes to the Cloud and get the Response
            HttpParams httpParams = new BasicHttpParams();

            // Set the timeout in milliseconds until a connection is established.
            // The default value is zero, that means the timeout is not used.
            HttpConnectionParams.setConnectionTimeout(httpParams, CONNECTION_TIMEOUT);

            // Set the default socket timeout (SO_TIMEOUT)
            // in milliseconds which is the timeout for waiting for data.
            HttpConnectionParams.setSoTimeout(httpParams, CONNECTION_TIMEOUT);
            HttpClient client = new DefaultHttpClient(httpParams);
            HttpPost request = new HttpPost(url);
            request.setHeader("Content-type", "application/json");
            request.setEntity(new ByteArrayEntity(bytesToSend));
            HttpResponse response = client.execute(request);

            Log.i(TAG, "SyncP data sent and received");

            // If response is null, then return
            if (response == null) {
                Log.e(TAG, "SyncP response from server null");
                return;
            }

            Vector<String> encodedSyncPDataReceived = new Vector<String>();
            int statusCode = response.getStatusLine().getStatusCode();
            Log.d(TAG, "SyncP response code:" + statusCode);
            if (statusCode == 200) {
                // Convert the response to JSON
                JSONObject jsonResponse = new JSONObject(EntityUtils.toString(
                        response.getEntity(), "UTF-8"));

                if (jsonResponse.get("data") instanceof JSONArray) {
                    JSONArray jsonArray = jsonResponse.getJSONArray("data");
                    for (int i = 0; i < jsonArray.length(); i++) {
                        if (jsonArray.get(i) instanceof String) {
                            encodedSyncPDataReceived.add(jsonArray.getString(i));
                            //Log.i(TAG, "jsonArray.getString(i): " + jsonArray.getString(i));
                        }
                    }
                } else if (jsonResponse.get("data") instanceof String) {
                    encodedSyncPDataReceived.add(jsonResponse.getString("data"));
                    //Log.i(TAG, "jsonResponse.getString(data): " + jsonResponse.getString("data"));
                } else {
                    Log.e(TAG, "Data in JSON Object neither an array nor a string.");
                    return;
                }

                if (isEncoded) {
                    // Send new encodedSyncPDataRequest to SYNC
                    EncodedSyncPData encodedSyncPDataRequest = RPCRequestFactory
                            .buildEncodedSyncPData(encodedSyncPDataReceived, mPoliciesCorrelationId);

                    callback.onComplete(encodedSyncPDataRequest);
                } else {
                    //convert encoded syncp packet to binary
                    byte[] syncpPacket = encodedSyncPDataReceived.firstElement().getBytes();

                    // Send new binary syncp data to SYNC
                    SyncPData syncPDataRequest = RPCRequestFactory.buildSyncPData(syncpPacket,
                            mPoliciesCorrelationId);

                    callback.onComplete(syncPDataRequest);
                }
            } else {
                callback.onError("SyncP response code:" + statusCode);
            }

        } catch (JSONException e) {
            Log.e(TAG, "JSONException: ", e);
            callback.onError("JSONException:" + e.getMessage());
        } catch (UnsupportedEncodingException e) {
            Log.e(TAG, "Could not encode string.", e);
            callback.onError("Could not encode string:" + e.getMessage());
        } catch (ProtocolException e) {
            Log.e(TAG, "Could not set request method to post.", e);
            callback.onError("Could not set request method to post:" + e.getMessage());
        } catch (MalformedURLException e) {
            Log.e(TAG, "URL Exception when sending EncodedSyncPData to an external server.", e);
            callback.onError("URL Exception when sending EncodedSyncPData to an external server:" +
                    e.getMessage());
        } catch (IOException e) {
            Log.e(TAG, "IOException: " + e);
            callback.onError("IOException:" + e.getMessage());
        } catch (Exception e) {
            Log.e(TAG, "Unexpected Exception: ", e);
            callback.onError("Unexpected Exception:" + e.getMessage());
        }
    }
}