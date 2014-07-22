package com.ford.syncV4.protocol;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.session.SessionTest;

import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

import static org.mockito.Mockito.mock;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 19.06.14
 * Time: 11:49
 */
public class MessageFrameAssemblerTest extends InstrumentationTestCase {

    private static final int MSG_ID = 3467;

    private MessageFrameAssemblerListener mListener;
    private MessageFrameAssembler mAssembler;
    private ArgumentCaptor<ServiceType> mServiceTypeCaptor;
    private ArgumentCaptor<Byte> mSessionIdCaptor;
    private ArgumentCaptor<Byte> mProtocolVersionCaptor;
    private ArgumentCaptor<Integer> mMessageIdCaptor;
    private ArgumentCaptor<Boolean> mIsEncryptedCaptor;

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        System.setProperty("dexmaker.dexcache",
                getInstrumentation().getTargetContext().getCacheDir().getPath());

        mListener = mock(MessageFrameAssemblerListener.class);
        mAssembler = new MessageFrameAssembler(mListener);

        mSessionIdCaptor = ArgumentCaptor.forClass(byte.class);
        mProtocolVersionCaptor = ArgumentCaptor.forClass(byte.class);
        mMessageIdCaptor = ArgumentCaptor.forClass(Integer.class);
        mServiceTypeCaptor = ArgumentCaptor.forClass(ServiceType.class);
        mIsEncryptedCaptor = ArgumentCaptor.forClass(boolean.class);
    }

    public void testConstructorWitNullListenerFail() {
        try {
            MessageFrameAssembler assembler = new MessageFrameAssembler(null);
            fail("Expected NullPointerException to be thrown");
        } catch (NullPointerException e) {
            assertTrue(true);
        }
    }

    public void testConstructorWitListenerNotFail() {
        assertNotNull(mAssembler);
    }

    public void testHandleHeartbeatACK() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.HeartbeatACK.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onHeartbeatACK(mSessionIdCaptor.capture());
        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
    }

    public void testHandleHeartbeat() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.Heartbeat.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onHeartbeat(mSessionIdCaptor.capture());
        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
    }

    public void testHandleEndServiceACK() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.EndServiceACK.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onEndServiceACK(mSessionIdCaptor.capture(),
                mMessageIdCaptor.capture(), mServiceTypeCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
        assertEquals(MSG_ID, mMessageIdCaptor.getValue().intValue());
        assertEquals(ServiceType.RPC, mServiceTypeCaptor.getValue());
    }

    public void testHandleEndService() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.EndService.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onEndService(mSessionIdCaptor.capture(),
                mMessageIdCaptor.capture(), mServiceTypeCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
        assertEquals(MSG_ID, mMessageIdCaptor.getValue().intValue());
        assertEquals(ServiceType.RPC, mServiceTypeCaptor.getValue());
    }

    public void testHandleEndServiceNACK() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.EndServiceNACK.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onEndServiceNACK(mSessionIdCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
    }

    public void testHandleMobileNaviACK() {
        ProtocolFrameHeader protocolFrameHeader = getProtocolFrameHeader(
                FrameDataControlFrameType.MobileNaviACK.getValue());
        protocolFrameHeader.setServiceType(ServiceType.Mobile_Nav);
        mAssembler.handleFrame(protocolFrameHeader, new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onMobileNaviACK(mSessionIdCaptor.capture(),
                mMessageIdCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
        assertEquals(MSG_ID, mMessageIdCaptor.getValue().intValue());
    }

    public void testHandleStartService() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.StartService.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onStartService(mSessionIdCaptor.capture(),
                mServiceTypeCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
        assertEquals(ServiceType.RPC, mServiceTypeCaptor.getValue());
    }

    public void testHandleStartServiceACK() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.StartServiceACK.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onStartServiceACK(mSessionIdCaptor.capture(),
                mMessageIdCaptor.capture(), mServiceTypeCaptor.capture(),
                mProtocolVersionCaptor.capture(), mIsEncryptedCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
        assertEquals(MSG_ID, mMessageIdCaptor.getValue().intValue());
        assertEquals(ServiceType.RPC, mServiceTypeCaptor.getValue());
        assertEquals(ProtocolConstants.PROTOCOL_VERSION_TWO, mProtocolVersionCaptor.getValue()
                .byteValue());
    }

    public void testHandleStartServiceNACK() {
        mAssembler.handleFrame(getProtocolFrameHeader(
                        FrameDataControlFrameType.StartServiceNACK.getValue()), new byte[0],
                ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);

        Mockito.verify(mListener, Mockito.times(1)).onStartServiceNACK(mSessionIdCaptor.capture(),
                mServiceTypeCaptor.capture());

        assertEquals(SessionTest.SESSION_ID, mSessionIdCaptor.getValue().byteValue());
        assertEquals(ServiceType.RPC, mServiceTypeCaptor.getValue());
    }

    private ProtocolFrameHeader getProtocolFrameHeader(byte frameType) {
        ProtocolFrameHeader protocolFrameHeader = new ProtocolFrameHeader();
        protocolFrameHeader.setFrameData(frameType);
        protocolFrameHeader.setFrameType(FrameType.Control);
        protocolFrameHeader.setSessionID(SessionTest.SESSION_ID);
        protocolFrameHeader.setServiceType(ServiceType.RPC);
        protocolFrameHeader.setMessageID(MSG_ID);
        protocolFrameHeader.setVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        protocolFrameHeader.setDataSize(0);
        return protocolFrameHeader;
    }
}