package com.ford.syncV4.service.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 12:43 PM
 */

import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * This interface describes callback functions which are recognized by Secure Service messages
 * processor
 */
public interface ISecureServiceMessageCallback {

    /**
     * Call this callback when response for the
     * {@link com.ford.syncV4.service.secure.SecureServiceMessageFactory#buildProtectServiceRequest(byte, com.ford.syncV4.protocol.enums.ServiceType)}
     * is received
     *
     * @param result {@link ProtectServiceResponse}
     * @param serviceType a type of the Service
     */
    public void onProtectServiceResponse(ProtectServiceResponse result, ServiceType serviceType);

    /**
     * Call this callback when response for the
     * {@link com.ford.syncV4.service.secure.SecureServiceMessageFactory#buildHandshakeRequest(byte, com.ford.syncV4.protocol.enums.ServiceType)}
     * is received
     *
     * @param data handshake server to client data
     */
    public void onHandshakeResponse(byte[] data);
}