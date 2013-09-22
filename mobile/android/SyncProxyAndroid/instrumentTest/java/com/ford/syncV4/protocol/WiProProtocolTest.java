package com.ford.syncV4.protocol;

import android.test.InstrumentationTestCase;
import android.util.Log;

import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.util.BitConverter;

import junit.framework.Assert;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Hashtable;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyByte;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;


/**
 * Created by Andrew Batutin on 8/21/13.
 */
public class WiProProtocolTest extends InstrumentationTestCase {

    public static final byte VERSION = (byte) 2;
    public static final int MESSAGE_ID = 1;
    public static final byte SESSION_ID = (byte) 48;
    public static final byte FRAME_SEQUENCE_NUMBER = (byte) 1;
    public static final int FRAME_SIZE_SHIFT = 100;
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

                @Override
                public void onMobileNavAckReceived(int frameReceivedNumber) {

                }

                @Override
                public void onProtocolAppUnregistered() {

                }
            };
    private static final String TAG = WiProProtocolTest.class.getSimpleName();
    Method currentCheckMethod;
    private WiProProtocol sut;
    private ProtocolFrameHeader currentFrameHeader;
    private byte[] currentData;

    public WiProProtocolTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        sut = new WiProProtocol(mock(IProtocolListener.class)) {


            @Override
            public void SendMessage(ProtocolMessage protocolMsg) {
                prepareMockItems();
                super.SendMessage(protocolMsg);
            }

            private void prepareMockItems() {
                _messageLocks = mock(Hashtable.class);
                when(_messageLocks.get(anyByte())).thenReturn("mockLock");
                doThrow(new IllegalStateException("should not get protocol error")).when(_protocolListener).onProtocolError(anyString(), any(Exception.class));
            }

            @Override
            protected void handleProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset, int length) {
                super.handleProtocolFrameToSend(header, data, offset, length);
                callCheck(currentCheckMethod, data, header, offset, length);
            }
        };
        sut.setVersion((byte) 2);
    }

    public void testSendMobileNavSmallFrameProtocolMessageSucceed() throws Exception {
        ProtocolMessage message = generateMobileNavProtocolMessage(8);
        currentData = generateByteArray(0, 8);
        currentFrameHeader = ProtocolFrameHeaderFactory.createSingleSendData(SessionType.Mobile_Nav, SESSION_ID, currentData.length, MESSAGE_ID, VERSION);
        currentCheckMethod = generateCurrentCheckMethod("checkCurrentArgumentsSmallFrame");
        sut.SendMessage(message);
    }

    public void testSendMobileNavFirstBigFrameProtocolMessageSucceed() throws Exception {
        ProtocolMessage message = generateMobileNavProtocolMessage(WiProProtocol.MAX_DATA_SIZE * 2);
        currentData = generateByteArray(0, WiProProtocol.MAX_DATA_SIZE * 2);
        currentFrameHeader = ProtocolFrameHeaderFactory.createMultiSendDataFirst(SessionType.Mobile_Nav, SESSION_ID, MESSAGE_ID, VERSION);
        currentCheckMethod = generateCurrentCheckMethod("checkCurrentArgumentsFirstBigFrame");
        sut.SendMessage(message);
    }

    public void testSendMobileNavConsecutiveBigFrameProtocolMessageSucceed() throws Exception {
        ProtocolMessage message = generateMobileNavProtocolMessage(WiProProtocol.MAX_DATA_SIZE * 3);
        currentData = generateByteArray(0, WiProProtocol.MAX_DATA_SIZE);
        currentFrameHeader = ProtocolFrameHeaderFactory.createMultiSendDataRest(SessionType.Mobile_Nav, SESSION_ID, currentData.length, FRAME_SEQUENCE_NUMBER, MESSAGE_ID, VERSION);
        currentCheckMethod = generateCurrentCheckMethod("checkCurrentArgumentsSecondBigFrame");
        sut.SendMessage(message);
    }

    public void testSendMobileNavLastBigFrameProtocolMessageSucceed() throws Exception {
        ProtocolMessage message = generateMobileNavProtocolMessage(WiProProtocol.MAX_DATA_SIZE * 3);
        currentData = generateByteArray(WiProProtocol.MAX_DATA_SIZE * 2, WiProProtocol.MAX_DATA_SIZE);
        currentFrameHeader = ProtocolFrameHeaderFactory.createMultiSendDataRest(SessionType.Mobile_Nav, SESSION_ID, currentData.length, (byte) 0, MESSAGE_ID, VERSION);
        currentCheckMethod = generateCurrentCheckMethod("checkCurrentArgumentsLastBigFrame");
        sut.SendMessage(message);
    }

    public void testSendMobileNabLastUnAlightedBigFrameProtocolMessageSucceed() throws Exception {
        ProtocolMessage message = generateMobileNavProtocolMessage(WiProProtocol.MAX_DATA_SIZE * 3 + FRAME_SIZE_SHIFT);
        currentData = generateByteArray(WiProProtocol.MAX_DATA_SIZE * 3, FRAME_SIZE_SHIFT);
        currentFrameHeader = ProtocolFrameHeaderFactory.createMultiSendDataRest(SessionType.Mobile_Nav, SESSION_ID, currentData.length, (byte) 0, MESSAGE_ID, VERSION);
        currentCheckMethod = generateCurrentCheckMethod("checkCurrentArgumentsLastUnAlightedBigFrame");
        sut.SendMessage(message);
    }

    private Method generateCurrentCheckMethod(String checkMethodName) throws NoSuchMethodException {
        Class[] parameterTypes = new Class[4];
        parameterTypes[0] = byte[].class;
        parameterTypes[1] = ProtocolFrameHeader.class;
        parameterTypes[2] = int.class;
        parameterTypes[3] = int.class;
        return WiProProtocolTest.class.getMethod(checkMethodName, parameterTypes);
    }

    private ProtocolMessage generateMobileNavProtocolMessage(int i) {
        byte[] frame = generateByteArray(0, i);
        ProtocolMessage message = new ProtocolMessage();
        message.setData(frame);
        message.setVersion((byte) 2);
        message.setSessionID((byte) 48);
        message.setSessionType(SessionType.Mobile_Nav);
        return message;
    }

    private byte[] generateByteArray(int offset, int size) {
        byte[] b = new byte[size];
        for (int i = 0; i < size; i++) {
            b[i] = (byte) (i + offset);
        }
        return b;
    }

    private void callCheck(Method method, byte[] data, ProtocolFrameHeader messageHeader, int offset, int length) {
        Object[] parameters = new Object[4];
        parameters[0] = data;
        parameters[1] = messageHeader;
        parameters[2] = offset;
        parameters[3] = length;
        try {
            method.invoke(WiProProtocolTest.this, parameters);
        } catch (IllegalAccessException e) {
            Log.e("SyncProxyTester", e.toString());
        } catch (InvocationTargetException e) {
            reconstructAssertionError(e);
        }
    }

    private void reconstructAssertionError(InvocationTargetException e) {
        String methodName = getMethodName(e);
        assertNull("Should not get here. " + e.getCause().getMessage() + " " + methodName, e);
    }

    private String getMethodName(InvocationTargetException e) {
        String methodName = "";
        if (e.getCause() != null && e.getCause().getStackTrace() != null && e.getCause().getStackTrace().length > 0) {
            for (int i = 0; i < e.getCause().getStackTrace().length; i++) {
                if (e.getCause().getStackTrace()[i].toString().contains(this.getClass().getName())) {
                    methodName = e.getCause().getStackTrace()[i].toString();
                    break;
                }
            }
        }
        return methodName;
    }

    public void checkCurrentArgumentsSmallFrame(byte[] data, ProtocolFrameHeader messageHeader, int offset, int length) throws Exception {
        assertTrue(Arrays.equals(currentData, data));
        assertEquals("SessionType should be equal.", currentFrameHeader.getSessionType(), messageHeader.getSessionType());
        assertEquals("FrameType should be equal.", currentFrameHeader.getFrameType(), messageHeader.getFrameType());
        assertEquals("FrameData should be equal.", currentFrameHeader.getFrameData(), messageHeader.getFrameData());
        assertEquals("Version should be equal.", currentFrameHeader.getVersion(), messageHeader.getVersion());
        assertEquals("Compressed state should be equal.", currentFrameHeader.isCompressed(), messageHeader.isCompressed());
        assertEquals("Frame headers should be equal.", currentFrameHeader.getDataSize(), messageHeader.getDataSize());
        assertEquals("DataSize should be equal.", currentFrameHeader.getMessageID(), messageHeader.getMessageID());
        assertEquals("Frame headers should be equal.", currentFrameHeader.getSessionID(), messageHeader.getSessionID());
    }

    public void checkCurrentArgumentsFirstBigFrame(byte[] data, ProtocolFrameHeader messageHeader, int offset, int length) throws Exception {
        if (messageHeader.getFrameType() == FrameType.First) {
            byte[] firstFrameData = getFirstFrameData(currentData);
            assertTrue("Arrays should be equal.", Arrays.equals(firstFrameData, data));
            assertEquals("SessionType should be equal.", currentFrameHeader.getSessionType(), messageHeader.getSessionType());
            assertEquals("FrameType should be equal.", currentFrameHeader.getFrameType(), messageHeader.getFrameType());
            assertEquals("FrameData should be equal.", currentFrameHeader.getFrameData(), messageHeader.getFrameData());
            assertEquals("Version should be equal.", currentFrameHeader.getVersion(), messageHeader.getVersion());
            assertEquals("Compressed state should be equal.", currentFrameHeader.isCompressed(), messageHeader.isCompressed());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getDataSize(), messageHeader.getDataSize());
            assertEquals("DataSize should be equal.", currentFrameHeader.getMessageID(), messageHeader.getMessageID());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getSessionID(), messageHeader.getSessionID());
        }
    }

    public void checkCurrentArgumentsSecondBigFrame(byte[] data, ProtocolFrameHeader messageHeader, int offset, int length) throws Exception {
        if (messageHeader.getFrameType() == FrameType.Consecutive && messageHeader.getFrameData() == (byte) 1) {
            assertTrue("Length of data should be less then WiProProtocol.MAX_DATA_SIZE", length <= WiProProtocol.MAX_DATA_SIZE);
            byte[] res = getDataToCheck(data, offset, length);
            assertTrue("Arrays should be equal.", Arrays.equals(currentData, res));
            assertTrue("Offset should be 0 for second frame", offset == 0);
            assertEquals("SessionType should be equal.", currentFrameHeader.getSessionType(), messageHeader.getSessionType());
            assertEquals("FrameType should be equal.", currentFrameHeader.getFrameType(), messageHeader.getFrameType());
            assertEquals("FrameData should be equal.", currentFrameHeader.getFrameData(), messageHeader.getFrameData());
            assertEquals("Version should be equal.", currentFrameHeader.getVersion(), messageHeader.getVersion());
            assertEquals("Compressed state should be equal.", currentFrameHeader.isCompressed(), messageHeader.isCompressed());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getDataSize(), messageHeader.getDataSize());
            assertEquals("DataSize should be equal.", currentFrameHeader.getMessageID(), messageHeader.getMessageID());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getSessionID(), messageHeader.getSessionID());
        }
    }

    private byte[] getDataToCheck(byte[] data, int offset, int length) {
        byte[] res = new byte[length];
        System.arraycopy(data, offset, res, 0, length);
        return res;
    }

    public void checkCurrentArgumentsLastBigFrame(byte[] data, ProtocolFrameHeader messageHeader, int offset, int length) throws Exception {
        if (messageHeader.getFrameType() == FrameType.Consecutive && messageHeader.getFrameData() == (byte) 0) {
            assertTrue("Length of data should be less then WiProProtocol.MAX_DATA_SIZE", length <= WiProProtocol.MAX_DATA_SIZE);
            byte[] res = getDataToCheck(data, offset, length);
            assertTrue("Arrays should be equal.", Arrays.equals(currentData, res));
            assertTrue("Offset should be 2976 for last frame", offset == WiProProtocol.MAX_DATA_SIZE * 3 - length);
            assertEquals("SessionType should be equal.", currentFrameHeader.getSessionType(), messageHeader.getSessionType());
            assertEquals("FrameType should be equal.", currentFrameHeader.getFrameType(), messageHeader.getFrameType());
            assertEquals("FrameData should be equal.", currentFrameHeader.getFrameData(), messageHeader.getFrameData());
            assertEquals("Version should be equal.", currentFrameHeader.getVersion(), messageHeader.getVersion());
            assertEquals("Compressed state should be equal.", currentFrameHeader.isCompressed(), messageHeader.isCompressed());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getDataSize(), messageHeader.getDataSize());
            assertEquals("DataSize should be equal.", currentFrameHeader.getMessageID(), messageHeader.getMessageID());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getSessionID(), messageHeader.getSessionID());
        }
    }

    public void checkCurrentArgumentsLastUnAlightedBigFrame(byte[] data, ProtocolFrameHeader messageHeader, int offset, int length) throws Exception {
        if (messageHeader.getFrameType() == FrameType.Consecutive && messageHeader.getFrameData() == (byte) 0) {
            assertTrue("Length of data should be == 100", length == FRAME_SIZE_SHIFT);
            assertTrue("Offset of data should be == 4464", offset == WiProProtocol.MAX_DATA_SIZE * 3);
            byte[] res = getDataToCheck(data, offset, length);
            assertTrue("Arrays should be equal.", Arrays.equals(currentData, res));
            assertEquals("SessionType should be equal.", currentFrameHeader.getSessionType(), messageHeader.getSessionType());
            assertEquals("FrameType should be equal.", currentFrameHeader.getFrameType(), messageHeader.getFrameType());
            assertEquals("FrameData should be equal.", currentFrameHeader.getFrameData(), messageHeader.getFrameData());
            assertEquals("Version should be equal.", currentFrameHeader.getVersion(), messageHeader.getVersion());
            assertEquals("Compressed state should be equal.", currentFrameHeader.isCompressed(), messageHeader.isCompressed());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getDataSize(), messageHeader.getDataSize());
            assertEquals("DataSize should be equal.", currentFrameHeader.getMessageID(), messageHeader.getMessageID());
            assertEquals("Frame headers should be equal.", currentFrameHeader.getSessionID(), messageHeader.getSessionID());
        }
    }

    private byte[] getFirstFrameData(byte[] data) {
        int frameCount = data.length / WiProProtocol.MAX_DATA_SIZE;
        if (data.length % WiProProtocol.MAX_DATA_SIZE > 0) {
            frameCount++;
        }
        byte[] firstFrameData = new byte[8];
        // First four bytes are data size.
        System.arraycopy(BitConverter.intToByteArray(data.length), 0, firstFrameData, 0, 4);
        // Second four bytes are frame count.
        System.arraycopy(BitConverter.intToByteArray(frameCount), 0, firstFrameData, 4, 4);
        return firstFrameData;
    }

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

        final byte[] msgFirstBytes = new byte[]{0x20, 0x07, 0x02, 0x01};
        StartSessionACKMessageStream.write(msgFirstBytes);
        final byte[] msgDataSize = new byte[]{0x00, 0x00, 0x00, 0x00};
        StartSessionACKMessageStream.write(msgDataSize);
        final byte[] msgHashID =
                new byte[]{0x12, 0x34, (byte) 0xCD, (byte) 0xEF};
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
                        new byte[]{(byte) 0xCD, (byte) 0xEF, 0x12, 0x34};

                switch (sendCount) {
                    case 0:
                        final byte[] msgDataSize =
                                extractByteArrayPart(msgBytes, 4, 4);
                        final byte[] expectedMsgDataSize =
                                new byte[]{0x00, 0x00, 0x00, 0x04};
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

            @Override
            public void onMobileNavAckReceived(int frameReceivedNumber) {

            }

            @Override
            public void onProtocolAppUnregistered() {

            }
        };

        final WiProProtocol protocol = new WiProProtocol(protocolListener);
        protocol.hashID = 0xCDEF1234;
        protocol._version = 2;
        protocol.EndProtocolSession(SessionType.RPC, (byte) 0x01);
    }
}
