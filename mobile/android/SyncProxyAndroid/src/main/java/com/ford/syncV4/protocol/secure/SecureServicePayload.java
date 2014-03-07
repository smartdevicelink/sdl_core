package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

/**
 * This class build a payload byte array for the Secure Service
 */
public class SecureServicePayload {

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

    /**
     * Constructor
     *
     * @param functionId   API function identifier
     * @param reqSeqNumber Request sequential number
     * @param data bytes array of the data to be sent with payload
     */
    public SecureServicePayload(byte functionId, int reqSeqNumber, byte[] data) {

        if (data == null) {
            throw new IllegalArgumentException(SecureServicePayload.class.getSimpleName() +
                    " data can't be null");
        }

        mFunctionId = functionId;
        mReqSeqNumber = reqSeqNumber;
        mData = data;
    }

    /**
     * Assemble payload to array of the bytes
     *
     * @return
     */
    public byte[] toBytes() {

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byteArrayOutputStream.write(mFunctionId);

        byte[] reqSeqNumberBytes = ByteBuffer.allocate(4).putInt(mReqSeqNumber).array();
        byteArrayOutputStream.write(reqSeqNumberBytes, 0, 4);

        byteArrayOutputStream.write(mData, 0, mData.length);

        return byteArrayOutputStream.toByteArray();
    }
}