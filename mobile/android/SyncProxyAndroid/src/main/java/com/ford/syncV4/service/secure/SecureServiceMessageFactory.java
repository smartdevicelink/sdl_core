package com.ford.syncV4.service.secure;

import com.ford.syncV4.protocol.ProtocolConst;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 10:37 AM
 */
public class SecureServiceMessageFactory {

    private static final String TAG = "SecureServiceMessageFactory";


    public static ProtocolMessage buildHandshakeRequest(byte sessionId, byte[] payloadData) {

        ProtocolMessage protocolMessage = new ProtocolMessage();
        protocolMessage.setSessionID(sessionId);
        protocolMessage.setVersion((byte) 2);
        protocolMessage.setRPCType(ProtocolMessage.RPCTYPE_NOTIFICATION);
        protocolMessage.setSessionType(ServiceType.Heartbeat);
        protocolMessage.setFunctionID(ProtocolConst.SEND_HANDSHAKE_ID);
        protocolMessage.setData(payloadData);
        return protocolMessage;
    }
}