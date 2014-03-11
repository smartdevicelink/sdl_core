package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import android.util.Log;

import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.ByteEnumer;

import java.nio.ByteBuffer;

/**
 * This class build a payload byte array for the Secure Service
 */
public class SecureServicePayloadParser {

    private static final String TAG = "SecureServicePayloadParser";

    /**
     * Minimum data size according to API
     */
    private static final int PAYLOAD_MIN_SIZE = 9;

    /**
     * API function identifier
     */
    private int mFunctionId;

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
        if (data == null) {
            Log.w(TAG, SecureServicePayloadParser.class.getSimpleName() + " parse(), data is NULL");
            return;
        }
        if (data.length == 0) {
            Log.w(TAG, SecureServicePayloadParser.class.getSimpleName() + " parse(), data is empty");
            return;
        }
        if (data.length < PAYLOAD_MIN_SIZE) {
            Log.w(TAG, SecureServicePayloadParser.class.getSimpleName() + " parse(), data size is wrong");
            return;
        }

        Log.d(TAG, "Parse data size:" + data.length);
        Log.d(TAG, "Parse query id:" + (data[0] & 0x0F));

        mFunctionId =  ((int)data[3] << 16) & ((int)data[2] << 8) & (int)data[1];
        Log.d(TAG, "Parse API function Id:" + mFunctionId);

        mReqSeqNumber = BitConverter.intFromByteArray(data, 4);
        Log.d(TAG, "Parse request sequence number:" + Integer.toHexString(mReqSeqNumber));

        //int dataSize = BitConverter.intFromByteArray(data, 5);
        //Log.d(TAG, "Parse data size:" + dataSize);


    }
}