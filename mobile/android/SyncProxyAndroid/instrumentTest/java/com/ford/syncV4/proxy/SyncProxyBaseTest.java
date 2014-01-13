package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SyncInterfaceAvailability;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.TCPTransportConfig;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 1/13/14.
 */
public class SyncProxyBaseTest extends InstrumentationTestCase {

    public void testSyncProxyBaseHasServicePoolField() throws Exception {
        SyncMsgVersion syncMsgVersion = new SyncMsgVersion();
        syncMsgVersion.setMajorVersion(2);
        syncMsgVersion.setMinorVersion(2);
        TCPTransportConfig conf = mock(TCPTransportConfig.class);
        final IProxyListenerALM listenerALM = mock(IProxyListenerALM.class);
        SyncProxyBase proxyALM = new SyncProxyALM(listenerALM,
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
                        _syncConnection.closeConnection(_rpcSessionID, false);
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
            }
        };
        assertNotNull("should not be null", proxyALM.getServicePool());
    }

}
