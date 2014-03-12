package com.ford.syncV4.service.secure;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/5/14
 * Time: 4:49 PM
 */

import android.util.Log;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.secure.SecureServicePayload;
import com.ford.syncV4.protocol.secure.SecureServicePayloadParser;

/**
 * This class provides main functionality to manage (process, etc ...) SecureService messages
 */
public class SecureServiceMessageManager {

    private static final String TAG = "SecureServiceMessageManager";

    /**
     * Callback handler
     */
    private ISecureServiceMessageCallback mMessageCallback;

    /**
     * Set a listener for the message parsing results
     *
     * @param value an instance of the {@link ISecureServiceMessageCallback}
     */
    public void setMessageCallback(ISecureServiceMessageCallback value) {
        mMessageCallback = value;
    }

    /**
     * Process {@link com.ford.syncV4.protocol.ProtocolMessage} in order to extract secured data
     *
     * @param protocolMessage protocol message
     */
    public void processMessage(ProtocolMessage protocolMessage) {

        if (mMessageCallback == null) {
            throw new NullPointerException(SecureServiceMessageManager.class.getSimpleName() +
                    " processMessage, SecureServiceMessageCallback must be set before process" +
                    " message");
        }

        Log.d(TAG, "Process Secure msg");

        SecureServicePayloadParser secureServicePayloadParser = new SecureServicePayloadParser();
        SecureServicePayload secureServicePayload =
                secureServicePayloadParser.parse(protocolMessage.getData());

        // ServiceType could be obtained from ProtocolMessage
        ServiceType serviceType = protocolMessage.getServiceType();

        //mMessageCallback.onProtectServiceResponse(ProtectServiceResponse.SUCCESS, serviceType);
    }
}