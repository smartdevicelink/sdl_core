package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.OnAppInterfaceUnregistered;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.session.EndServiceInitiator;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.TCPTransportConfig;

import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

import java.io.OutputStream;
import java.io.PipedOutputStream;
import java.util.Hashtable;
import java.util.List;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyByte;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 1/13/14
 */
public class SyncProxyBaseTest extends InstrumentationTestCase {

    private static final int CALLBACK_WAIT_TIMEOUT = 500;
    private IProxyListenerALM proxyListenerMock;
    private SyncProxyALM proxy;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);
        proxyListenerMock = mock(IProxyListenerALM.class);
        proxy = TestCommon.getSyncProxyALMNoTransport(getInstrumentation().getTargetContext(),
                proxyListenerMock);

        when(proxy.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);
    }

    public void testSyncProxyBaseHasServicePoolField() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        assertNotNull("should not be null", proxyALM.getServicePool());
    }

    public void testMobileNavSessionAddedToServicePoolOnStart() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        List<Service> serviceList = proxyALM.getServicePool();
        assertTrue(proxyALM.hasServiceInServicesPool(SessionTest.APP_ID, ServiceType.Mobile_Nav));
        assertEquals(SessionTest.APP_ID, serviceList.get(0).getAppId());
    }

    public void testMobileNavSessionRemovedFromPoolListOnStop() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.stopMobileNaviService(SessionTest.APP_ID);
        assertEquals("pool should be empty", false,
                proxyALM.hasServiceInServicesPool(SessionTest.APP_ID, ServiceType.Mobile_Nav));
    }

    public void testMobileNavServiceEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testRPCServiceAddedToPoolOnStart() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        assertEquals("pool should has RPC service", 1, proxyALM.getServicePool().size());
    }

    public void testRPCServiceOnEndSessionStayInCollection() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.initializeSession(SessionTest.APP_ID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);

        proxyALM.endSession(SessionTest.SESSION_ID, EndServiceInitiator.SDK);
        assertEquals("pool should not be empty", 1, proxyALM.getServicePool().size());
    }

    // TODO : reconsider this case because 'dispose' does not clean Session directly, but via it's
    // response from SDL

    /*public void testAllServicesEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.initializeSession(SessionTest.APP_ID);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }*/

    public void testStopMobileNaviSessionForUnexcitingSessionDontThrowsException() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        try {
            proxyALM.stopMobileNaviService(SessionTest.APP_ID);
        } catch (ArrayIndexOutOfBoundsException e) {
            assertNull("exception should not be thrown", e);
        }
    }

    private SyncProxyBase getSyncProxyBase() throws SyncException {
        SyncMsgVersion syncMsgVersion = RPCStructFactory.createSyncMsgVersion();
        TCPTransportConfig conf = mock(TCPTransportConfig.class);

        return new SyncProxyALM(proxyListenerMock,
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
                ProtocolConstants.PROTOCOL_VERSION_THREE,
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
                        mSyncConnection.closeConnection();
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
                syncSession.addAppId(SessionTest.APP_ID);
                syncSession.updateSessionId(SessionTest.SESSION_ID);
            }

            @Override
            public void setSyncConnection(SyncConnection syncConnection) {
                if (syncConnection != null) {
                    super.setSyncConnection(syncConnection);
                }
            }
        };
    }

    public void testOnAudioServiceStartServiceAddedToPool() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        Service audioService = new Service();
        audioService.setAppId(SessionTest.APP_ID);
        audioService.setServiceType(ServiceType.Audio_Service);
        assertTrue("pool should have AudioService ",
                proxyALM.hasServiceInServicesPool(SessionTest.APP_ID, ServiceType.Audio_Service));
    }

    public void testOnAudioServiceStartServiceCallbackCalled() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        Mockito.verify(proxyListenerMock, times(1)).onAudioServiceStart(SessionTest.APP_ID);
    }

    public void testAudioServiceRemovedFromPoolOnStopAudioService() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.stopAudioService(SessionTest.APP_ID);
        Service mobileNaviService = new Service();
        mobileNaviService.setAppId(SessionTest.APP_ID);
        mobileNaviService.setServiceType(ServiceType.Mobile_Nav);
        assertTrue("pool should have Mobile nav service ",
                proxyALM.hasServiceInServicesPool(SessionTest.APP_ID, ServiceType.Mobile_Nav));
        Service audioService = new Service();
        audioService.setAppId(SessionTest.APP_ID);
        audioService.setServiceType(ServiceType.Audio_Service);
        assertFalse("pool should not have Audio service ",
                proxyALM.getServicePool().contains(audioService));
    }

    public void testStartAudioDataTransferClassConnectionMethod() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.startAudioDataTransfer(SessionTest.APP_ID);
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        verify(proxyALM.mSyncConnection, times(1)).startAudioDataTransfer(sessionIDCaptor.capture());
    }

    public void testStartAudioDataTransferReturnsStream() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        when(proxyALM.mSyncConnection.startAudioDataTransfer(SessionTest.SESSION_ID)).thenReturn(new PipedOutputStream());
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                SessionTest.SESSION_ID, ProtocolConstants.PROTOCOL_VERSION_THREE);
        OutputStream stream = proxyALM.startAudioDataTransfer(SessionTest.APP_ID);
        assertNotNull("stream should not be null", stream);
    }

    public void testStopAudioDataTransferFiresCallback() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        proxyALM.stopAudioDataTransfer();
        verify(proxyALM.mSyncConnection, timeout(1)).stopAudioDataTransfer();
    }

    // TODO : Reconsider such cases as "doUnregisterAppInterface" does not directly close any connections
    /*public void testCloseSessionCalledWithRightSessionID() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        proxyALM.setActiveAppId(SessionTest.APP_ID);
        proxyALM.getInterfaceBroker().onTransportConnected();
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxyALM.doUnregisterAppInterface(SessionTest.APP_ID, false);
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        ArgumentCaptor<Boolean> keepConnectionCaptor = ArgumentCaptor.forClass(Boolean.class);
        verify(proxyALM.mSyncConnection, times(1)).closeConnection(sessionIDCaptor.capture(),
                keepConnectionCaptor.capture());
        assertEquals("session id of closed RPC service should be same as initial session id",
                SessionTest.SESSION_ID, sessionIDCaptor.getValue().byteValue());
        assertFalse(keepConnectionCaptor.getValue());
    }*/

    public void testAppUnregisteredWithIgnitionOff() throws Exception {
        assertNotNull(proxy);

        // send OnAppInterfaceUnregistered
        Hashtable<String, Object> params = new Hashtable<String, Object>();

        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.IGNITION_OFF;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        ArgumentCaptor<OnAppInterfaceUnregistered> argumentCaptor =
                ArgumentCaptor.forClass(OnAppInterfaceUnregistered.class);

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredReason(anyString(),
                argumentCaptor.capture());

        assertEquals(AppInterfaceUnregisteredReason.IGNITION_OFF,
                argumentCaptor.getValue().getReason());
    }

    public void testAppUnregisteredWithMasterReset() throws Exception {
        assertNotNull(proxy);

        // send OnAppInterfaceUnregistered
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.MASTER_RESET;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        ArgumentCaptor<OnAppInterfaceUnregistered> argumentCaptor =
                ArgumentCaptor.forClass(OnAppInterfaceUnregistered.class);

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredReason(anyString(),
                argumentCaptor.capture());

        assertEquals(AppInterfaceUnregisteredReason.MASTER_RESET,
                argumentCaptor.getValue().getReason());
    }

    public void testAppUnregisteredWithFactoryDefaults() throws Exception {
        assertNotNull(proxy);

        // send OnAppInterfaceUnregistered
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.FACTORY_DEFAULTS;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        ArgumentCaptor<OnAppInterfaceUnregistered> argumentCaptor =
                ArgumentCaptor.forClass(OnAppInterfaceUnregistered.class);

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredReason(anyString(),
                argumentCaptor.capture());

        assertEquals(AppInterfaceUnregisteredReason.FACTORY_DEFAULTS,
                argumentCaptor.getValue().getReason());
    }

    // Obsolete case
    /*public void testScheduleInitializeProxyNotCalledIfServiceListIsEmpty() throws Exception {
        assertNotNull(proxy);
        addSessionToServicesList();
        proxy.syncSession.stopSession(SessionTest.APP_ID);
        proxy.scheduleInitializeProxy();
        TimerTask timerTask = proxy.getCurrentReconnectTimerTask();
        assertNull("timerTask should be null", timerTask);
    }*/

    public void testInCaseSessionRestartedRpcServiceShouldBeRecreated() throws Exception {
        assertNotNull(proxy);
        addSessionToServicesList();
        proxy.doUnregisterAppInterface(SessionTest.APP_ID);
        proxy.setSyncConnection(mock(SyncConnection.class));
        proxy.getInterfaceBroker().onTransportConnected();
        proxy.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        assertFalse(proxy.syncSession.isServicesEmpty());
        assertTrue(proxy.syncSession.hasService(SessionTest.APP_ID, ServiceType.RPC));
    }

    public void testSessionHasOnlyOneRPCService() throws Exception {
        assertNotNull(proxy);
        proxy.initializeSession(SessionTest.APP_ID);
        addSessionToServicesList();
        //proxy.setSyncConnection(mock(SyncConnection.class));
        proxy.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxy.initializeSession(SessionTest.APP_ID);
        proxy.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        assertEquals("only one rpc service should be in service list", 1,
                proxy.syncSession.getServicesNumber());
    }

    public void testUnregisterAppResponseTriggersStopServicesAndSession() throws Exception {
        assertNotNull(proxy);

        proxy.getInterfaceBroker().onTransportConnected();

        proxy.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);

        proxy.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxy.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        Hashtable<String, Object> params = new Hashtable<String, Object>();

        params.put(Names.resultCode, Result.SUCCESS);
        params.put(Names.success, true);
        ProtocolMessage protocolMessage = TestCommon.createProtocolMessage(
                Names.UnregisterAppInterface, params,
                ProtocolMessage.RPCTYPE_RESPONSE, 1);
        protocolMessage.setSessionID(SessionTest.SESSION_ID);

        proxy.dispatchIncomingMessage(protocolMessage);
        verify(proxy.getSyncConnection(), times(1)).closeMobileNaviService(SessionTest.SESSION_ID);
        verify(proxy.getSyncConnection(), times(1)).closeAudioService(SessionTest.SESSION_ID);
        assertNotNull(proxy.getSyncConnection());
        assertEquals("Service pool should not be empty", 1, proxy.getServicePool().size());
        assertEquals("Session id should be 0", SessionTest.SESSION_ID,
                proxy.syncSession.getSessionIdByAppId(SessionTest.APP_ID));
    }

    public void testAutoActivateIdDesiredSet() throws Exception {
        String autoActivateIdDesired = "_autoActivateIdDesired";
        proxy.setAutoActivateIdReturned(autoActivateIdDesired);
        String real = proxy.getAutoActivateIdReturned();
        assertEquals(real, autoActivateIdDesired);
    }

    public void testSyncConnectionSessionIdIsSetToNullAfterStopSession() throws Exception {
        proxy.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_THREE);
        proxy.doUnregisterAppInterface(SessionTest.APP_ID);
        byte realSessionId = proxy.getSessionIdByAppId(SessionTest.APP_ID);
        assertEquals(ProtocolConstants.PROTOCOL_VERSION_UNDEFINED, realSessionId);
    }

    public void testRPCMessageHandlerIsSetAfterCreation() throws Exception {
        IRPCMessageHandler messageHandler = proxy.getRPCMessageHandler();
        assertNotNull("message handler should be not null", messageHandler);
    }

    public void testRPCMessageHandlerCalledOnIncomingMessage() throws Exception {
        proxy.setRPCMessageHandler(mock(IRPCMessageHandler.class));
        // send OnAppInterfaceUnregistered
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.IGNITION_OFF;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 1));
        verify(proxy.getRPCMessageHandler(), times(1)).handleRPCMessage(anyByte(), anyString(),
                any(Hashtable.class));
    }

    private void addSessionToServicesList() {
        if (!proxy.syncSession.hasService(SessionTest.APP_ID, ServiceType.RPC)) {
            Service service = new Service();
            service.setServiceType(ServiceType.RPC);
            service.setAppId(SessionTest.APP_ID);
            proxy.syncSession.addService(service);
        }
    }
}