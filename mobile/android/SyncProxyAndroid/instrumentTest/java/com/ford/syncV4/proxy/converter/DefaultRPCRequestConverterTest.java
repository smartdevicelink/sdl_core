package com.ford.syncV4.proxy.converter;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;

import org.hamcrest.core.IsNull;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.Charset;
import java.util.List;

import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.greaterThan;
import static org.hamcrest.core.Is.is;
import static org.mockito.Mockito.mock;

/**
 * Tests for DefaultRPCRequestConverter class.
 *
 * Created by enikolsky on 2014-01-21.
 */
public class DefaultRPCRequestConverterTest extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final String NUMBER_OF_OBJECTS_IS_INCORRECT =
            "Number of objects is incorrect";
    private DefaultRPCRequestConverter converter;
    private JsonRPCMarshaller marshaller;
    private int maxDataSize;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        converter = new DefaultRPCRequestConverter();
        marshaller = new JsonRPCMarshaller();

        WiProProtocol protocol =
                new WiProProtocol(mock(IProtocolListener.class));
        protocol.setVersion(PROTOCOL_VERSION);
        maxDataSize = WiProProtocol.MAX_DATA_SIZE;
    }

    public void testGetProtocolMessagesForShowShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        Show msg = new Show();
        msg.setAlignment(TextAlignment.CENTERED);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(protocolMessages.size(), is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        assertThat(pm.getData(), notNullValue());
        assertThat(pm.getData().length, greaterThan(0));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getMessageType(), is(MessageType.RPC));
        assertThat(pm.getFunctionID(), is(13));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), nullValue());
    }

    public void testGetProtocolMessagesForSmallPutFileShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;
        final byte[] data = TestCommon.getRandomBytes(32);

        PutFile msg = new PutFile();
        msg.setSyncFileName("file");
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm);
        assertThat(pm.getData(), IsNull.notNullValue());
        assertThat(pm.getJsonSize(), greaterThan(0));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), is(data));
    }

    // or should it?
    public void testGetProtocolMessagesForPutFileWithoutBulkDataShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        PutFile msg = new PutFile();
        msg.setSyncFileName("file");
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm);
        assertThat(pm.getData(), notNullValue());
        assertThat(pm.getJsonSize(), greaterThan(0));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), nullValue());
    }

    public void testMediumPutFileShouldNotBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkNoOffsetAndLengthInJSON(json0);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        assertThat(pm0.getBulkData(), is(data));
    }

    // check a corner case when "data size" == "max data size" * N
    public void testPutFileWithDivisibleBulkDataSizeShouldNotBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        final int dataSize = maxDataSize * 2;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkNoOffsetAndLengthInJSON(json0);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        assertThat(pm0.getBulkData(), is(data));
    }

    public void testBigPutFileShouldNotBeSplitIntoMultipleProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        final int dataCopies = 12;
        final int extraDataSize = 20;
        final int dataSize = (maxDataSize * dataCopies) + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        final ProtocolMessage pm = protocolMessages.get(0);
        final byte[] json = pm.getData();
        checkNoOffsetAndLengthInJSON(json);
        assertThat(pm.getBulkData(), is(data));
    }

    public void testGetProtocolMessagesShouldThrowNullPointerExceptionWhenRequestIsNull() {
        try {
            converter.getProtocolMessages(null, (byte) 0, marshaller,
                    PROTOCOL_VERSION);
            fail("Should have thrown NullPointerException");
        } catch (NullPointerException e) {
            // success
        }
    }

    public void testGetProtocolMessagesShouldThrowNullPointerExceptionWhenMarshallerIsNull() {
        try {
            Show msg = new Show();

            converter.getProtocolMessages(msg, (byte) 0, null,
                    PROTOCOL_VERSION);
            fail("Should have thrown NullPointerException");
        } catch (NullPointerException e) {
            // success
        }
    }

    private void commonPutFileProtocolMessageAsserts(
            ProtocolMessage protocolMessage) {
        assertThat(protocolMessage.getVersion(), is(PROTOCOL_VERSION));
        assertThat(protocolMessage.getFunctionID(), is(PUTFILE_FUNCTIONID));
        assertThat(protocolMessage.getMessageType(), is(MessageType.RPC));
    }

    private void checkNoOffsetAndLengthInJSON(byte[] data)
            throws JSONException {
        assertThat(data, notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat(jsonObject.has(OFFSET), is(false));
        assertThat(jsonObject.has(LENGTH), is(false));
    }
}
