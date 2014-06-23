package com.ford.syncV4.protocol.secure;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.ServiceType;

/**
 * Created by Andrew Batutin on 3/18/14.
 */
public class SecureStartServiceTest  extends AndroidTestCase {

    private static final byte VERSION = 0x1;
    private static final byte SESSION_ID = 0x0;

    public void testStartSecureSessionPacketIsEncrypted() throws Exception {
        ProtocolFrameHeader protocolMessage = ProtocolFrameHeaderFactory.createStartSecureService(ServiceType.RPC, (byte)0,(byte)1);
        assertEquals(protocolMessage.isEncrypted(), true);
    }

    public void testStartSecureSessionSetSessionType() throws Exception {
        ProtocolFrameHeader protocolMessage = ProtocolFrameHeaderFactory.createStartSecureService(ServiceType.RPC,(byte)0,(byte)1);
        assertEquals(protocolMessage.getServiceType(), ServiceType.RPC);
        ProtocolFrameHeader protocolMessageAudio = ProtocolFrameHeaderFactory.createStartSecureService(ServiceType.Audio_Service,(byte)0,(byte)1);
        assertEquals(protocolMessageAudio.getServiceType(), ServiceType.Audio_Service);
    }

    public void testStartSecureSessionPacketIsFormedCorrectly() throws Exception {
        ProtocolFrameHeader protocolMessage = ProtocolFrameHeaderFactory.createStartSecureService(ServiceType.RPC, (byte)0,(byte)1);
        assertEquals(VERSION, protocolMessage.getVersion());
        assertEquals(true, protocolMessage.isEncrypted());
        assertEquals( FrameType.Control, protocolMessage.getFrameType());
        assertEquals( ServiceType.RPC, protocolMessage.getServiceType());
        assertEquals(FrameDataControlFrameType.StartService.getValue(), protocolMessage.getFrameData());
        assertEquals(SESSION_ID, protocolMessage.getSessionId());
        assertEquals(0, protocolMessage.getDataSize());
    }



}
