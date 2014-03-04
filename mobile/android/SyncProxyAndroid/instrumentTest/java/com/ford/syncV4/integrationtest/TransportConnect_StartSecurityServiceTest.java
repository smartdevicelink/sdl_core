package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;
import android.util.Log;

import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;

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
    private static final byte VERSION = (byte) 2;

    /**
     * Time out of the {@link java.util.concurrent.CountDownLatch}, in milliseconds
     */
    private static final int TIME_OUT = 1000;

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

    public void testOnTransportConnected_StartSecurityService() throws InterruptedException {
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
                                                 byte version, String correlationID) {

            }

            @Override
            public void onStartServiceNackReceived(ServiceType serviceType) {

            }

            @Override
            public void onSecureServiceStarted(byte version) {
                Log.d(LOG_TAG, "Secure Service started");
                assertEquals((byte) 2, version);
                passed[0] = true;
                countDownLatch.countDown();
            }

        });

        protocol.setVersion(VERSION);

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
                        .createStartSecureServiceACK((byte) 2);

                byte[] bytes = protocolFrameHeader.assembleHeaderBytes();

                protocol.HandleReceivedBytes(bytes, bytes.length);
            }
        };

        connection.init(mTcpTransportConfig, mock(SyncTransport.class), protocol);

        Log.d(LOG_TAG, "Start connect");
        connection.onTransportConnected();

        countDownLatch.await(TIME_OUT, TimeUnit.MILLISECONDS);

        assertTrue(passed[0]);
    }
}