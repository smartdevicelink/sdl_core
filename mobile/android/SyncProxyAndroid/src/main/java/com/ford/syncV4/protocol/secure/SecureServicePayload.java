package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import android.util.Log;

import com.ford.syncV4.protocol.BinaryFrameHeader;
import com.ford.syncV4.service.secure.ProtectServiceResponse;

/**
 * This class build a payload byte array for the Secure Service
 */
public class SecureServicePayload {

    private static final String TAG = "SecureServicePayload";

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

    /**
     * @return Id of the Secure Service API function
     */
    public int getFunctionId() {
        if (mBinaryFrameHeader != null) {
            return mBinaryFrameHeader.getFunctionID();
        }
        return  -1;
    }

    /**
     * @return Seq number of the request
     */
    public int getSeqNumber() {
        if (mBinaryFrameHeader != null) {
            return mBinaryFrameHeader.getCorrID();
        }
        return  -1;
    }

    /**
     * @return {@link com.ford.syncV4.service.secure.ProtectServiceResponse} of the Start securing
     * service request
     */
    public ProtectServiceResponse getStartServiceResponse() {
        if (mData != null) {
            int result = mData[0];
            switch (result) {
                case 1:
                    return ProtectServiceResponse.SUCCESS;
                case 2:
                    return ProtectServiceResponse.PENDING;
                case 3:
                    return ProtectServiceResponse.SERVICE_ALREADY_PROTECTED;
                case 4:
                    return ProtectServiceResponse.SERVICE_NOT_FOUND;
                case 255:
                    return ProtectServiceResponse.INTERNAL_ERROR;
            }
        }
        return ProtectServiceResponse.UNKNOWN;
    }
}