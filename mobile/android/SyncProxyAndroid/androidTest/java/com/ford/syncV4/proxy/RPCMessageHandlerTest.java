package com.ford.syncV4.proxy;

import android.test.AndroidTestCase;

import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.session.Session;

import static org.mockito.Mockito.mock;

/**
 * Created by Andrew Batutin on 2/10/14
 */
public class RPCMessageHandlerTest extends AndroidTestCase {

    public void testRPCMessageHandlerCreated() throws Exception {
        RPCMessageHandler rpcMessageHandler = new RPCMessageHandler(mock(SyncProxyBase.class));
        assertNotNull("rpcMessageHandler should not be bull", rpcMessageHandler);
    }

    public void testHandleRPCMessageWIthNullHashTableDoNotThrowNPE() throws Exception {
        RPCMessageHandler rpcMessageHandler = new RPCMessageHandler(mock(SyncProxyBase.class));
        rpcMessageHandler.handleRPCMessage(Session.DEFAULT_SESSION_ID, "", null);
    }
}