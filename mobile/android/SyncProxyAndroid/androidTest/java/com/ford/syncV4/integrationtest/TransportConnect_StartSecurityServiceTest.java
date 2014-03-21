package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.service.Service;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.TestConfig;

import java.lang.reflect.Field;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

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

    public void testOnTransportConnected_StartSecurityService() throws InterruptedException, SyncException, NoSuchFieldException, IllegalAccessException {
        final boolean[] passed = {false};
        final CountDownLatch countDownLatch = new CountDownLatch(1);
        final WiProProtocol protocol = new WiProProtocol(new IProtocolListener() {
            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset, int length) {

            }

            @Override
            public void onProtocolMessageReceived(ProtocolMessage msg) {

            }

            @Override
            public void onProtocolSessionStarted(Session session, byte version,
                                                 String correlationID) {

            }

            @Override
            public void onProtocolServiceEnded(ServiceType serviceType, byte sessionID,
                                               String correlationID) {

            }

            @Override
            public void onProtocolHeartbeatACK() {

            }

            @Override
            public void onResetHeartbeat() {

            }

            @Override
            public void onProtocolError(String info, Exception e) {

            }

            @Override
            public void onMobileNavAckReceived(int frameReceivedNumber) {

            }

            @Override
            public void onProtocolAppUnregistered() {

            }

            @Override
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID,
                                                 boolean encrypted, byte version, String correlationID) {

            }

            @Override
            public void onStartServiceNackReceived(ServiceType serviceType) {

            }

            @Override
            public void onSecureServiceStarted(byte version) {
                Log.d(LOG_TAG, "Secure Service started");
                assertEquals(PROTOCOL_VERSION, version);
                passed[0] = true;
                countDownLatch.countDown();
            }

        });

        protocol.setVersion(PROTOCOL_VERSION);

        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {
            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset, int length) {
                super.onProtocolMessageBytesToSend(msgBytes, offset, length);

                Log.d(LOG_TAG, "Send bytes");

                // Simulate Network delay
                try {
                    Thread.sleep(20);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                ProtocolFrameHeader protocolFrameHeader = ProtocolFrameHeaderFactory
                        .createStartSecureServiceACK(PROTOCOL_VERSION);

                byte[] bytes = protocolFrameHeader.assembleHeaderBytes();

                protocol.HandleReceivedBytes(bytes, bytes.length);
            }
        };

        connection.init(mTcpTransportConfig, mock(SyncTransport.class), protocol);

        IProxyListenerALMTesting proxyListenerMock = mock(IProxyListenerALMTesting.class);

        SyncProxyALM proxy = new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                false, null, null, null, null, null, null, false, false, 2,
                null, connection, new TestConfig());
        proxy.setServiceToCypher(ServiceType.RPC);
        emulateProtocolSessionStarted(proxy);

        countDownLatch.await(TIME_OUT, TimeUnit.MILLISECONDS);

        assertTrue(passed[0]);
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