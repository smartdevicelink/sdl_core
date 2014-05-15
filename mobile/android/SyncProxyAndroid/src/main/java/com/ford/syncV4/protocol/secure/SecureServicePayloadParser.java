package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import android.util.Log;

import com.ford.syncV4.protocol.BinaryFrameHeader;
import com.ford.syncV4.protocol.ProtocolConst;

/**
 * This class helps to parse raw data of the Secure Service response
 */
public class SecureServicePayloadParser {

    private static final String TAG = "SecureServicePayloadParser";

    public SecureServicePayloadParser() {

    }

    /**
     * Parse raw data of the Secure Service response into
     * {@link com.ford.syncV4.protocol.secure.SecureServicePayload} object
     *
     * @param data bytes array data
     *
     * @return {@link com.ford.syncV4.protocol.secure.SecureServicePayload}
     */
    public SecureServicePayload parse(byte[] data) {
        SecureServicePayload secureServicePayload = new SecureServicePayload();

        if (data == null) {
            Log.w(TAG, SecureServicePayloadParser.class.getSimpleName() + " parse(), data is NULL");
            return secureServicePayload;
        }
        if (data.length == 0) {
            Log.w(TAG, SecureServicePayloadParser.class.getSimpleName() + " parse(), data is empty");
            return secureServicePayload;
        }
        if (data.length < ProtocolConst.PROTOCOL_V2_HEADER_SIZE) {
            Log.w(TAG, SecureServicePayloadParser.class.getSimpleName() + " parse(), data size " +
                    "is wrong");
            return secureServicePayload;
        }

        secureServicePayload.setBinaryFrameHeader(BinaryFrameHeader.parseBinaryHeader(data));
        secureServicePayload.setData(BinaryFrameHeader.parseSecureServiceBinaryHeaderData(data));

        return secureServicePayload;
    }
}