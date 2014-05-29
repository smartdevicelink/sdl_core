package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

import org.mockito.ArgumentCaptor;

import static org.hamcrest.CoreMatchers.containsString;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 8/22/13
 */
public class SyncProxyALMTest extends InstrumentationTestCase {

    private static final byte SESSION_ID = (byte) 48;
    private static final byte VERSION = ProtocolConstants.PROTOCOL_VERSION_TWO;

    public SyncProxyALMTest() {

    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext()
                .getCacheDir().getPath());
    }

    private SyncProxyALM getSyncProxyALM() throws SyncException {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        when(conf.getTransportType()).thenReturn(TransportType.TCP);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        return getSyncProxyALM(syncMsgVersion, conf, listenerALM);
    }

    private SyncProxyALM getSyncProxyALM(final SyncMsgVersion syncMsgVersion,
                                         final TCPTransportConfig conf,
                                         final IProxyListenerALM listenerALM) throws SyncException {
        return new SyncProxyALM(listenerALM,
                                /*sync proxy configuration resources*/null,
                                /*enable advanced lifecycle management true,*/
                "appName",
                                /*ngn media app*/null,
                                /*vr synonyms*/null,
                                /*is media app*/true,
                                /*App type*/null,
                syncMsgVersion,
                                /*language desired*/Language.EN_US,
                                /*HMI Display Language Desired*/Language.EN_US,
                                /*App ID*/"8675308",
                                /*autoActivateID*/null,
                                /*callbackToUIThre1ad*/ false,
                                /*preRegister*/ false,
                2,
                conf, new TestConfig()) {


            @Override
            public void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                //_haveReceivedFirstNonNoneHMILevel = false;
                //_haveReceivedFirstFocusLevel = false;
                //_haveReceivedFirstFocusLevelFull = false;
                //_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(
                                syncSession.getSessionIdByAppId(SessionTest.APP_ID), false);
                        mSyncConnection = null;
                    }
                    mSyncConnection = mock(SyncConnection.class);
                    when(mSyncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.startTransport();
                    }
                }
            }
        };
    }

    public void testSyncProxyBaseShouldBeCreated() throws Exception {
        SyncProxyALM syncProxy = getSyncProxyALM();
        assertNotNull("Sync Proxy should not be null", syncProxy);
    }

    public void testOnMobileNavSessionStarted() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        when(conf.getTransportType()).thenReturn(TransportType.TCP);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);

        SyncProxyALM proxyALM = new SyncProxyALM(listenerALM,
                                /*sync proxy configuration resources*/null,
                                /*enable advanced lifecycle management true,*/
                "appName",
                                /*ngn media app*/null,
                                /*vr synonyms*/null,
                                /*is media app*/true,
                                /*app type*/null,
                syncMsgVersion,
                                /*language desired*/Language.EN_US,
                                /*HMI Display Language Desired*/Language.EN_US,
                                /*App ID*/"8675308",
                                /*autoActivateID*/null,
                                /*callbackToUIThre1ad*/ false,
                                /*preRegister*/ false,
                2,
                conf, new TestConfig()) {


            @Override
            public void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                //_haveReceivedFirstNonNoneHMILevel = false;
                //_haveReceivedFirstFocusLevel = false;
                //_haveReceivedFirstFocusLevelFull = false;
                //_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(
                                syncSession.getSessionIdByAppId(SessionTest.APP_ID), false);
                        mSyncConnection = null;
                    }
                    mSyncConnection = mock(SyncConnection.class);
                    when(mSyncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.startTransport();
                    }
                }
            }

            @Override
            protected void onProtocolServiceStarted_MobileNavi(byte sessionId) {
                super.onProtocolServiceStarted_MobileNavi(sessionId);
                assertEquals("Session ID should be equal",
                        syncSession.getSessionIdByAppId(SessionTest.APP_ID), SESSION_ID);
            }
        };
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SESSION_ID, VERSION);
    }

    public void testReceivedMobileNavSessionIncomingMessage() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        when(conf.getTransportType()).thenReturn(TransportType.TCP);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        SyncProxyALM proxyALM = new SyncProxyALM(listenerALM,
                                /*sync proxy configuration resources*/null,
                                /*enable advanced lifecycle management true,*/
                "appName",
                                /*ngn media app*/null,
                                /*vr synonyms*/null,
                                /*is media app*/true,
                                /*app type*/null,
                syncMsgVersion,
                                /*language desired*/Language.EN_US,
                                /*HMI Display Language Desired*/Language.EN_US,
                                /*App ID*/"8675308",
                                /*autoActivateID*/null,
                                /*callbackToUIThre1ad*/ false,
                                /*preRegister*/ false,
                2,
                conf, new TestConfig()) {


            @Override
            public void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                //_haveReceivedFirstNonNoneHMILevel = false;
                //_haveReceivedFirstFocusLevel = false;
                //_haveReceivedFirstFocusLevelFull = false;
                //_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(
                                syncSession.getSessionIdByAppId(SessionTest.APP_ID), false);
                        mSyncConnection = null;
                    }
                    mSyncConnection = mock(SyncConnection.class);
                    when(mSyncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.startTransport();
                    }
                }
            }

            @Override
            protected void handleMobileNavMessage(ProtocolMessage message) {
                super.handleMobileNavMessage(message);
                assertEquals(message.getServiceType(), ServiceType.Mobile_Nav);
                assertEquals(message.getVersion(), ProtocolConstants.PROTOCOL_VERSION_TWO);
                assertTrue(message.getSessionID() == SESSION_ID);
            }
        };
        ProtocolMessage message = new ProtocolMessage();
        message.setVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        message.setSessionID(SESSION_ID);
        message.setServiceType(ServiceType.Mobile_Nav);
        proxyALM.dispatchIncomingMessage(message);
    }

    public void testSyncProxyBaseEndSessionCallbackTest() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        SyncProxyALM proxyALM = new SyncProxyALM(listenerALM,
                                /*sync proxy configuration resources*/null,
                                /*enable advanced lifecycle management true,*/
                "appName",
                                /*ngn media app*/null,
                                /*vr synonyms*/null,
                                /*is media app*/true,
                                /*app type*/null,
                syncMsgVersion,
                                /*language desired*/Language.EN_US,
                                /*HMI Display Language Desired*/Language.EN_US,
                                /*App ID*/"8675308",
                                /*autoActivateID*/null,
                                /*callbackToUIThre1ad*/ false,
                                /*preRegister*/ false,
                2,
                conf, new TestConfig()) {


            @Override
            public void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                //_haveReceivedFirstNonNoneHMILevel = false;
                //_haveReceivedFirstFocusLevel = false;
                //_haveReceivedFirstFocusLevelFull = false;
                //_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(
                                syncSession.getSessionIdByAppId(SessionTest.APP_ID), false);
                        mSyncConnection = null;
                    }
                    mSyncConnection = mock(SyncConnection.class);
                    when(mSyncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.startTransport();
                    }
                }
            }
        };
        ArgumentCaptor<ServiceType> sessionTypeCaptor = ArgumentCaptor.forClass(ServiceType.class);
        ArgumentCaptor<String> appIdCaptor = ArgumentCaptor.forClass(String.class);
        proxyALM.handleEndServiceAck(ServiceType.RPC, SESSION_ID);
        verify(listenerALM).onProtocolServiceEndedAck(sessionTypeCaptor.capture(),
                appIdCaptor.capture());
        assertEquals(ServiceType.RPC, sessionTypeCaptor.getValue());
        assertEquals(SessionTest.APP_ID_DEFAULT, appIdCaptor.getValue());
    }

    public void testSyncProxyBaseStartSessionCallbackTest() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        SyncProxyALM proxyALM = new SyncProxyALM(listenerALM,
                                /*sync proxy configuration resources*/null,
                                /*enable advanced lifecycle management true,*/
                "appName",
                                /*ngn media app*/null,
                                /*vr synonyms*/null,
                                /*is media app*/true,
                                /*app type*/null,
                syncMsgVersion,
                                /*language desired*/Language.EN_US,
                                /*HMI Display Language Desired*/Language.EN_US,
                                /*App ID*/"8675308",
                                /*autoActivateID*/null,
                                /*callbackToUIThre1ad*/ false,
                                /*preRegister*/ false,
                2,
                conf, new TestConfig()) {


            @Override
            public void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                //_haveReceivedFirstNonNoneHMILevel = false;
                //_haveReceivedFirstFocusLevel = false;
                //_haveReceivedFirstFocusLevelFull = false;
                //_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(
                                syncSession.getSessionIdByAppId(SessionTest.APP_ID), false);
                        mSyncConnection = null;
                    }
                    mSyncConnection = mock(SyncConnection.class);
                    when(mSyncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.startTransport();
                    }
                }
            }

        };
        ArgumentCaptor<String> appIdCaptor = ArgumentCaptor.forClass(String.class);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SESSION_ID, VERSION);
        verify(listenerALM).onSessionStarted(appIdCaptor.capture());
        assertEquals(SessionTest.APP_ID_DEFAULT, appIdCaptor.getValue());
    }

    public void testHeartBeatIsSet() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        SyncProxyALM proxyALM = new SyncProxyALM(listenerALM,
                                /*sync proxy configuration resources*/null,
                                /*enable advanced lifecycle management true,*/
                "appName",
                                /*ngn media app*/null,
                                /*vr synonyms*/null,
                                /*is media app*/true,
                                /*app type*/null,
                syncMsgVersion,
                                /*language desired*/Language.EN_US,
                                /*HMI Display Language Desired*/Language.EN_US,
                                /*App ID*/"8675308",
                                /*autoActivateID*/null,
                                /*callbackToUIThre1ad*/ false,
                                /*preRegister*/ false,
                2,
                conf, new TestConfig()) {


            @Override
            public void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                //_haveReceivedFirstNonNoneHMILevel = false;
                //_haveReceivedFirstFocusLevel = false;
                //_haveReceivedFirstFocusLevelFull = false;
                //_syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(
                                syncSession.getSessionIdByAppId(SessionTest.APP_ID), false);
                        mSyncConnection = null;
                    }
                    mSyncConnection = mock(SyncConnection.class);
                    when(mSyncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.startTransport();
                    }
                }
            }

        };
        SyncProxyALM.setHeartBeatInterval(50);
        assertEquals("Heartbeat should be 50", 50, SyncProxyALM.getHeartBeatInterval());
    }


    public void testMaxJsonSizeInIncomingMessageShouldCallOnError() throws SyncException {
        final WiProProtocol protocol = new WiProProtocol(mock(IProtocolListener.class));
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        final SyncConnection syncConnectionMock = mock(SyncConnection.class);
        when(syncConnectionMock.getWiProProtocol()).thenReturn(protocol);

        IProxyListenerALMTesting proxyListenerMock = mock(IProxyListenerALMTesting.class);
        SyncProxyALM proxy =
                new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                        false, null, null, null, null, null, null, false, false,
                        ProtocolConstants.PROTOCOL_VERSION_TWO, null, syncConnectionMock,
                        new TestConfig());
        SyncConnection connection = new SyncConnection(new Session(), proxy.getInterfaceBroker());
        connection.init(null, mock(SyncTransport.class));
        proxy.setSyncConnection(connection);
        when(connection.getIsConnected()).thenReturn(true);

        final byte maxByte = (byte) 0xFF;
        final byte[] bytes =
                { 0x21, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, maxByte, maxByte, maxByte, maxByte, 0x00 };
        connection.onTransportBytesReceived(bytes, bytes.length);

        ArgumentCaptor<Throwable> throwableArgumentCaptor =
                ArgumentCaptor.forClass(Throwable.class);
        verify(proxyListenerMock, timeout(100).times(1)).onError(anyString(),
                throwableArgumentCaptor.capture());
        assertThat(throwableArgumentCaptor.getValue().toString(),
                containsString(OutOfMemoryError.class.getSimpleName()));
    }
}