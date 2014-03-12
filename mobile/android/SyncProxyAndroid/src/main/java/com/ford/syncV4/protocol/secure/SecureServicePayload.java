package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import com.ford.syncV4.protocol.BinaryFrameHeader;

/**
 * This class build a payload byte array for the Secure Service
 */
public class SecureServicePayload {

    /**
     * Binary header of the Secure Service protocol
     */
    private BinaryFrameHeader mBinaryFrameHeader;

    /**
     * Data of the Secure Service
     */
    private byte[] mData;

    /**
     * @return Binary header of the Secure Service protocol
     */
    public BinaryFrameHeader getBinaryFrameHeader() {
        return mBinaryFrameHeader;
    }

    /**
     * Set Binary header of the Secure Service protocol
     * @param mBinaryFrameHeader {@link com.ford.syncV4.protocol.BinaryFrameHeader}
     */
    public void setBinaryFrameHeader(BinaryFrameHeader mBinaryFrameHeader) {
        this.mBinaryFrameHeader = mBinaryFrameHeader;
    }

    /**
     * @return Data of the Secure Service
     */
    public byte[] getData() {
        return mData;
    }

    /**
     * Set Data of the Secure Service
     * @param mData array of the bytes
     */
    public void setData(byte[] mData) {
        this.mData = mData;
    }
}