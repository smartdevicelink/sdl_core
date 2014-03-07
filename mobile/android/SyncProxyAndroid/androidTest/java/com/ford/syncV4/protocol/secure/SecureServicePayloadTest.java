package com.ford.syncV4.protocol.secure;

import com.ford.syncV4.protocol.enums.ServiceType;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 3:32 PM
 */
public class SecureServicePayloadTest extends TestCase {

    private static final String TAG = "SecureServicePayloadTest";

    private int mReqSeqNumber = 123;
    private byte mProtectServiceRequestId = (byte) 1;

    public void testCreatePayloadNotNull() {
        SecureServicePayload secureServicePayload = new SecureServicePayload(
                mProtectServiceRequestId, mReqSeqNumber, new byte[0]);

        assertNotNull(secureServicePayload.toBytes());
    }

    public void testCreatePayloadWithNullData() {
        SecureServicePayload secureServicePayload = null;
        try {
            secureServicePayload = new SecureServicePayload(mProtectServiceRequestId,
                    mReqSeqNumber, null);
            fail();
        } catch (IllegalArgumentException e) {
            /* expected */
        }
    }

    public void testPayloadDataLengthWithDataPayload() {
        ServiceType serviceType = ServiceType.Audio_Service;
        byte[] payloadData = new byte[1];
        payloadData[0] = serviceType.getValue();

        SecureServicePayload secureServicePayload = new SecureServicePayload(
                mProtectServiceRequestId, mReqSeqNumber, payloadData);

        // 1 byte is mProtectServiceRequestId
        // 4 bytes is mReqSeqNumber
        // 1 byte is payloadData itself
        int bytesNumber = 6;

        assertEquals(bytesNumber, secureServicePayload.toBytes().length);
    }

    public void testPayloadDataLengthWithEmptyPayload() {
        byte[] payloadData = new byte[0];

        SecureServicePayload secureServicePayload = new SecureServicePayload(
                mProtectServiceRequestId, mReqSeqNumber, payloadData);

        // 1 byte is mProtectServiceRequestId
        // 4 bytes is mReqSeqNumber
        // 0 byte is payloadData itself
        int bytesNumber = 5;

        assertEquals(bytesNumber, secureServicePayload.toBytes().length);
    }
}