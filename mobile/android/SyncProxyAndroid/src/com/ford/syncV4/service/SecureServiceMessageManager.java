package com.ford.syncV4.service;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 4:49 PM
 */

import com.ford.syncV4.protocol.ProtocolMessage;

/**
 * This class provides main functionality to manage (process, etc ...) SecureService messages
 */
public class SecureServiceMessageManager {

    /**
     * Process {@link com.ford.syncV4.protocol.ProtocolMessage} in order to extract secured data
     *
     * @param protocolMessage protocol message
     * @param callback a callback function
     */
    public void processProtocolSecureMessage(ProtocolMessage protocolMessage,
                                             ISecureServiceMessageCallback callback) {

        // TODO : To be implement

        callback.onProtectServiceResponse(ProtectServiceResponse.SUCCESS);
    }
}