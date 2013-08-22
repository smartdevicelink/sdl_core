package com.ford.syncV4.test.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.SyncMsgVersion;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 8/22/13.
 */
public class SyncProxyALMTest extends InstrumentationTestCase {

    public SyncProxyALMTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
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
}
