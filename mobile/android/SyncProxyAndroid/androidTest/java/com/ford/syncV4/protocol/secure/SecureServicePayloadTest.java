package com.ford.syncV4.protocol.secure;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 3:32 PM
 * 
 */
public class SecureServicePayloadTest extends TestCase {

    private static final String TAG = "SecureServicePayloadTest";

    private int mReqSeqNumber = 123;
    private byte mProtectServiceRequestId = (byte) 1;

    // TODO : Obsolet Test Cases

    /*public void testCreatePayloadNotNull() {
        SecureServicePayload secureServicePayload = new SecureServicePayload(
                SecureServiceQueryType.QueryType.REQUEST,
                mProtectServiceRequestId, mReqSeqNumber, new byte[0]);

        assertNotNull(secureServicePayload.toBytes());
    }

    public void testCreatePayloadWithNullData() {
        SecureServicePayload secureServicePayload = null;
        try {
            secureServicePayload = new SecureServicePayload(
                    SecureServiceQueryType.QueryType.REQUEST,
                    mProtectServiceRequestId,
                    mReqSeqNumber, null);
            fail();
        } catch (IllegalArgumentException e) {
            *//* expected *//*
        }
    }

    public void testPayloadDataLengthWithDataPayload() {
        ServiceType serviceType = ServiceType.Audio_Service;
        byte[] payloadData = new byte[1];
        payloadData[0] = serviceType.getValue();

        SecureServicePayload secureServicePayload = new SecureServicePayload(
                SecureServiceQueryType.QueryType.REQUEST,
                mProtectServiceRequestId, mReqSeqNumber, payloadData);

        // 1 byte query_type
        // 2 - 4 bytes are API function identifier
        // 5 - 8 bytes are request sequential number
        // 9 - 12 bytes are reserved
        // 13 byte is data
        int bytesNumber = 13;

        assertEquals(bytesNumber, secureServicePayload.toBytes().length);
    }

    public void testPayloadDataLengthWithEmptyPayload() {
        byte[] payloadData = new byte[0];

        SecureServicePayload secureServicePayload = new SecureServicePayload(
                SecureServiceQueryType.QueryType.REQUEST,
                mProtectServiceRequestId, mReqSeqNumber, payloadData);

        // 1 byte query_type
        // 2 - 4 bytes are API function identifier
        // 5 - 8 bytes are request sequential number
        // 9 - 12 bytes are reserved
        int bytesNumber = 12;

        assertEquals(bytesNumber, secureServicePayload.toBytes().length);
    }*/
}