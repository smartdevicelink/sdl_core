package com.ford.syncV4.test.protocol;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.SessionType;

import java.util.Arrays;
import java.util.Hashtable;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;


/**
 * Created by Andrew Batutin on 8/21/13.
 */
public class WiProProtocolTest extends InstrumentationTestCase {

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
                _messageLocks = mock(Hashtable.class);
                when(_messageLocks.get((byte) 0)).thenReturn("mockLock");
                super.SendMessage(protocolMsg);
            }

            @Override
            protected void handleProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset, int length) {
                super.handleProtocolFrameToSend(header, data, offset, length);
                try {
                    checkCurrentArguments(data, header);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        sut.setVersion((byte) 2);
    }

    public void testSendMobileNavSmallFrameProtocolMessageSucceed() throws Exception {
        ProtocolMessage message = generateMobileNavProtocolMessage();
        currentData = new byte[]{1, 2, 3, 4, 5, 7};
        currentFrameHeader = ProtocolFrameHeaderFactory.createSingleSendData(SessionType.Mobile_Nav, (byte) 48, currentData.length, 1, (byte) 2);
        sut.SendMessage(message);
    }

    private ProtocolMessage generateMobileNavProtocolMessage() {
        byte[] frame = new byte[]{1, 2, 3, 4, 5, 7};
        ProtocolMessage message = new ProtocolMessage();
        message.setData(frame);
        message.setVersion((byte)2);
        message.setSessionID((byte) 48);
        message.setSessionType(SessionType.Mobile_Nav);
        return message;
    }

    public void checkCurrentArguments(byte[] data, ProtocolFrameHeader messageHeader) throws Exception {
        assertTrue(Arrays.equals(currentData, data));
        assertEquals(currentFrameHeader, messageHeader);
    }
}
