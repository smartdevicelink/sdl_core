package com.ford.syncV4.syncConnection;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitor;
import com.ford.syncV4.session.Session;
import com.ford.syncV4.streaming.H264Packetizer;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TCPTransportConfig;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.BitConverter;

import org.mockito.ArgumentCaptor;

import java.io.OutputStream;
import java.util.Arrays;

import static org.hamcrest.CoreMatchers.containsString;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 8/22/13.
 */
public class SyncConnectionTest extends InstrumentationTestCase {

    public static final byte VERSION = (byte) 2;
    public static final byte SESSION_ID = (byte) 48;
    public static final int MESSAGE_ID = 48;

    private static final String LOG_TAG = "SyncConnectionTest";

    private SyncConnection sut;
    private TCPTransportConfig config;

    public SyncConnectionTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        config = mock(TCPTransportConfig.class);
        when(config.getTransportType()).thenReturn(TransportType.TCP);
        sut = new SyncConnection(mock(ISyncConnectionListener.class));
        sut.init(config);
        WiProProtocol protocol = (WiProProtocol) sut.getWiProProtocol();
        protocol.setProtocolVersion(VERSION);
    }

    public void testSyncConnectionShouldBeCreated() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        assertNotNull("should not be null", connection);
    }

    public void testStartMobileNavServiceShouldSendAppropriateBytes() throws Exception {
        final boolean[] passed = {false};
        byte sessionID = 0x0A;
        Session session = new Session();
        session.setSessionId(sessionID);
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(ServiceType.Mobile_Nav, sessionID, VERSION);
        header.setSessionID(sessionID);
        final ProtocolFrameHeader realHeader = header;
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length) {
                super.onProtocolMessageBytesToSend(msgBytes, offset, length);
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes, realHeader.assembleHeaderBytes()));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be 12", length, 12);
                passed[0] = true;
            }
        };
        connection.init(config);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(VERSION);
        connection.startMobileNavService(session);
        assertTrue(passed[0]);
    }

    public void testOnTransportBytesReceivedReturnedStartSessionACK() throws Exception {
        final boolean[] passed = {false};
        final ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(ServiceType.Mobile_Nav, SESSION_ID, MESSAGE_ID, VERSION);
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {

            @Override
            public void onProtocolServiceStarted(ServiceType serviceType, byte sessionID, byte version, String correlationID) {
                super.onProtocolServiceStarted(serviceType,sessionID, version, correlationID);
                assertEquals("Correlation ID is empty string so far", "", correlationID);
                assertEquals("ServiceType should be equal.", header.getServiceType(), serviceType);
                assertEquals("Frame headers should be equal.", header.getSessionID(), sessionID);
                assertEquals("Version should be equal.", header.getVersion(), version);
                passed[0] = true;
            }
        };
        connection.init(config);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(VERSION);
        connection.onTransportBytesReceived(header.assembleHeaderBytes(), header.assembleHeaderBytes().length);
        assertTrue(passed[0]);
    }

    public void testCloseMobileNavSessionShouldSendAppropriateBytes() throws Exception {
        final byte[] data = BitConverter.intToByteArray(0);
        final ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createEndSession(ServiceType.Mobile_Nav, SESSION_ID, 0, VERSION, data.length);
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {

            private int count = 0;

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
                if (count == 0) {
                    byte[] commonArray = new byte[msgBytes.length];
                    System.arraycopy(header.assembleHeaderBytes(), 0, commonArray, 0, header.assembleHeaderBytes().length);
                    System.arraycopy(data, 0, commonArray, header.assembleHeaderBytes().length, data.length);
                    assertTrue("Arrays should be equal", Arrays.equals(msgBytes, commonArray));
                    assertEquals("Offset should be 0", offset, 0);
                    assertEquals("Length should be 12", length, 16);
                    count++;
                }
            }
        };
        connection.init(config);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(VERSION);
        connection.closeMobileNaviService(SESSION_ID);
    }

    public void testStopTransportIsCalledForRPCService() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {
            @Override
            public Boolean getIsConnected() {
                _transport = mock(SyncTransport.class);
                return super.getIsConnected();
            }
        };
        connection.init(config);
        connection.getIsConnected();
        connection.onProtocolServiceEnded(ServiceType.RPC, SESSION_ID, "");
        verify(connection._transport, times(1)).stopReading();
    }

    public void testStopTransportNotCalledForNavigationService() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {
            @Override
            public Boolean getIsConnected() {
                _transport = mock(SyncTransport.class);
                return super.getIsConnected();
            }
        };
        connection.init(config);
        connection.getIsConnected();
        connection.onProtocolServiceEnded(ServiceType.Mobile_Nav, SESSION_ID, "");
        verify(connection._transport, never()).stopReading();

    }

    public void testStartAudioServiceShouldSendAppropriateBytes() throws Exception {
        final boolean[] isPassed = {false};
        byte sessionID = 0x0A;
        Session session = new Session();
        session.setSessionId(sessionID);
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(ServiceType.Audio_Service, sessionID, VERSION);
        header.setSessionID(sessionID);
        final ProtocolFrameHeader realHeader = header;
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class)) {

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length) {
                super.onProtocolMessageBytesToSend(msgBytes, offset, length);
                isPassed[0] = true;
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes, realHeader.assembleHeaderBytes()));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be 12", length, 12);
            }
        };
        connection.init(config);
        WiProProtocol protocol = (WiProProtocol) connection.getWiProProtocol();
        protocol.setProtocolVersion(VERSION);
        connection.startAudioService(session);
        assertTrue(isPassed[0]);
    }

    public void testStartAudioDataTransferReturnsOutputStream() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        OutputStream stream = connection.startAudioDataTransfer(SESSION_ID);
        assertNotNull("output stream should be created", stream);
    }

    public void testStartAudioDataTransferCreatesAudioPacketizer() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        OutputStream stream = connection.startAudioDataTransfer(SESSION_ID);
        assertNotNull("audio pacetizer should not be null", connection.mAudioPacketizer);
    }

/*
    // FIXME this test fails with
    // junit.framework.AssertionFailedError: expected:<RUNNABLE> but was:<WAITING>
    public void testStartAudioDataTransferStartsPacetizer() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        OutputStream stream = connection.startAudioDataTransfer(SESSION_ID);
        H264Packetizer packetizer = (H264Packetizer) connection.mAudioPacketizer;
        assertEquals(Thread.State.RUNNABLE, packetizer.getThread().getState());
    }
*/

    public void testStartAudioDataTransferSetsSessionID() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        OutputStream stream = connection.startAudioDataTransfer(SESSION_ID);
        H264Packetizer packetizer = (H264Packetizer) connection.mAudioPacketizer;
        assertEquals("session id should be equal SESSION_ID", SESSION_ID, packetizer.getSessionID());
    }

    public void testStopAudioDataTransferStopPacketizer() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        connection.mAudioPacketizer = mock(H264Packetizer.class);
        connection.stopAudioDataTransfer();
        verify(connection.mAudioPacketizer, times(1)).stop();
    }

    public void testCloseAudioServiceSendEndServiceMessage() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.init(config);
        connection._protocol = mock(WiProProtocol.class);
        connection._transport = mock(SyncTransport.class);
        when(connection._transport.getIsConnected()).thenReturn(true);
        connection.closeAudioService(SESSION_ID);
        ArgumentCaptor<ServiceType> serviceTypeCaptor = ArgumentCaptor.forClass(ServiceType.class);
        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        verify(connection._protocol, times(1)).EndProtocolService(serviceTypeCaptor.capture(), sessionIDCaptor.capture());
        assertEquals("should end audio service", ServiceType.Audio_Service, serviceTypeCaptor.getValue());
        assertEquals("should end session with SESSION_ID", SESSION_ID, sessionIDCaptor.getValue().byteValue());
    }

    // TODO : Reconsider this test case as onTransportConnected is now invoke another listener

    /*public void testStartSessionWithCorrectId() throws Exception {
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.setSessionId(SESSION_ID);
        connection.init(config);
        connection._protocol = mock(WiProProtocol.class);
        connection._transport = mock(SyncTransport.class);
        when(connection._transport.getIsConnected()).thenReturn(true);

        connection.onTransportConnected();

        ArgumentCaptor<Byte> sessionIDCaptor = ArgumentCaptor.forClass(byte.class);
        verify(connection._protocol, times(1)).StartProtocolSession(sessionIDCaptor.capture());
        assertEquals("Should start session with SESSION_ID", SESSION_ID, sessionIDCaptor.getValue().byteValue());
    }*/

    public void testOnCloseSessionAudioPacketizerStops() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.setSessionId(SESSION_ID);
        connection.init(config);
        connection._protocol = mock(WiProProtocol.class);
        connection._transport = mock(SyncTransport.class);
        connection.mAudioPacketizer = mock(H264Packetizer.class);
        when(connection._transport.getIsConnected()).thenReturn(true);
        connection.closeConnection(SESSION_ID, false);
        verify(connection.mAudioPacketizer, times(1)).stop();
    }

    public void testHeartbeatMonitorStoppedIfConnectionClosedWithoutKeepConnection() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        IHeartbeatMonitor heartbeatMonitor = mock(IHeartbeatMonitor.class);
        connection.setHeartbeatMonitor(heartbeatMonitor);
        assertNotNull(connection.getHeartbeatMonitor());
        connection.closeConnection((byte) 0, false, true);
        verify(heartbeatMonitor, times(1)).stop();
    }

    public void testHeartbeatMonitorNotStoppedIfConnectionClosedWithKeepConnection() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.setHeartbeatMonitor(mock(IHeartbeatMonitor.class));
        assertNotNull(connection.getHeartbeatMonitor());
        connection.closeConnection((byte) 0, true, true);
        verify(connection.getHeartbeatMonitor(), never()).stop();
        assertNotNull("heartbeat monitor should not be null",connection.getHeartbeatMonitor());
    }

    public void testHeartbeatMonitorResetOnHeartbeatReset() throws Exception {
        IHeartbeatMonitor heartbeatMonitor = mock(IHeartbeatMonitor.class);
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.setHeartbeatMonitor(heartbeatMonitor);
        connection.onResetHeartbeat();
        verify(heartbeatMonitor).notifyTransportActivity();
    }

    public void testHeartbeatSendDoNotResetHeartbeat() throws Exception {
        IHeartbeatMonitor heartbeatMonitor = mock(IHeartbeatMonitor.class);
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class));
        connection.sendHeartbeat(heartbeatMonitor);
        verify(heartbeatMonitor, never()).notifyTransportActivity();
    }

    public void testMaxJsonSizeInIncomingMessageShouldCallOnProtocolError() {
        final ISyncConnectionListener connectionListenerMock =
                mock(ISyncConnectionListener.class);
        SyncConnection connection = new SyncConnection(connectionListenerMock);
        connection.init(null, mock(SyncTransport.class));
        final WiProProtocol protocol = new WiProProtocol(connection);
        protocol.setProtocolVersion((byte) 0x02);
        connection._protocol = protocol;

        final byte maxByte = (byte) 0xFF;
        final byte[] bytes =
                { 0x21, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, maxByte, maxByte, maxByte, maxByte, 0x00 };
        connection.onTransportBytesReceived(bytes, bytes.length);

        ArgumentCaptor<Throwable> throwableArgumentCaptor =
                ArgumentCaptor.forClass(Throwable.class);
        verify(connectionListenerMock, timeout(100).times(1)).onProtocolError(
                anyString(), throwableArgumentCaptor.capture());
        assertThat(throwableArgumentCaptor.getValue().toString(),
                containsString(OutOfMemoryError.class.getSimpleName()));
    }
}
