package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

import java.lang.reflect.Field;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 3/4/14
 * Time: 1:10 PM
 */
public class TransportConnect_StartSecurityServiceTest extends InstrumentationTestCase {

    private static final String LOG_TAG = "TransportConnect_StartSecurityServiceTest";

    /**
     * Time out of the {@link java.util.concurrent.CountDownLatch}, in milliseconds
     */
    private static final int TIME_OUT = 1000;

    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final byte SESSION_ID = (byte) 1;
    private static final String CORRELATION_ID = "1234567890";

    private TCPTransportConfig mTcpTransportConfig;
    private SyncConnection mSyncConnection;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext()
                .getCacheDir().getPath());
        mTcpTransportConfig = mock(TCPTransportConfig.class);
        when(mTcpTransportConfig.getTransportType()).thenReturn(TransportType.TCP);
        mSyncConnection = new SyncConnection(mock(ISyncConnectionListener.class));
        mSyncConnection.init(mTcpTransportConfig);
    }



    private void emulateProtocolSessionStarted(SyncProxyALM proxy) throws NoSuchFieldException,
                                                                   IllegalAccessException {
        final Field interfaceBroker = SyncProxyBase.class.getDeclaredField("_interfaceBroker");
        interfaceBroker.setAccessible(true);
        SyncProxyBase.SyncInterfaceBroker broker =
                (SyncProxyBase.SyncInterfaceBroker) interfaceBroker.get(proxy);

        Session session = new Session();
        session.setSessionId(SESSION_ID);

        Service rpcService = new Service();
        rpcService.setServiceType(ServiceType.RPC);
        session.addService(rpcService);

        broker.onProtocolSessionStarted(session, PROTOCOL_VERSION, CORRELATION_ID);
    }
}