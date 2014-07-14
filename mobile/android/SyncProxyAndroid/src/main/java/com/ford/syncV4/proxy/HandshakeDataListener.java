package com.ford.syncV4.proxy;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.secure.secureproxy.IHandshakeDataListener;
import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;
import com.ford.syncV4.proxy.callbacks.OnError;
import com.ford.syncV4.service.secure.SecureServiceMessageFactory;
import com.ford.syncV4.service.secure.mutations.AbstractMutation;
import com.ford.syncV4.service.secure.mutations.MutationManager;
import com.ford.syncV4.session.Session;

/**
 * Created by admin on 7/2/14.
 */
public class HandshakeDataListener implements IHandshakeDataListener {

    private MutationManager mutationManager;
    private IMessageDispatcher messageDispatcher;
    private byte sessionID = Session.DEFAULT_SESSION_ID;
    private byte version = 2;

    public HandshakeDataListener(IMessageDispatcher messageDispatcher) {
        this.messageDispatcher = messageDispatcher;
    }

    public MutationManager getMutationManager() {
        return mutationManager;
    }

    public void setMutationManager(MutationManager mutationManager) {
        this.mutationManager = mutationManager;
    }

    @Override
    public void onHandshakeDataReceived(byte[] data) {
        ProtocolMessage protocolMessage =
                SecureServiceMessageFactory.buildHandshakeRequest(sessionID, data, version);
        ProtocolMessage message = mutateHandshakeMessage(protocolMessage);
        messageDispatcher.dispatchOutgoingMessage(message);
    }

    private ProtocolMessage mutateHandshakeMessage(ProtocolMessage protocolMessage) {
        AbstractMutation mutation = mutationManager.getHeadMutation();
        if (mutation == null) {
            return protocolMessage;
        }
        ProtocolMessage result = mutation.mutate(protocolMessage);
        return result;
    }

    @Override
    public void onHandShakeCompleted() {

    }

    @Override
    public void onError(Exception e) {
        String errorMsg = "Secure Connection Error ";
        if (e.getMessage() != null) {
            errorMsg = e.getMessage();
        }
        InternalProxyMessage proxyMessage = new OnError(errorMsg, e);
        messageDispatcher.dispatchInternalMessage(proxyMessage);
    }

    @Override
    public synchronized void setSessionID(byte sessionId) {
        this.sessionID = sessionId;
    }

    @Override
    public synchronized void setVersion(byte version) {
        this.version = version;
    }
}
