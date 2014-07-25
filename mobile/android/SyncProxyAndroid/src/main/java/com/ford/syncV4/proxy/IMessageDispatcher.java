package com.ford.syncV4.proxy;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;
import com.ford.syncV4.proxy.interfaces.IProxyListenerBase;

/**
 * Created by admin on 7/14/14.
 */
public interface IMessageDispatcher<ProxyListenerType extends IProxyListenerBase> {
    void dispatchIncomingMessage(ProtocolMessage message);

    void dispatchOutgoingMessage(ProtocolMessage message);

    void dispatchInternalMessage(InternalProxyMessage message);
}
