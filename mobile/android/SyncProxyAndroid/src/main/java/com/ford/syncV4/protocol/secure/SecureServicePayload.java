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
     * Byte for compatibility with RPC request
     */
    private static final byte COMPATIBILITY_BYTE = 0x00;

    /**
     * Reserved bytes
     */
    private static final int RESERVED_BYTES = 0x000000;

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

    /**
     * Constructor
     *
     * @param functionId   API function identifier
     * @param reqSeqNumber Request sequential number
     * @param data         bytes array of the data to be sent with payload
     */
    public SecureServicePayload(int functionId, int reqSeqNumber, byte[] data) {

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

        byteArrayOutputStream.write(COMPATIBILITY_BYTE);

        byte[] functionIdBytes = new byte[3];
        functionIdBytes[0] = (byte) (mFunctionId & 0x000000FF);
        functionIdBytes[1] = (byte) ((mFunctionId & 0x0000FF00) >> 8);
        functionIdBytes[2] = (byte) ((mFunctionId & 0x00FF0000) >> 16);
        byteArrayOutputStream.write(functionIdBytes, 0, 3);

        byte[] reqSeqNumberBytes = ByteBuffer.allocate(4).putInt(mReqSeqNumber).array();
        byteArrayOutputStream.write(reqSeqNumberBytes, 0, 4);

        byte[] reservedBytes = ByteBuffer.allocate(4).putInt(RESERVED_BYTES).array();
        byteArrayOutputStream.write(reservedBytes, 0, 4);

        byteArrayOutputStream.write(mData, 0, mData.length);

        return byteArrayOutputStream.toByteArray();
    }
}