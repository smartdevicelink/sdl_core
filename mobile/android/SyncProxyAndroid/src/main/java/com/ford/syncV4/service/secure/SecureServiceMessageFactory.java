package com.ford.syncV4.service.secure;

import android.util.Log;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.secure.SecureServicePayload;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 10:37 AM
 */
public class SecureServiceMessageFactory {

    private static final String TAG = "SecureServiceMessageFactory";

    private static final byte PROTECT_SERVICE_REQUEST_ID = (byte) 1;
    private static final int REQ_SEQ_NUMBER_FAKE = 123;

    /**
     * Build a {@link com.ford.syncV4.protocol.ProtocolMessage} for the start securing of the
     * provided service
     *
     * @param serviceType a type of the service to be secured
     * @param sessionId Id of the session
     *
     * @return {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public static ProtocolMessage buildProtectServiceRequest(byte sessionId,
                                                             ServiceType serviceType) {

        // TODO : To be implemented

        ProtocolMessage protocolMessage = new ProtocolMessage();
        protocolMessage.setSessionID(sessionId);
        protocolMessage.setSessionType(ServiceType.Secure_Service);
        protocolMessage.setFunctionID(0);
        protocolMessage.setCorrID(0);

        byte[] payloadData = new byte[1];
        payloadData[0] = serviceType.getValue();

        SecureServicePayload secureServicePayload = new SecureServicePayload(
                PROTECT_SERVICE_REQUEST_ID, REQ_SEQ_NUMBER_FAKE, payloadData);

        Log.d(TAG, "BuildProtectServiceRequest, secure service payload data length:" +
                secureServicePayload.toBytes().length);

        protocolMessage.setData(secureServicePayload.toBytes());

        return protocolMessage;
    }

    /**
     * Build a {@link com.ford.syncV4.protocol.ProtocolMessage} for the handshake procedure
     *
     * @param serviceType a type of the service to be secured
     * @param sessionId Id of the session
     *
     * @return {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public static ProtocolMessage buildHandshakeRequest(byte sessionId,
                                                        ServiceType serviceType) {

        // TODO : To be implemented

        ProtocolMessage protocolMessage = new ProtocolMessage();
        protocolMessage.setSessionID(sessionId);
        protocolMessage.setSessionType(ServiceType.Secure_Service);
        protocolMessage.setFunctionID(0);
        protocolMessage.setCorrID(0);
        protocolMessage.setData(new byte[0]);

        return protocolMessage;
    }
}