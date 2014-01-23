package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SyncInterfaceAvailability;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.TCPTransportConfig;

import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

import java.io.OutputStream;
import java.io.PipedOutputStream;
import java.util.List;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 1/13/14.
 */
public class SyncProxyBaseTest extends InstrumentationTestCase {

    private static byte sessionID = (byte) 1;
    public static final byte VERSION = (byte) 2;
    private IProxyListenerALM listenerALM;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        listenerALM = mock(IProxyListenerALM.class);
    }

    public void testSyncProxyBaseHasServicePoolField() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        assertNotNull("should not be null", proxyALM.getServicePool());
    }

    public void testMobileNavSessionAddedToServicePoolOnStart() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, VERSION, "");
        List<Service> serviceList = proxyALM.getServicePool();
        assertEquals("service pool should have mobile nav service", ServiceType.Mobile_Nav, serviceList.get(0).getServiceType());
        assertEquals(sessionID, serviceList.get(0).getSession().getSessionId());
    }

    public void testMobileNavSessionRemovedFromPoolListOnStop() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, VERSION, "");
        proxyALM.stopMobileNaviService();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testMobileNavServiceEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, VERSION, "");
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testRPCServiceAddedToPoolOnStart() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        assertEquals("pool should has RPC service", 1, proxyALM.getServicePool().size());
    }

    public void testRPCServiceEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.dispose();
        assertEquals("pool should be empty", 0, proxyALM.getServicePool().size());
    }

    public void testAllServicesEndedOnDispose() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, sessionID, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, sessionID, VERSION, "");
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

        return new SyncProxyALM(listenerALM,
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
                conf) {
            @Override
            protected void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                _haveReceivedFirstNonNoneHMILevel = false;
                _haveReceivedFirstFocusLevel = false;
                _haveReceivedFirstFocusLevelFull = false;
                _syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (_syncConnection != null) {
                        _syncConnection.closeConnection(currentSession.getSessionId(), false);
                        _syncConnection = null;
                    }
                    _syncConnection = mock(SyncConnection.class);
                    when(_syncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized (CONNECTION_REFERENCE_LOCK) {
                    if (_syncConnection != null) {
                        _syncConnection.startTransport();
                    }
                }
                currentSession.setSessionId(sessionID);
            }
        };
    }

    public void testOnAudioServiceStartServiceAddedToPool() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), VERSION, "");
        Service audioService = new Service();
        audioService.setSession(session);
        audioService.setServiceType(ServiceType.Audio_Service);
        assertTrue("pool should have AudioService ", proxyALM.getServicePool().contains(audioService));
    }

    public void testOnAudioServiceStartServiceCallbackCalled() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), VERSION, "");
        Mockito.verify(listenerALM, times(1)).onAudioServiceStart();
    }

    public void testAudioServiceRemovedFromPoolOnStopAudioService() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Mobile_Nav, session.getSessionId(), VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), VERSION, "");
        proxyALM.stopAudioService();
        Service mobileNaviService = new Service();
        mobileNaviService.setSession(session);
        mobileNaviService.setServiceType(ServiceType.Mobile_Nav);
        assertTrue("pool should have Mobile nav service ", proxyALM.getServicePool().contains(mobileNaviService));
        Service audioService = new Service();
        audioService.setSession(session);
        audioService.setServiceType(ServiceType.Audio_Service);
        assertFalse("pool should not have Audio service ", proxyALM.getServicePool().contains(audioService));
    }

    public void testStartAudioDataTransferClassConnectionMethod() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM._syncConnection = mock(SyncConnection.class);
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), VERSION, "");
        proxyALM.startAudioDataTransfer();
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        verify(proxyALM._syncConnection, times(1)).startAudioDataTransfer(sessionIDCaptor.capture());
    }

    public void testStartAudioDataTransferReturnsStream() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM._syncConnection = mock(SyncConnection.class);
        when(proxyALM._syncConnection.startAudioDataTransfer(sessionID)).thenReturn(new PipedOutputStream());
        Session session = Session.createSession(ServiceType.RPC, sessionID);
        proxyALM.getInterfaceBroker().onProtocolSessionStarted(session, VERSION, "");
        proxyALM.getInterfaceBroker().onProtocolServiceStarted(ServiceType.Audio_Service, session.getSessionId(), VERSION, "");
        OutputStream stream = proxyALM.startAudioDataTransfer();
        assertNotNull("stream should not be null" ,stream);
    }

    public void testStopAudioDataTransferFiresCallback() throws Exception {
        SyncProxyBase proxyALM = getSyncProxyBase();
        proxyALM._syncConnection = mock(SyncConnection.class);
        proxyALM.stopAudioDataTransfer();
        verify(proxyALM._syncConnection, timeout(1)).stopAudioDataTransfer();
    }
}
