package com.ford.syncV4.service.secure;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/6/14
 * Time: 10:37 AM
 */
public class SecureServiceMessageFactory {

    /**
     * Build a {@link com.ford.syncV4.protocol.ProtocolMessage} for the start securing of the
     * provided service
     *
     * @param serviceType a type of the service to be secured
     *
     * @return {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public static ProtocolMessage buildProtectServiceRequest(ServiceType serviceType) {

        // TODO : To be implemented
        return new ProtocolMessage();
    }

    /**
     * Build a {@link com.ford.syncV4.protocol.ProtocolMessage} for the handshake procedure
     *
     * @param serviceType a type of the service to be secured
     *
     * @return {@link com.ford.syncV4.protocol.ProtocolMessage}
     */
    public static ProtocolMessage buildHandshakeRequest(ServiceType serviceType) {

        // TODO : To be implemented
        return new ProtocolMessage();
    }
}