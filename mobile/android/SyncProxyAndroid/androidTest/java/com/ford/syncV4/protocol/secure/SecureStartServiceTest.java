package com.ford.syncV4.protocol.secure;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;

/**
 * Created by Andrew Batutin on 3/18/14.
 */
public class SecureStartServiceTest  extends AndroidTestCase {

    public void testStartSecureSessionPacketIsEncrypted() throws Exception {
        ProtocolFrameHeader protocolMessage = ProtocolFrameHeaderFactory.createStartSecureService((byte)0,(byte)1);
        assertEquals(protocolMessage.isEncrypted(), true);
    }
}
