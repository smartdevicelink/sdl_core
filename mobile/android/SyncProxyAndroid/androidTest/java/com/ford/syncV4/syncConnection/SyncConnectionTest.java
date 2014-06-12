package com.ford.syncV4.syncConnection;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.AbstractProtocol;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.heartbeat.HeartbeatBuilder;
import com.ford.syncV4.protocol.heartbeat.HeartbeatMonitor;
import com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitor;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.streaming.H264Packetizer;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.transport.usb.USBTransportConfig;
import com.ford.syncV4.util.BitConverter;

import org.mockito.ArgumentCaptor;

import java.io.OutputStream;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.containsString;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 8/22/13
 */
public class SyncConnectionTest extends InstrumentationTestCase {

    public static final int MESSAGE_ID = 48;

    private static final String LOG_TAG = "SyncConnectionTest";

    private SyncConnection mSyncConnection;
    private TCPTransportConfig mTransportConfig;

    public SyncConnectionTest() {

    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache",
                getInstrumentation().getTargetContext().getCacheDir().getPath());
        mTransportConfig = mock(TCPTransportConfig.class);
        when(mTransportConfig.getTransportType()).thenReturn(TransportType.TCP);
        mSyncConnection = new SyncConnection(new Session(), mock(ISyncConnectionListener.class));
        mSyncConnection.init(mTransportConfig);
        WiProProtocol protocol = (WiProProtocol) mSyncConnection.getWiProProtocol();
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
    }

    public void testSyncConnectionShouldBeCreated() throws Exception {
        SyncConnection connection = new SyncConnection(new Session(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        assertNotNull("should not be null", connection);
    }

    public void testStartMobileNavServiceShouldSendAppropriateBytes() throws Exception {
        final boolean[] passed = {false};
        Session session = SessionTest.getInitializedSession();
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(
                ServiceType.Mobile_Nav, session.getSessionIdByAppId(SessionTest.APP_ID),
                ProtocolConstants.PROTOCOL_VERSION_TWO, false);

        final ProtocolFrameHeader realHeader = header;
        final SyncConnection connection = new SyncConnection(session,
                mock(ISyncConnectionListener.class)) {

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length) {
                super.onProtocolMessageBytesToSend(msgBytes, offset, length);
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes,
                        realHeader.assembleHeaderBytes()));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be " + ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2,
                        length, ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);
                passed[0] = true;
            }
        };
        connection.init(mTransportConfig);
        when(connection.getIsConnected()).thenReturn(true);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        protocol.StartProtocolService(ServiceType.Mobile_Nav, session.getSessionIdByAppId(
                SessionTest.APP_ID), false);

        // wait for processing
        Thread.sleep(50);

        assertTrue(passed[0]);
    }

    public void testOnTransportBytesReceivedReturnedStartSessionACK() throws Exception {
        final boolean[] passed = {false};
        final ProtocolFrameHeader header = ProtocolFrameHeaderFactory.
                createStartSessionACK(ServiceType.Mobile_Nav, SessionTest.SESSION_ID, MESSAGE_ID,
                        ProtocolConstants.PROTOCOL_VERSION_TWO);
        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class)) {

            @Override
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, boolean encrypted,
                                                 byte version) {
                super.onProtocolServiceStarted(serviceType,sessionID,encrypted, version);
                assertEquals("ServiceType should be equal.", header.getServiceType(), serviceType);
                assertEquals("Frame headers should be equal.", header.getSessionID(), sessionID);
                assertEquals("Version should be equal.", header.getVersion(), version);
                passed[0] = true;
            }
        };
        connection.init(mTransportConfig);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        protocol.HandleReceivedBytes(header.assembleHeaderBytes(),
                header.assembleHeaderBytes().length);
        assertTrue(passed[0]);
    }

    public void testCloseMobileNavSessionShouldSendAppropriateBytes() throws Exception {
        final boolean[] result = {false};
        final CountDownLatch countDownLatch = new CountDownLatch(1);
        final Session session = SessionTest.getInitializedSession();
        final byte[] data = BitConverter.intToByteArray(0);
        final ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createEndSession(ServiceType.Mobile_Nav,
                        SessionTest.SESSION_ID, 0,
                        ProtocolConstants.PROTOCOL_VERSION_TWO, data.length);
        final SyncConnection connection = new SyncConnection(session,
                mock(ISyncConnectionListener.class)) {

            @Override
            public void closeMobileNaviService(byte rpcSessionID) {
                _transport = mock(SyncTransport.class);
                when(_transport.getIsConnected()).thenReturn(true);
                super.closeMobileNaviService(rpcSessionID);
            }

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length) {
                super.onProtocolMessageBytesToSend(msgBytes, offset, length);

                result[0] = true;

                byte[] commonArray = new byte[msgBytes.length];
                System.arraycopy(header.assembleHeaderBytes(), 0, commonArray, 0,
                        header.assembleHeaderBytes().length);
                System.arraycopy(data, 0, commonArray, header.assembleHeaderBytes().length,
                        data.length);
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes, commonArray));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be 12", length, 16);

                countDownLatch.countDown();
            }
        };
        connection.init(mTransportConfig);

        WiProProtocol wiProProtocol = new WiProProtocol(mock(IProtocolListener.class));
        WiProProtocol wiProProtocolSpy = spy(wiProProtocol);

        when(wiProProtocolSpy.getHashIdBySessionId(SessionTest.SESSION_ID)).thenReturn(0);

        connection.closeMobileNaviService(SessionTest.SESSION_ID);

        countDownLatch.await(1, TimeUnit.SECONDS);

        assertTrue(result[0]);
    }

    public void testStopTransportIsCalledForRPCService() throws Exception {
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class)) {
            @Override
            public Boolean getIsConnected() {
                _transport = mock(SyncTransport.class);
                return super.getIsConnected();
            }
        };
        connection.init(mTransportConfig);
        connection.getIsConnected();
        connection.onProtocolServiceEndedAck(ServiceType.RPC, SessionTest.SESSION_ID);
        verify(connection._transport, times(1)).stopReading();
    }

    public void testStopTransportNotCalledForNavigationService() throws Exception {
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class)) {
            @Override
            public Boolean getIsConnected() {
                _transport = mock(SyncTransport.class);
                return super.getIsConnected();
            }
        };
        connection.init(mTransportConfig);
        connection.getIsConnected();
        connection.onProtocolServiceEnded(ServiceType.Mobile_Nav, SessionTest.SESSION_ID);
        verify(connection._transport, never()).stopReading();

    }

    public void testStartAudioServiceShouldSendAppropriateBytes() throws Exception {
        final boolean[] isPassed = {false};
        Session session = SessionTest.getInitializedSession();
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createStartSession(ServiceType.Audio_Service,
                        session.getSessionIdByAppId(SessionTest.APP_ID),
                        ProtocolConstants.PROTOCOL_VERSION_TWO, false);
        final ProtocolFrameHeader realHeader = header;
        final SyncConnection connection = new SyncConnection(session,
                mock(ISyncConnectionListener.class)) {

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length) {
                super.onProtocolMessageBytesToSend(msgBytes, offset, length);
                isPassed[0] = true;
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes,
                        realHeader.assembleHeaderBytes()));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be " + ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2,
                        length, ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);
            }
        };
        connection.init(mTransportConfig);
        when(connection.getIsConnected()).thenReturn(true);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_THREE);
        protocol.StartProtocolService(ServiceType.Audio_Service,
                session.getSessionIdByAppId(SessionTest.APP_ID), false);
        // wait for processing
        Thread.sleep(50);
        assertTrue(isPassed[0]);
    }

    public void testStartAudioDataTransferReturnsOutputStream() throws Exception {
        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        OutputStream stream = connection.startAudioDataTransfer(SessionTest.SESSION_ID, false);
        assertNotNull("output stream should be created", stream);
    }

    public void testStartAudioDataTransferCreatesAudioPacketizer() throws Exception {
        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        OutputStream stream = connection.startAudioDataTransfer(SessionTest.SESSION_ID, false);
        assertNotNull("audio pacetizer should not be null", connection.mAudioPacketizer);
    }

/*
    // FIXME this test fails with
    // junit.framework.AssertionFailedError: expected:<RUNNABLE> but was:<WAITING>
    public void testStartAudioDataTransferStartsPacetizer() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        OutputStream stream = connection.startAudioDataTransfer(SESSION_ID);
        H264Packetizer packetizer = (H264Packetizer) connection.mAudioPacketizer;
        assertEquals(Thread.State.RUNNABLE, packetizer.getThread().getState());
    }
*/

    public void testStartAudioDataTransferSetsSessionID() throws Exception {
        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        OutputStream stream = connection.startAudioDataTransfer(SessionTest.SESSION_ID, false);
        H264Packetizer packetizer = (H264Packetizer) connection.mAudioPacketizer;
        assertEquals("session id should be equal SESSION_ID", SessionTest.SESSION_ID,
                packetizer.getSessionID());
    }

    public void testStopAudioDataTransferStopPacketizer() throws Exception {
        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        connection.mAudioPacketizer = mock(H264Packetizer.class);
        connection.stopAudioDataTransfer();
        verify(connection.mAudioPacketizer, times(1)).stop();
    }

    public void testCloseAudioServiceSendEndServiceMessage() throws Exception {
        final SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        connection._protocol = mock(WiProProtocol.class);
        connection._transport = mock(SyncTransport.class);
        when(connection._transport.getIsConnected()).thenReturn(true);
        connection.closeAudioService(SessionTest.SESSION_ID);
        ArgumentCaptor<ServiceType> serviceTypeCaptor = ArgumentCaptor.forClass(ServiceType.class);
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        verify(connection._protocol, times(1)).EndProtocolService(serviceTypeCaptor.capture(),
                sessionIDCaptor.capture());
        assertEquals("should end audio service", ServiceType.Audio_Service,
                serviceTypeCaptor.getValue());
        assertEquals("should end session with SESSION_ID", SessionTest.SESSION_ID,
                sessionIDCaptor.getValue().byteValue());
    }

    public void testOnCloseSessionAudioPacketizerStops() throws Exception {
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection.init(mTransportConfig);
        connection._protocol = mock(WiProProtocol.class);
        connection._transport = mock(SyncTransport.class);
        connection.mAudioPacketizer = mock(H264Packetizer.class);
        when(connection._transport.getIsConnected()).thenReturn(true);
        connection.closeConnection(false);
        verify(connection.mAudioPacketizer, times(1)).stop();
    }

    public void testHeartbeatMonitorStoppedIfConnectionClosedWithoutKeepConnection()
            throws Exception {
        IHeartbeatMonitor heartbeatMonitor = HeartbeatBuilder.buildHeartbeatMonitor(
                SessionTest.SESSION_ID, 5000, true);
        IHeartbeatMonitor heartbeatMonitorSpy = spy(heartbeatMonitor);
        Session session = SessionTest.getInitializedSession();
        ISyncConnectionListener listener = mock(ISyncConnectionListener.class);
        SyncConnection connection = new SyncConnection(session, listener);
        connection.init(new USBTransportConfig(getInstrumentation().getTargetContext()));

        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        SyncProxyALM syncProxyALM = new SyncProxyALM(proxyListenerMock, null, "!", null, null, true,
                null, null, null, null, SessionTest.APP_ID, null, false, false,
                ProtocolConstants.PROTOCOL_VERSION_TWO, null, connection, new TestConfig());

        syncProxyALM.initializeSession(SessionTest.APP_ID);
        syncProxyALM.getInterfaceBroker().onProtocolSessionStarted(SessionTest.SESSION_ID,
                ProtocolConstants.PROTOCOL_VERSION_TWO);

        connection.setHeartbeatMonitor(heartbeatMonitorSpy);
        assertNotNull(connection.getHeartbeatMonitor(heartbeatMonitor.getSessionId()));

        syncProxyALM.getInterfaceBroker().onTransportDisconnected("");
        verify(heartbeatMonitorSpy, times(1)).stop();
    }

    public void testHeartbeatMonitorNotStoppedIfConnectionClosedWithKeepConnection() throws Exception {
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        IHeartbeatMonitor heartbeatMonitor = mock(HeartbeatMonitor.class);
        connection.setHeartbeatMonitor(heartbeatMonitor);
        assertNotNull(connection.getHeartbeatMonitor(heartbeatMonitor.getSessionId()));
        connection.closeConnection(true);
        verify(connection.getHeartbeatMonitor(heartbeatMonitor.getSessionId()), never()).stop();
        assertNotNull("HB monitor should not be null",connection.getHeartbeatMonitor(heartbeatMonitor.getSessionId()));
    }

    public void testHeartbeatMonitorResetOnHeartbeatReset() throws Exception {
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        IHeartbeatMonitor heartbeatMonitor = mock(HeartbeatMonitor.class);
        connection.setHeartbeatMonitor(heartbeatMonitor);
        connection.onResetHeartbeatAck(heartbeatMonitor.getSessionId());
        verify(heartbeatMonitor).notifyTransportOutputActivity();
    }

    public void testHeartbeatSendDoNotResetHeartbeat() throws Exception {
        IHeartbeatMonitor heartbeatMonitor = mock(IHeartbeatMonitor.class);
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                mock(ISyncConnectionListener.class));
        connection._protocol = mock(AbstractProtocol.class);
        when(connection._protocol.getProtocolVersion()).thenReturn((byte) 3);
        connection.sendHeartbeat(heartbeatMonitor);
        verify(heartbeatMonitor, never()).notifyTransportOutputActivity();
    }

    public void testMaxJsonSizeInIncomingMessageShouldCallOnProtocolError() {
        final ISyncConnectionListener connectionListenerMock =
                mock(ISyncConnectionListener.class);
        SyncConnection connection = new SyncConnection(SessionTest.getInitializedSession(),
                connectionListenerMock);
        connection.init(null, mock(SyncTransport.class));
        final WiProProtocol protocol = new WiProProtocol(connection);
        protocol.setProtocolVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        connection._protocol = protocol;
        when(connection.getIsConnected()).thenReturn(true);

        final byte maxByte = (byte) 0xFF;
        final byte[] bytes =
                {0x21, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, maxByte, maxByte, maxByte, maxByte, 0x00};
        connection.onTransportBytesReceived(bytes, bytes.length);

        ArgumentCaptor<Throwable> throwableArgumentCaptor =
                ArgumentCaptor.forClass(Throwable.class);
        verify(connectionListenerMock, timeout(100).times(1)).onProtocolError(
                anyString(), throwableArgumentCaptor.capture());
        assertThat(throwableArgumentCaptor.getValue().toString(),
                containsString(OutOfMemoryError.class.getSimpleName()));
    }
}
