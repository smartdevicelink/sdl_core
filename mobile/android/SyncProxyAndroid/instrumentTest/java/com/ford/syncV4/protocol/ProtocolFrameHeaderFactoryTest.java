package com.ford.syncV4.protocol;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.ServiceType;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Created by Andrew Batutin on 8/20/13.
 */
public class ProtocolFrameHeaderFactoryTest extends AndroidTestCase {
    static final byte SESSION_TYPE_RPC = 0x07;
    static final byte PROTOCOL_VERSION = 0x02;
    static final byte FRAME_HEARTBEAT = 0x00;
    static final byte FRAME_HEARTBEAT_ACK = (byte) 0xFF;

    public void testMobileNavStartSessionFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createMobileNavStartSession(48,
                        (byte) 48, (byte) 2);
        assertEquals(header.getServiceType(), ServiceType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(),
                FrameDataControlFrameType.StartService.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0x00);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testSessionStartACKFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createStartSessionACK(
                        ServiceType.RPC, (byte) 48, 48, (byte) 2);
        byte[] data = header.assembleHeaderBytes();
        assertNotNull("data should not be null", data);
    }

    public void testSessionStartNACKFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createStartSessionNACK(
                        ServiceType.Mobile_Nav, (byte) 48, 48, (byte) 2);
        byte[] data = header.assembleHeaderBytes();
        assertNotNull("data should not be null", data);
    }

    public void testMobileNavStartSessionACKFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createStartSessionACK(
                        ServiceType.Mobile_Nav, (byte) 48, 48, (byte) 2);
        assertEquals(header.getServiceType(), ServiceType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(),
                FrameDataControlFrameType.StartServiceACK.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0x00);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testMobileNavEndSessionFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createEndSession(
                        ServiceType.Mobile_Nav, (byte) 48, 48, (byte) 2, 0);
        assertEquals(header.getServiceType(), ServiceType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(),
                FrameDataControlFrameType.EndService.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0x00);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testMobileNavDataSessionFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createSingleSendData(
                        ServiceType.Mobile_Nav, (byte) 48, 10, 48, (byte) 2);
        assertEquals(header.getServiceType(), ServiceType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Single);
        assertEquals(header.getFrameData(), 0x00);
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 10);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testMobileNavAckFrameCreation() throws Exception {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createMobileNaviAck(
                        ServiceType.Mobile_Nav, (byte) 48, 48, (byte) 2);
        assertEquals(header.getServiceType(), ServiceType.Mobile_Nav);
        assertEquals(header.getFrameType(), FrameType.Control);
        assertEquals(header.getFrameData(),
                FrameDataControlFrameType.MobileNaviACK.getValue());
        assertEquals(header.getVersion(), (byte) 2);
        assertEquals(header.isCompressed(), false);
        assertEquals(header.getDataSize(), 0);
        assertTrue(header.getMessageID() == 48);
        assertTrue(header.getSessionID() == (byte) 48);
    }

    public void testCreatedHeartbeatShouldHaveCorrectBytes() {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createHeartbeat(
                ServiceType.valueOf(SESSION_TYPE_RPC), PROTOCOL_VERSION);

        final byte[] correct =
                new byte[]{ 0x20, SESSION_TYPE_RPC, FRAME_HEARTBEAT, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        assertThat("Wrong binary header", header.assembleHeaderBytes(),
                is(correct));
    }

    public void testCreatedHeartbeatShouldHaveCorrectHeaderValues() {
        ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createHeartbeat(
                ServiceType.Heartbeat, PROTOCOL_VERSION);

        assertThat("Wrong version", header.getVersion(), is(PROTOCOL_VERSION));
        assertThat("Wrong compression flag", header.isCompressed(), is(false));
        assertThat("Wrong frame type", header.getFrameType(),
                is(FrameType.Control));
        assertThat("Wrong currentSession type", header.getServiceType(),
                is(ServiceType.Heartbeat));
        assertThat("Wrong frame data", header.getFrameData(),
                is(FrameDataControlFrameType.Heartbeat.value()));
        assertThat("Wrong currentSession id", header.getSessionID(), is((byte) 0));
        assertThat("Wrong data size", header.getDataSize(), is(0));
        assertThat("Wrong message id", header.getMessageID(), is(0));
    }

    public void testCreatedHeartbeatACKShouldHaveCorrectBytes() {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createHeartbeatACK(
                        ServiceType.valueOf(SESSION_TYPE_RPC),
                        PROTOCOL_VERSION);

        final byte[] correct =
                new byte[]{ 0x20, SESSION_TYPE_RPC, FRAME_HEARTBEAT_ACK, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        assertThat("Wrong binary header", header.assembleHeaderBytes(),
                is(correct));
    }

    public void testCreatedHeartbeatACKShouldHaveCorrectHeaderValues() {
        ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createHeartbeatACK(
                        ServiceType.valueOf(SESSION_TYPE_RPC),
                        PROTOCOL_VERSION);

        assertThat("Wrong version", header.getVersion(), is(PROTOCOL_VERSION));
        assertThat("Wrong compression flag", header.isCompressed(), is(false));
        assertThat("Wrong frame type", header.getFrameType(),
                is(FrameType.Control));
        assertThat("Wrong currentSession type", header.getServiceType(),
                is(ServiceType.RPC));
        assertThat("Wrong frame data", header.getFrameData(),
                is(FrameDataControlFrameType.HeartbeatACK.value()));
        assertThat("Wrong currentSession id", header.getSessionID(), is((byte) 0));
        assertThat("Wrong data size", header.getDataSize(), is(0));
        assertThat("Wrong message id", header.getMessageID(), is(0));
    }
}
