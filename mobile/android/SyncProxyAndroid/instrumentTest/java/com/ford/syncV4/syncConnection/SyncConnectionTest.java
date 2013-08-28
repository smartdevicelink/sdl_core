package com.ford.syncV4.syncConnection;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.transport.BaseTransportConfig;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.util.BitConverter;

import java.util.Arrays;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 8/22/13.
 */
public class SyncConnectionTest extends InstrumentationTestCase {

    public static final byte VERSION = (byte) 2;
    public static final byte SESSION_ID = (byte) 48;
    public static final int MESSAGE_ID = 48;
    private SyncConnection sut;

    public SyncConnectionTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        sut = new SyncConnection(mock(ISyncConnectionListener.class), mock(BaseTransportConfig.class));
        WiProProtocol protocol = (WiProProtocol)sut.getWiProProtocol();
        protocol.setVersion(VERSION);
    }

    public void testSyncConnectionShouldBeCreated() throws Exception {
        SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class), mock(BaseTransportConfig.class));
        assertNotNull("should not be null", connection);
    }

    public void testStartMobileNavSessionShouldSendAppropriateBytes() throws Exception{
        final ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(SessionType.Mobile_Nav, 0x00, VERSION);
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class), mock(BaseTransportConfig.class)){

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length){
                super.onProtocolMessageBytesToSend(msgBytes,offset,length);
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes, header.assembleHeaderBytes()));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be 12", length, 12);
            }
        };
        WiProProtocol protocol = (WiProProtocol)connection.getWiProProtocol();
        protocol.setVersion(VERSION);
        connection.startMobileNavSession();
    }

    public void testOnTransportBytesReceivedReturnedStartSessionACK() throws Exception {
        final ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(SessionType.Mobile_Nav, SESSION_ID, MESSAGE_ID, VERSION);
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class), mock(BaseTransportConfig.class)){

            @Override
            public void onProtocolSessionStarted(SessionType sessionType, byte sessionID, byte version, String correlationID) {
                super.onProtocolSessionStarted(sessionType, sessionID, version, correlationID);
                assertEquals("Correlation ID is empty string so far", "",correlationID);
                assertEquals("SessionType should be equal.", header.getSessionType(), sessionType);
                assertEquals("Frame headers should be equal.", header.getSessionID(),sessionID);
                assertEquals("Version should be equal.", header.getVersion(), version);
            }
        };
        WiProProtocol protocol = (WiProProtocol)connection.getWiProProtocol();
        protocol.setVersion(VERSION);
        connection.onTransportBytesReceived(header.assembleHeaderBytes(), header.assembleHeaderBytes().length);
    }

    public void testCloseMobileNavSessionShouldSendAppropriateBytes() throws Exception {
        byte[] data = BitConverter.intToByteArray(0xCDEF1234);
        final ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createEndSession(SessionType.Mobile_Nav, SESSION_ID, 0, VERSION, data.length);
        final SyncConnection connection = new SyncConnection(mock(ISyncConnectionListener.class), mock(BaseTransportConfig.class)){

            @Override
            public void closeMobileNavSession(byte rpcSessionID) {
                _transport = mock(SyncTransport.class);
                when(_transport.getIsConnected()).thenReturn(true);
                super.closeMobileNavSession(rpcSessionID);
            }

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes, int offset,
                                                     int length){
                super.onProtocolMessageBytesToSend(msgBytes,offset,length);
                assertTrue("Arrays should be equal", Arrays.equals(msgBytes, header.assembleHeaderBytes()));
                assertEquals("Offset should be 0", offset, 0);
                assertEquals("Length should be 12", length, 12);
            }
        };
        WiProProtocol protocol = (WiProProtocol)connection.getWiProProtocol();
        protocol.setVersion(VERSION);
        connection.closeMobileNavSession(SESSION_ID);

    }
}
