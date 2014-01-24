package com.ford.syncV4.protocol;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.SyncTransport;

import static org.mockito.Matchers.anyByte;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;

/**
 * Tests for the case when the app wants to unregister itself without closing
 * its connection.
 *
 * Created by enikolsky on 2013-11-15.
 */
public class UnregisterWithoutDisconnectTest extends InstrumentationTestCase {
    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);
    }

    public void testResetProxyShouldDisconnectTransport() throws Exception {
        SyncTransport transportMock = mock(SyncTransport.class);
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);

        SyncConnection connection = spy(new SyncConnection(null));
        connection.init(null, transportMock);

        SyncProxyALM syncProxy =
                new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection);
        assertNotNull(syncProxy);

        syncProxy.resetProxy();

        // FIXME: unfortunately, I can't verify that
        // SyncProxyBase.unregisterAppInterfacePrivate() is called because it's
        // protected. And I couldn't add the PowerMock library to the project
        // due to errors during 'dexTest' step

        verify(connection).closeConnection(anyByte(), eq(false));
        verify(transportMock).disconnect();
    }

    public void testCloseSessionWithKeepingConnectionShouldNotDisconnectTransport()
            throws SyncException {
        SyncTransport transportMock = mock(SyncTransport.class);
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);

        SyncConnection connection = spy(new SyncConnection(null));
        connection.init(null, transportMock);

        SyncProxyALM syncProxy =
                new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection);
        assertNotNull(syncProxy);

        syncProxy.closeSession(true);

        verify(connection).closeConnection(anyByte(), eq(true));
        verify(transportMock, never()).disconnect();
    }

    public void testCloseSessionWithoutKeepingConnectionShouldDisconnectTransport()
            throws SyncException {
        SyncTransport transportMock = mock(SyncTransport.class);
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);

        SyncConnection connection = spy(new SyncConnection(null));
        connection.init(null, transportMock);

        SyncProxyALM syncProxy =
                new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection);
        assertNotNull(syncProxy);

        syncProxy.closeSession(false);

        verify(connection).closeConnection(anyByte(), eq(false));
        verify(transportMock).disconnect();
    }
}
