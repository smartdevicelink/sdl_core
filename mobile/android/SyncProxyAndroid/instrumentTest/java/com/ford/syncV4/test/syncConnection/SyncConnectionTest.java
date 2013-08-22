package com.ford.syncV4.test.syncConnection;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.BaseTransportConfig;

import java.util.Arrays;

import static org.mockito.Mockito.mock;

/**
 * Created by Andrew Batutin on 8/22/13.
 */
public class SyncConnectionTest extends InstrumentationTestCase {

    public static final byte VERSION = (byte) 2;
    private SyncConnection sut;

    public SyncConnectionTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
        sut = new SyncConnection(mock(ISyncConnectionListener.class), mock(BaseTransportConfig.class));
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

}
