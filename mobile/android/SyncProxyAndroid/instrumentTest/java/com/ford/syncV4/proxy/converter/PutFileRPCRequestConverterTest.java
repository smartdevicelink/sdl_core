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

import org.hamcrest.MatcherAssert;
import org.hamcrest.Matchers;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.List;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.greaterThan;
import static org.hamcrest.Matchers.nullValue;
import static org.hamcrest.core.Is.is;
import static org.hamcrest.core.IsNull.notNullValue;
import static org.mockito.Mockito.mock;

/**
 * Tests for PutFileRPCRequestConverter class.
 *
 * Created by enikolsky on 2014-01-21.
 */
public class PutFileRPCRequestConverterTest extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final String NUMBER_OF_OBJECTS_IS_INCORRECT =
            "Number of objects is incorrect";
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private PutFileRPCRequestConverter converter;
    private JsonRPCMarshaller marshaller;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        converter = new PutFileRPCRequestConverter();
        marshaller = new JsonRPCMarshaller();
    }

    public void testDefaultMaxDataSizeShouldBeLessThanProtocolMaxDataSize() {
        WiProProtocol protocol =
                new WiProProtocol(mock(IProtocolListener.class));
        protocol.setVersion(PROTOCOL_VERSION);

        MatcherAssert.assertThat(converter.getMaxDataSize(),
                Matchers.lessThanOrEqualTo(WiProProtocol.MAX_DATA_SIZE));
    }

    public void testGetProtocolMessagesShouldReturnEmptyListIfRequestIsNotPutFile() {
        assertThat(
                converter.getProtocolMessages(new Show(), (byte) 0, marshaller,
                        PROTOCOL_VERSION).size(), is(0));
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
        assertThat(pm.getData(), notNullValue());
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

    public void testMediumPutFileShouldBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        converter.setMaxDataSize(100);

        final int extraDataSize = 10;
        final int maxDataSize = converter.getMaxDataSize();
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(2));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkOffsetAndLengthInJSON(json0, 0, maxDataSize);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));

        ProtocolMessage pm1 = protocolMessages.get(1);
        commonPutFileProtocolMessageAsserts(pm1);
        final byte[] json1 = pm1.getData();
        checkOffsetAndLengthInJSON(json1, maxDataSize, dataSize - maxDataSize);
        assertThat(pm1.getJsonSize(), is(json1.length));
        assertThat(pm1.getSessionID(), is(sessionID));
        assertThat(pm1.getCorrID(), is(correlationID));
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize, dataSize);
        assertThat(pm1.getBulkData(), is(data1));
    }

    // check a corner case when "data size" == "max data size" * N
    public void testPutFileWithDivisibleBulkDataSizeShouldBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        converter.setMaxDataSize(100);

        final int maxDataSize = converter.getMaxDataSize();
        final int N = 2;
        final int dataSize = maxDataSize * N;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(2));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkOffsetAndLengthInJSON(json0, 0, maxDataSize);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));

        ProtocolMessage pm1 = protocolMessages.get(1);
        commonPutFileProtocolMessageAsserts(pm1);
        final byte[] json1 = pm1.getData();
        checkOffsetAndLengthInJSON(json1, maxDataSize, dataSize - maxDataSize);
        assertThat(pm1.getJsonSize(), is(json1.length));
        assertThat(pm1.getSessionID(), is(sessionID));
        assertThat(pm1.getCorrID(), is(correlationID));
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize, dataSize);
        assertThat(pm1.getBulkData(), is(data1));
    }

    public void testBigPutFileShouldBeSplitIntoMultipleProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        final int maxDataSize = converter.getMaxDataSize();
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
                is(dataCopies + 1));

        for (int i = 0; i < dataCopies; ++i) {
            final ProtocolMessage pm = protocolMessages.get(i);
            final byte[] json = pm.getData();
            checkOffsetAndLengthInJSON(json, i * maxDataSize, maxDataSize);
        }

        final ProtocolMessage pm = protocolMessages.get(dataCopies);
        final byte[] json = pm.getData();
        checkOffsetAndLengthInJSON(json, dataCopies * maxDataSize,
                extraDataSize);
    }

    private void checkOffsetAndLengthInJSON(byte[] data, int offset, int length)
            throws JSONException {
        assertThat(data, notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat(jsonObject.getInt(OFFSET), is(offset));
        assertThat(jsonObject.getInt(LENGTH), is(length));
    }

    private void commonPutFileProtocolMessageAsserts(
            ProtocolMessage protocolMessage) {
        assertThat(protocolMessage.getVersion(), is(PROTOCOL_VERSION));
        assertThat(protocolMessage.getFunctionID(), is(PUTFILE_FUNCTIONID));
        assertThat(protocolMessage.getMessageType(), is(MessageType.RPC));
    }
}
