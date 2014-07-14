package com.ford.syncV4.protocol.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 1:24 PM
 */

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.BinaryFrameHeader;
import com.ford.syncV4.service.secure.SecurityInternalError;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.Objects;

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
            return SecurityInternalError.getError(result);
        }
        return SecurityInternalError.UNKNOWN;
    }

    public Hashtable<String, Object> getSecureErrorDescription() {
        JsonRPCMarshaller marshaller = new JsonRPCMarshaller();
        Hashtable<String, Object> error =  marshaller.unmarshall(mBinaryFrameHeader.getJsonData());
        return error;
    }
}