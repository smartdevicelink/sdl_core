package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import com.ford.syncV4.protocol.BinaryFrameHeader;
import com.ford.syncV4.service.secure.SecurityInternalError;

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

    public SecurityInternalError getSecureError() {
        if (mData != null) {
            int result = mBinaryFrameHeader.getBulkData()[0];
            switch (result) {
                case 0x0:
                    return SecurityInternalError.ERROR_SUCCESS;
                case 0x1:
                    return SecurityInternalError.ERROR_INVALID_QUERY_SIZE;
                case 0x2:
                    return SecurityInternalError.ERROR_INVALID_QUERY_ID;
                case 0x3:
                    return SecurityInternalError.ERROR_NOT_SUPPORTED;
                case 0x4:
                    return SecurityInternalError.ERROR_SERVICE_ALREADY_PROTECTED;
                case 0x5:
                    return SecurityInternalError.ERROR_CREATE_SLL;
                case 0x6:
                    return SecurityInternalError.ERROR_CONNECTION_NOT_FOUND;
                case 0x7:
                    return SecurityInternalError.ERROR_SESSION_NOT_FOUND;
                case 0x8:
                    return SecurityInternalError.ERROR_SERVICE_NOT_PROTECTED;
                case 0xF0:
                    return SecurityInternalError.ERROR_SSL_INVALID_DATA;
                case 0xFF:
                    return SecurityInternalError.ERROR_UNKWOWN_INTERNAL_ERROR;

            }
        }
        return SecurityInternalError.UNKNOWN;
    }

}