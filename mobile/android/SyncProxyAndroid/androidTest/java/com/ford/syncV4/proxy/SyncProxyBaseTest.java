package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.DeviceInfo;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.AppHMIType;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.SyncInterfaceAvailability;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.TCPTransportConfig;

import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

import java.io.OutputStream;
import java.io.PipedOutputStream;
import java.util.Hashtable;
import java.util.List;
import java.util.TimerTask;
import java.util.Vector;

import static org.mockito.Matchers.any;
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
    private static byte sessionID = (byte) 1;
    private IProxyListenerALM proxyListenerMock;
    private SyncProxyALM proxy;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);
        proxyListenerMock = mock(IProxyListenerALM.class);
        proxy = TestCommon.getSyncProxyALMNoTransport(proxyListenerMock);

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

        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        List<Service> serviceList = proxyALM.getServicePool();
        assertTrue(proxyALM.hasServiceInServicesPool(ServiceType.Mobile_Nav));
        assertEquals(sessionID, serviceList.get(0).getSession().getSessionId());
    }

    public void testMobileNavSessionRemovedFromPoolListOnStop() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.stopMobileNaviService();
        assertEquals("pool should be empty", false, proxyALM.hasServiceInServicesPool(ServiceType.Mobile_Nav));
    }

    public void testMobileNavServiceEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testRPCServiceAddedToPoolOnStart() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        assertEquals("pool should has RPC service", 1, proxyALM.getServicePool().size());
    }

    public void testRPCServiceEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testAllServicesEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, sessionID, false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testStopMobileNaviSessionForUnexcitingSessionDontThrowsException() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        try {
            proxyALM.stopMobileNaviService();
        } catch (ArrayIndexOutOfBoundsException e) {
            assertNull("exception should not be thrown", e);
        }
    }

    private SyncProxyBase getSyncProxyBase() throws SyncException {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
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
                _haveReceivedFirstNonNoneHMILevel = false;
                _haveReceivedFirstFocusLevel = false;
                _haveReceivedFirstFocusLevelFull = false;
                _syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (mSyncConnection != null) {
                        mSyncConnection.closeConnection(currentSession.getSessionId(), false);
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
                currentSession.setSessionId(sessionID);
            }

            @Override
            public void setSyncConnection(SyncConnection value) {
                if (value != null) {
                    super.setSyncConnection(value);
                }
            }
        };
    }

    public void testOnAudioServiceStartServiceAddedToPool() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                session.getSessionId(), false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        Service audioService = new Service();
        audioService.setSession(session);
        audioService.setServiceType(ServiceType.Audio_Service);
        assertTrue("pool should have AudioService ",
                proxyALM.hasServiceInServicesPool(ServiceType.Audio_Service));
    }

    public void testOnAudioServiceStartServiceCallbackCalled() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                session.getSessionId(), false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        Mockito.verify(proxyListenerMock, times(1)).onAudioServiceStart(false, sessionID);
    }

    public void testAudioServiceRemovedFromPoolOnStopAudioService() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();

        when(proxyALM.mSyncConnection.getProtocolVersion())
                .thenReturn(ProtocolConstants.PROTOCOL_VERSION_THREE);

        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav,
                session.getSessionId(), false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service,
                session.getSessionId(), false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.stopAudioService();
        Service mobileNaviService = new Service();
        mobileNaviService.setSession(session);
        mobileNaviService.setServiceType(ServiceType.Mobile_Nav);
        assertTrue("pool should have Mobile nav service ",
                proxyALM.hasServiceInServicesPool(ServiceType.Mobile_Nav));
        Service audioService = new Service();
        audioService.setSession(session);
        audioService.setServiceType(ServiceType.Audio_Service);
        assertFalse("pool should not have Audio service ",
                proxyALM.getServicePool().contains(audioService));
    }

    public void testStartAudioDataTransferClassConnectionMethod() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.startAudioDataTransfer();
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        ArgumentCaptor<Boolean> encryptionCaptor = ArgumentCaptor.forClass(boolean.class);
        verify(proxyALM.mSyncConnection, times(1)).startAudioDataTransfer(sessionIDCaptor.capture(), encryptionCaptor.capture());
    }

    public void testStartAudioDataTransferReturnsStream() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        when(proxyALM.mSyncConnection.startAudioDataTransfer(sessionID, false)).thenReturn(new PipedOutputStream());
        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), false, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        OutputStream stream = proxyALM.startAudioDataTransfer();
        assertNotNull("stream should not be null", stream);
    }

    public void testStopAudioDataTransferFiresCallback() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        proxyALM.stopAudioDataTransfer();
        verify(proxyALM.mSyncConnection, timeout(1)).stopAudioDataTransfer();
    }

    public void testCloseSessionCalledWithRightSessionID() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.mSyncConnection = mock(SyncConnection.class);
        Session session = Session.createSession(ServiceType.RPC, sessionID, false);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxyALM.closeSession(false);
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        ArgumentCaptor<Boolean> keepConnectionCaptor = ArgumentCaptor.forClass(Boolean.class);
        verify(proxyALM.mSyncConnection, times(1)).closeConnection(sessionIDCaptor.capture(), keepConnectionCaptor.capture());
        assertEquals("session id of closed RPC service should be same as initial session id", sessionID, sessionIDCaptor.getValue().byteValue());
        assertFalse(keepConnectionCaptor.getValue());
    }

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

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredReason(
                reason);
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

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredReason(
                reason);
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

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredReason(
                reason);
    }

    public void testScheduleInitializeProxyNotCalledIfServiceListIsEmpty() throws Exception {
        assertNotNull(proxy);
        proxy.currentSession = Session.createSession(ServiceType.RPC, sessionID, false);
        proxy.currentSession.stopSession();
        proxy.scheduleInitializeProxy();
        TimerTask timerTask = proxy.getCurrentReconnectTimerTask();
        assertNull("timerTask should be null", timerTask);
    }

    public void testScheduleInitializeProxyCalledIfServiceListIsNotEmpty() throws Exception {
        assertNotNull(proxy);
        proxy.currentSession = Session.createSession(ServiceType.RPC, sessionID, false);
        proxy.scheduleInitializeProxy();
        TimerTask timerTask = proxy.getCurrentReconnectTimerTask();
        assertNotNull("timerTask should not be null", timerTask);
    }

    public void testInCaseSessionRestartedRpcServiceShouldBeRecreated() throws Exception {
        assertNotNull(proxy);
        proxy.currentSession = Session.createSession(ServiceType.RPC, sessionID, false);
        proxy.closeSession(false);
        proxy.setSyncConnection(mock(SyncConnection.class));
        proxy.getInterfaceBroker().onProtocolSessionStarted(Session.createSession(ServiceType.RPC, sessionID, false), ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        assertFalse(proxy.currentSession.isServicesEmpty());
        assertTrue(proxy.currentSession.hasService(ServiceType.RPC));
    }

    public void testSessionHasOnlyOneRPCService() throws Exception {
        assertNotNull(proxy);
        proxy.currentSession = Session.createSession(ServiceType.RPC, sessionID, false);
        proxy.closeSession(false);
        proxy.setSyncConnection(mock(SyncConnection.class));
        proxy.getInterfaceBroker().onProtocolSessionStarted(Session.createSession(ServiceType.RPC, sessionID, false), ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxy.getInterfaceBroker().onProtocolSessionStarted(Session.createSession(ServiceType.RPC, sessionID, false), ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        assertEquals("only one rpc service should be in service list", 1, proxy.currentSession.getServiceList().size());
    }

    public void testUnregisterAppResponseTriggersStopServicesAndSession() throws Exception {
        assertNotNull(proxy);

        proxy.getInterfaceBroker().onProtocolSessionStarted(Session.createSession(ServiceType.RPC,
                sessionID, false), ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxy.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, false,
                ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxy.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, sessionID, false,
                ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        Hashtable<String, Object> params = new Hashtable<String, Object>();

        params.put(Names.resultCode, Result.SUCCESS);
        params.put(Names.success, true);
        ProtocolMessage protocolMessage = TestCommon.createProtocolMessage(
                Names.UnregisterAppInterface, params,
                ProtocolMessage.RPCTYPE_RESPONSE, 1);
        protocolMessage.setSessionID(sessionID);

        proxy.dispatchIncomingMessage(protocolMessage);
        verify(proxy.getSyncConnection(), times(1)).closeMobileNaviService(sessionID);
        verify(proxy.getSyncConnection(), times(1)).closeAudioService(sessionID);
        verify(proxy.getSyncConnection(), times(1)).closeConnection(sessionID, true);
        assertNotNull(proxy.getSyncConnection());
        assertEquals("Service pool should be empty", 0, proxy.getServicePool().size());
        assertEquals("Session id should be 0", 0, proxy.currentSession.getSessionId());
    }

    public void testSyncMsgVersionRequestSet() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        proxy.setSyncMsgVersionRequest(syncMsgVersion);
        SyncMsgVersion real = proxy.getSyncMsgVersionRequest();
        assertEquals("msg version should be equal", real, syncMsgVersion);
    }

    public void testApplicationNameSetTest() throws Exception {
        String name = "name";
        proxy.setApplicationName(name);
        String real = proxy.getApplicationName();
        assertEquals("names should be the same", real, name);
    }

    public void testTtsNameSet() throws Exception {
        Vector<TTSChunk> ttsChunkVector = new Vector<TTSChunk>();
        TTSChunk ttsChunk = new TTSChunk();
        ttsChunk.setText("Text");
        ttsChunkVector.add(ttsChunk);
        proxy.setTTSName(ttsChunkVector);
        Vector<TTSChunk> real = proxy.getTTSName();
        assertEquals(real, ttsChunkVector);
    }

    public void testNgnMediaScreenAppNameSet() throws Exception {
        String mediaScreenName = "name";
        proxy.setNgnMediaScreenAppName(mediaScreenName);
        String real = proxy.getNgnMediaScreenAppName();
        assertEquals(real, mediaScreenName);
    }

    public void testVrSynonymsSet() throws Exception {
        Vector<String> vrSynonums = new Vector<String>();
        vrSynonums.add("name");
        proxy.setVrSynonyms(vrSynonums);
        Vector<String> real = proxy.getVrSynonyms();
        assertEquals(real, vrSynonums);
    }

    public void testIsMediaAppSet() throws Exception {
        Boolean isMediaApp = false;
        proxy.setIsMediApp(isMediaApp);
        Boolean real = proxy.getIsMediaApp();
        assertEquals(real, isMediaApp);
    }

    public void testSyncLanguageDesiredSet() throws Exception {
        Language language = Language.AR_SA;
        proxy.setSyncLanguageDesired(language);
        Language real = proxy.getSyncLanguageDesired();
        assertEquals(real, language);
    }

    public void testHmiDisplayLanguageDesiredSet() throws Exception {
        Language language = Language.AR_SA;
        proxy.setHmiDisplayLanguageDesired(language);
        Language real = proxy.getHmiDisplayLanguageDesired();
        assertEquals(real, language);
    }

    public void testAppHMITypeSet() throws Exception {
        Vector<AppHMIType> appHMITypeVector = new Vector<AppHMIType>();
        appHMITypeVector.add(AppHMIType.DEFAULT);
        proxy.setAppHMIType(appHMITypeVector);
        Vector<AppHMIType> real = proxy.getAppHMIType();
        assertEquals(real, appHMITypeVector);
    }

    public void testAppIDSet() throws Exception {
        String appID = "appID";
        proxy.setAppID(appID);
        String real = proxy.getAppID();
        assertEquals(real, appID);
    }

    public void testAutoActivateIdDesiredSet() throws Exception {
        String autoActivateIdDesired = "_autoActivateIdDesired";
        proxy.setAutoActivateIdReturned(autoActivateIdDesired);
        String real = proxy.getAutoActivateIdReturned();
        assertEquals(real, autoActivateIdDesired);
    }

    public void testSyncConnectionSessionIdIsSetToNullAfterStopSession() throws Exception {
        proxy.getInterfaceBroker().onProtocolSessionStarted(Session.createSession(ServiceType.RPC, sessionID, false), ProtocolConstants.PROTOCOL_VERSION_THREE, "");
        proxy.closeSession(true);
        byte realSessionId = proxy.getSyncConnection().getSessionId();
        assertEquals(0, realSessionId);
    }

    public void testUpdateRegisterAppInterfaceParametersChangesRegisterParameters() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        Vector<TTSChunk> ttsChunkVector = new Vector<TTSChunk>();
        TTSChunk ttsChunk = new TTSChunk();
        ttsChunk.setText("Text");
        ttsChunkVector.add(ttsChunk);
        Vector<String> vrSynonyms = new Vector<String>();
        vrSynonyms.add("name");
        Vector<AppHMIType> appHMITypeVector = new Vector<AppHMIType>();
        appHMITypeVector.add(AppHMIType.DEFAULT);
        String mediaAppName = "ngnMediaScreenAppName";

        RegisterAppInterface msg = RPCRequestFactory.buildRegisterAppInterface(
                syncMsgVersion, "appName", ttsChunkVector, mediaAppName, vrSynonyms, true,
                Language.AR_SA, Language.CS_CZ, appHMITypeVector, "appID", 1, null, new DeviceInfo());
        proxy.updateRegisterAppInterfaceParameters(msg);

        assertEquals(syncMsgVersion, proxy.getSyncMsgVersionRequest());
        assertEquals("appName", proxy.getApplicationName());
        assertEquals(ttsChunkVector, proxy.getTTSName());
        assertEquals(mediaAppName, proxy.getNgnMediaScreenAppName());
        assertEquals(vrSynonyms, proxy.getVrSynonyms());
        assertEquals((Boolean) true, proxy.getIsMediaApp());
        assertEquals(Language.AR_SA, proxy.getSyncLanguageDesired());
        assertEquals(Language.CS_CZ, proxy.getHmiDisplayLanguageDesired());
        assertEquals(appHMITypeVector, proxy.getAppHMIType());
        assertEquals("appID", proxy.getAppID());
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
        verify(proxy.getRPCMessageHandler(), times(1)).handleRPCMessage(any(Hashtable.class));
    }
}
