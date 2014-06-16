package com.ford.syncV4.protocol;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.usb.USBTransportConfig;

import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;

/**
 * Tests for the case when the app wants to unregister itself.
 *
 * According to https://adc.luxoft.com/contour/perspective.req?projectId=65&docId=206179
 * UnregisterAppInterface disconnects the application from SDL. Connection is NOT closed.
 *
 * Created by enikolsky on 2013-11-15.
 */
public class UnregisterWithoutDisconnectTest extends InstrumentationTestCase {

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);
    }

    public void testResetProxyShouldNotDisconnectTransport() throws Exception {
        SyncTransport transportMock = mock(SyncTransport.class);
        SyncConnection connection = spy(new SyncConnection(SessionTest.getInitializedSession(),
                new USBTransportConfig(getInstrumentation().getTargetContext()),
                null));
        connection.init();

        SyncProxyALM syncProxy = getSyncProxy(connection);
        assertNotNull(syncProxy);

        syncProxy.getInterfaceBroker().onTransportConnected();

        syncProxy.resetProxy();

        // FIXME: unfortunately, I can't verify that
        // SyncProxyBase.unregisterAppInterfacePrivate() is called because it's
        // protected. And I couldn't add the PowerMock library to the project
        // due to errors during 'dexTest' step

        verify(connection).closeConnection(eq(true));
        verify(transportMock, never()).disconnect();
    }

    public void testUnregisterAppInterfaceShouldNotDisconnectTransport() throws SyncException {
        SyncTransport transportMock = mock(SyncTransport.class);
        ISyncConnectionListener syncConnectionListenerMock = mock(ISyncConnectionListener.class);
        Session session = SessionTest.getInitializedSession();

        SyncConnection connection = spy(new SyncConnection(session,
                new USBTransportConfig(getInstrumentation().getTargetContext()),
                syncConnectionListenerMock));
        connection.init();

        SyncProxyALM syncProxy = getSyncProxy(connection);
        assertNotNull(syncProxy);

        syncProxy.doUnregisterAppInterface(SessionTest.APP_ID);

        verify(connection, never()).closeConnection(eq(true));
        verify(transportMock, never()).disconnect();
    }

    private SyncProxyALM getSyncProxy(SyncConnection connection) throws SyncException {
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        return new SyncProxyALM(proxyListenerMock, null, "test", null, null,
                false, null, null, null, null, SessionTest.APP_ID, null, false, false,
                ProtocolConstants.PROTOCOL_VERSION_TWO, null, connection, new TestConfig());
    }
}