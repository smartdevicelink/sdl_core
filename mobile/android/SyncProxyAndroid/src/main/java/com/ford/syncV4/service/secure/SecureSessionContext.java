package com.ford.syncV4.service.secure;

import com.ford.syncV4.protocol.secure.secureproxy.IHandshakeDataListener;
import com.ford.syncV4.protocol.secure.secureproxy.IProtectServiceListener;
import com.ford.syncV4.protocol.secure.secureproxy.IRPCodedDataListener;
import com.ford.syncV4.protocol.secure.secureproxy.ProtocolSecureManager;
import com.ford.syncV4.proxy.HandshakeDataListener;
import com.ford.syncV4.proxy.IMessageDispatcher;
import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;
import com.ford.syncV4.proxy.callbacks.OnError;
import com.ford.syncV4.service.secure.mutations.MutationManager;

import java.io.IOException;
import java.util.Hashtable;

public class SecureSessionContext {

    private final IMessageDispatcher messageDispatcher;
    /**
     * An object which process Secure Service messages
     */
    public SecureServiceMessageManager mSecureServiceMessageManager;
    /**
     * An object which handle responses of the {@link com.ford.syncV4.service.secure.SecureServiceMessageManager}
     */
    public SecureServiceMessageCallback mSecureServiceMessageCallback;
    public ProtocolSecureManager protocolSecureManager;

    IHandshakeDataListener secureProxyServerListener;

    public SecureSessionContext(IMessageDispatcher messageDispatcher) {
        this.messageDispatcher = messageDispatcher;
        secureProxyServerListener = new HandshakeDataListener(this.messageDispatcher);
    }

    public void setUpSecureServiceManager() {
        mSecureServiceMessageManager = new SecureServiceMessageManager();
        mSecureServiceMessageCallback = new SecureServiceMessageCallback();
        mSecureServiceMessageManager.setMessageCallback(mSecureServiceMessageCallback);
        setupProtectServiceListener();
    }

    private void setupProtectServiceListener() {
        mSecureServiceMessageCallback.setProtectServiceListener(new IProtectServiceListener() {

            @Override
            public void onHandshakeResponse(byte[] data) {
                try {
                    protocolSecureManager.writeDataToProxyServer(data, new IRPCodedDataListener() {
                        @Override
                        public void onRPCPayloadCoded(byte[] bytes) {
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onHandshakeError(SecurityInternalError error, Hashtable<String, Object> errorDescription) {
                String errorMessage = "";
                if (errorDescription != null){
                    errorMessage = errorDescription.toString();
                }
                InternalProxyMessage proxyMessage = new OnError("Handshake Error " + error+ "; Messagee: " + errorMessage, new Exception("Handshake Error " + error + "; Messagee: " + errorMessage));
                messageDispatcher.dispatchInternalMessage(proxyMessage);
            }
        });
    }

    public void setupSecureProxy() {
        protocolSecureManager = new ProtocolSecureManager(secureProxyServerListener);
        protocolSecureManager.setupSecureEnvironment();
    }

    public void updateSecureProxyState(byte sessionID, byte version, MutationManager mutationManager) {
        secureProxyServerListener.setSessionID(sessionID);
        secureProxyServerListener.setVersion(version);
        ((HandshakeDataListener) secureProxyServerListener).setMutationManager(mutationManager);
    }

    @Override
    public String toString() {
        return "SecureSessionContext{" +
                "messageDispatcher=" + messageDispatcher +
                ", mSecureServiceMessageManager=" + mSecureServiceMessageManager +
                ", mSecureServiceMessageCallback=" + mSecureServiceMessageCallback +
                ", protocolSecureManager=" + protocolSecureManager +
                ", secureProxyServerListener=" + secureProxyServerListener +
                '}';
    }
}