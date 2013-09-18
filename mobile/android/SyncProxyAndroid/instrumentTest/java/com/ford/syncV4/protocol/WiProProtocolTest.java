package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.util.BitConverter;

import junit.framework.Assert;
import junit.framework.TestCase;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class WiProProtocolTest extends TestCase {

    public static final IProtocolListener DUMMY_PROTOCOL_LISTENER =
            new IProtocolListener() {
                @Override
                public void onProtocolMessageBytesToSend(byte[] msgBytes,
                                                         int offset,
                                                         int length) {
                }

                @Override
                public void onProtocolMessageReceived(ProtocolMessage msg) {
                }

                @Override
                public void onProtocolSessionStarted(SessionType sessionType,
                                                     byte sessionID,
                                                     byte version,
                                                     String correlationID) {
                }

                @Override
                public void onProtocolSessionEnded(SessionType sessionType,
                                                   byte sessionID,
                                                   String correlationID) {
                }

                @Override
                public void onProtocolHeartbeatPastDue(int heartbeatInterval_ms,
                                                       int pastDue_ms) {
                }

                @Override
                public void onProtocolError(String info, Exception e) {
                }
            };
    private static final String TAG = WiProProtocolTest.class.getSimpleName();

    public byte[] extractByteArrayPart(final byte[] src, int offset,
                                       int length) {
        final byte[] dst = new byte[length];
        System.arraycopy(src, offset, dst, 0, length);
        return dst;
    }

    public void testReadingHashIDFromStartSessionACK() throws Throwable {
        // null as a listener won't work
        final WiProProtocol protocol =
                new WiProProtocol(DUMMY_PROTOCOL_LISTENER);
        final ByteArrayOutputStream StartSessionACKMessageStream =
                new ByteArrayOutputStream(12);

        final byte[] msgFirstBytes = new byte[]{ 0x20, 0x07, 0x02, 0x01 };
        StartSessionACKMessageStream.write(msgFirstBytes);
        final byte[] msgDataSize = new byte[]{ 0x00, 0x00, 0x00, 0x00 };
        StartSessionACKMessageStream.write(msgDataSize);
        final byte[] msgHashID =
                new byte[]{ 0x12, 0x34, (byte) 0xCD, (byte) 0xEF };
        StartSessionACKMessageStream.write(msgHashID);

        final byte[] StartSessionACKMessage =
                StartSessionACKMessageStream.toByteArray();
        protocol.HandleReceivedBytes(StartSessionACKMessage,
                StartSessionACKMessage.length);
        Assert.assertEquals("HashID is incorrect", 0x1234CDEF, protocol.hashID);
    }

    public void testSendingHashIDWithEndSession() throws IOException {
        final IProtocolListener protocolListener = new IProtocolListener() {
            private int sendCount = 0;

            @Override
            public void onProtocolMessageBytesToSend(byte[] msgBytes,
                                                     int offset, int length) {
                // This method is called twice, sending header and then data
                final byte[] expectedMsgMessageID =
                        new byte[]{ (byte) 0xCD, (byte) 0xEF, 0x12, 0x34 };

                switch (sendCount) {
                    case 0:
                        final byte[] msgDataSize =
                                extractByteArrayPart(msgBytes, 4, 4);
                        final byte[] expectedMsgDataSize =
                                new byte[]{ 0x00, 0x00, 0x00, 0x04 };
                        Assert.assertEquals("Data Size is incorrect",
                                BitConverter
                                        .intFromByteArray(expectedMsgDataSize,
                                                0),
                                BitConverter.intFromByteArray(msgDataSize, 0));

                        final byte[] msgMessageID =
                                extractByteArrayPart(msgBytes, 8, 4);
                        Assert.assertEquals("Message ID should be hash ID",
                                BitConverter
                                        .intFromByteArray(expectedMsgMessageID,
                                                0),
                                BitConverter.intFromByteArray(msgMessageID, 0));
                        break;

                    case 1:
                        final byte[] msgData =
                                extractByteArrayPart(msgBytes, 0, 4);
                        final byte[] expectedMsgData = expectedMsgMessageID;
                        assertEquals("Data should contain hash ID", BitConverter
                                .intFromByteArray(expectedMsgData, 0),
                                BitConverter.intFromByteArray(msgData, 0));
                        break;

                    default:
                        Assert.assertTrue(String.format(
                                "onProtocolMessageBytesToSend is called too many times: %d",
                                sendCount), false);
                }

                ++sendCount;
            }

            @Override
            public void onProtocolMessageReceived(ProtocolMessage msg) {
            }

            @Override
            public void onProtocolSessionStarted(SessionType sessionType,
                                                 byte sessionID, byte version,
                                                 String correlationID) {
            }

            @Override
            public void onProtocolSessionEnded(SessionType sessionType,
                                               byte sessionID,
                                               String correlationID) {
            }

            @Override
            public void onProtocolHeartbeatPastDue(int heartbeatInterval_ms,
                                                   int pastDue_ms) {
            }

            @Override
            public void onProtocolError(String info, Exception e) {
            }
        };

        final WiProProtocol protocol = new WiProProtocol(protocolListener);
        protocol.hashID = 0xCDEF1234;
        protocol._version = 2;
        protocol.EndProtocolSession(SessionType.RPC, (byte) 0x01);
    }
}
