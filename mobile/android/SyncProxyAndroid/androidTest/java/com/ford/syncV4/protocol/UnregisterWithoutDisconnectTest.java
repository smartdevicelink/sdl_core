package com.ford.syncV4.protocol;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.test.TestConfig;

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

    private static final String APP_ID = "AppId";
    private static final byte SESSION_ID = 0x01;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);
    }

    // TODO : Should we disconnect transport ???
    public void testResetProxyShouldDisconnectTransport() throws Exception {
        SyncTransport transportMock = mock(SyncTransport.class);
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);

        SyncConnection connection = spy(new SyncConnection(SessionTest.getInitializedSession(), null));
        connection.init(null, transportMock);

        SyncProxyALM syncProxy =
                new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection, new TestConfig());
        assertNotNull(syncProxy);

        syncProxy.setActiveAppId(SessionTest.APP_ID);
        syncProxy.getInterfaceBroker().onTransportConnected();

        syncProxy.resetProxy();

        // FIXME: unfortunately, I can't verify that
        // SyncProxyBase.unregisterAppInterfacePrivate() is called because it's
        // protected. And I couldn't add the PowerMock library to the project
        // due to errors during 'dexTest' step

        verify(connection).closeConnection(anyByte(), eq(true));
        //verify(transportMock).disconnect();
    }

    public void testCloseSessionWithKeepingConnectionShouldNotDisconnectTransport()
            throws SyncException {
        SyncTransport transportMock = mock(SyncTransport.class);
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        ISyncConnectionListener syncConnectionListenerMock = mock(ISyncConnectionListener.class);
        Session session = SessionTest.getInitializedSession();

        SyncConnection connection = spy(new SyncConnection(session, syncConnectionListenerMock));
        connection.init(null, transportMock);

        SyncProxyALM syncProxy =
                new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection, new TestConfig());
        assertNotNull(syncProxy);

        syncProxy.closeSession(APP_ID, true);

        //verify(connection).closeConnection(anyByte(), eq(true));
        verify(transportMock, never()).disconnect();
    }

    public void testCloseSessionWithoutKeepingConnectionShouldDisconnectTransport()
            throws SyncException {
        SyncTransport transportMock = mock(SyncTransport.class);
        ISyncConnectionListener syncConnectionListenerMock = mock(ISyncConnectionListener.class);
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        Session session = SessionTest.getInitializedSession();
        SyncConnection connection = spy(new SyncConnection(session, syncConnectionListenerMock));
        connection.init(null, transportMock);

        SyncProxyALM syncProxy =
                new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection, new TestConfig());
        assertNotNull(syncProxy);

        syncProxy.setActiveAppId(SessionTest.APP_ID);
        syncProxy.getInterfaceBroker().onTransportConnected();

        //syncProxy.closeSession(APP_ID, false);
        syncProxy.dispose();

        connection.onProtocolServiceEnded(ServiceType.RPC, SessionTest.SESSION_ID);

        verify(syncConnectionListenerMock).onProtocolServiceEnded(eq(ServiceType.RPC), eq(SessionTest.SESSION_ID));
        verify(transportMock).disconnect();
    }
}