package com.ford.syncV4.test.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SyncInterfaceAvailability;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

import java.util.Arrays;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 8/22/13.
 */
public class SyncProxyALMTest extends InstrumentationTestCase {

    private SyncProxyALM sut;

    public SyncProxyALMTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        sut = getSyncProxyALM();
    }

    private SyncProxyALM getSyncProxyALM() throws SyncException {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf  = mock(TCPTransportConfig.class);
        when(conf.getTransportType()).thenReturn(TransportType.TCP);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        return new SyncProxyALM(listenerALM,
							/*sync proxy configuration resources*/null,
							/*enable advanced lifecycle management true,*/
                "appName",
							/*ngn media app*/null,
							/*vr synonyms*/null,
							/*is media app*/true,
                syncMsgVersion,
							/*language desired*/Language.EN_US,
							/*HMI Display Language Desired*/Language.EN_US,
							/*App ID*/"8675308",
							/*autoActivateID*/null,
							/*callbackToUIThre1ad*/ false,
							/*preRegister*/ false,
                2,
                conf){


            @Override
            protected void initializeProxy() throws SyncException {
                // Reset all of the flags and state variables
                _haveReceivedFirstNonNoneHMILevel = false;
                _haveReceivedFirstFocusLevel = false;
                _haveReceivedFirstFocusLevelFull = false;
                _syncIntefaceAvailablity = SyncInterfaceAvailability.SYNC_INTERFACE_UNAVAILABLE;

                // Setup SyncConnection
                synchronized(CONNECTION_REFERENCE_LOCK) {
                    if (_syncConnection != null) {
                        _syncConnection.closeConnection(_rpcSessionID);
                        _syncConnection = null;
                    }
                    _syncConnection = mock(SyncConnection.class);
                    when(_syncConnection.getIsConnected()).thenReturn(true);
                }
                synchronized(CONNECTION_REFERENCE_LOCK) {
                    if (_syncConnection != null) {
                        _syncConnection.startTransport();
                    }
                }
            }
        };
    }

    public void testSyncProxyBaseShouldBeCreated() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf  = mock(TCPTransportConfig.class);
        when(conf.getTransportType()).thenReturn(TransportType.TCP);
        IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        SyncProxyALM syncProxy = new SyncProxyALM(listenerALM,
							/*sync proxy configuration resources*/null,
							/*enable advanced lifecycle management true,*/
                            "appName",
							/*ngn media app*/null,
							/*vr synonyms*/null,
							/*is media app*/true,
                            syncMsgVersion,
							/*language desired*/Language.EN_US,
							/*HMI Display Language Desired*/Language.EN_US,
							/*App ID*/"8675308",
							/*autoActivateID*/null,
							/*callbackToUIThre1ad*/ false,
							/*preRegister*/ false,
                            2,
                conf);
        assertNotNull("Sync Proxy should not be null", syncProxy);
    }

    public void testSendVideoFrameShouldAddFrameToQueue() throws Exception {
        final byte[] testRTP = new byte[10];
        try {
            boolean result = sut.sendVideoFrame(testRTP);
            assertTrue("Failed to queue message", result);
        }catch (SyncException e){
            assertNull("Should not get here",e);
        }
    }

    public void testMobileNavProtocolMessageCreation() throws Exception {
        final byte[] testRTP = new byte[10];
        ProtocolMessage message = sut.createMobileNavSessionProtocolMessage(testRTP);
        assertTrue(Arrays.equals(testRTP, message.getData()));
        assertEquals(sut.getMobileNavSessionID(), message.getSessionID());
        assertEquals(MessageType.VIDEO, message.getMessageType());
        assertEquals(SessionType.Mobile_Nav, message.getSessionType());
    }
}
