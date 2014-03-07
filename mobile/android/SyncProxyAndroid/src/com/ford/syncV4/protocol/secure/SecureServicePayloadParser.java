package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import android.util.Log;

/**
 * This class build a payload byte array for the Secure Service
 */
public class SecureServicePayloadParser {

    private static final String TAG = "SecureServicePayloadParser";

    /**
     * API function identifier
     */
    private byte mFunctionId;

    /**
     * Request sequential number
     */
    private int mReqSeqNumber;

    /**
     * Data to be sent with payload
     */
    private byte[] mData;

    public SecureServicePayloadParser() {


    }

    public void parse(byte[] data) {
        Log.d(TAG, "Parse API function Id:" + data[0]);
    }
}