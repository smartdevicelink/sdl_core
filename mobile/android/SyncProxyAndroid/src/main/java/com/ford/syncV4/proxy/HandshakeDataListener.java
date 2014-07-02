package com.ford.syncV4.proxy;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.secure.secureproxy.IHandshakeDataListener;
import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;
import com.ford.syncV4.proxy.callbacks.OnError;
import com.ford.syncV4.service.secure.SecureServiceMessageFactory;
import com.ford.syncV4.service.secure.SecurityInternalError;
import com.ford.syncV4.service.secure.mutations.MutationManager;
import com.ford.syncV4.session.Session;

/**
 * Created by admin on 7/2/14.
 */
public class HandshakeDataListener implements IHandshakeDataListener {

    public MutationManager mutationManager = new MutationManager();

    {
        mutationManager.addMutationForError(SecurityInternalError.ERROR_INVALID_QUERY_ID);
        mutationManager.addMutationForError(SecurityInternalError.ERROR_INVALID_QUERY_SIZE);
    }

    private SyncProxyBase syncProxyBase;
    private byte sessionID = Session.DEFAULT_SESSION_ID;
    private byte version = 2;

    public HandshakeDataListener(SyncProxyBase syncProxyBase) {
        this.syncProxyBase = syncProxyBase;
    }

    @Override
    public void onHandshakeDataReceived(byte[] data) {
        ProtocolMessage protocolMessage =
                SecureServiceMessageFactory.buildHandshakeRequest(sessionID, data, version);
        syncProxyBase.dispatchOutgoingMessage(mutateHandshakeMessage(protocolMessage));
    }

    private ProtocolMessage mutateHandshakeMessage(ProtocolMessage protocolMessage) {
        return mutationManager.getHeadMutation().mutate(protocolMessage);
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
        syncProxyBase.dispatchInternalMessage(proxyMessage);
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
